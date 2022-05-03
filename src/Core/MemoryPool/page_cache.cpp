// 04/05/2022 by BKHao in Chimes.
//#include <memoryapi.h>
#include "Chimes/Core/MemoryPool/page_cache.h"

namespace Chimes
{
	PageCache& PageCache::GetInstance()
	{
		static PageCache instance = PageCache();
		return instance;
	}
	PageCache::~PageCache()
	{
		// TODO: memory leak
		delete[] spanlists_;
	}
	Span* PageCache::AllocateBigPageBlock(size_t byte_size)
	{
#ifdef DEBUG
		assert(byte_size > MemoryAlign::MAX_BYTES);
#endif // DEBUG
		byte_size = MemoryAlign::Roundup(byte_size, MemoryAlign::PAGE_SHIFE);
		size_t npage = byte_size >> MemoryAlign::PAGE_SHIFE;
		if (npage < MemoryAlign::NPAGES)
		{
			Span* span = NewSpan(npage);
			span->block_byte_size_ = byte_size;
			return span;
		}
		else
		{
			// TODO: Virtual
			void* ptr;
			//void* ptr = VirtualAlloc(0, npage << MemoryAlign::PAGE_SHIFE, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
			if (ptr == nullptr)
				throw std::bad_alloc();
			Span* span = new Span;
			span->npage_ = npage;
			span->pageid_ = (size_t)ptr >> MemoryAlign::PAGE_SHIFE;
			span->block_byte_size_ = npage << MemoryAlign::PAGE_SHIFE;
			block_to_span_[span->pageid_] = span;
			return span;
		}

	}
	void PageCache::DeallocateBigPageBlock(void* ptr, Span* span)
	{
		size_t npage = span->block_byte_size_ >> MemoryAlign::PAGE_SHIFE;
		if (npage < MemoryAlign::NPAGES)
		{
			span->block_byte_size_ = 0;
			TakebackSpan(span);
		}
		else
		{
			block_to_span_.erase(span->pageid_);
			delete span;
			// TODO: Virtual
			//VirtualFree(ptr, 0, MEM_RELEASE);
		}
	}
	Span* PageCache::NewSpan(size_t n)
	{
		std::unique_lock<std::mutex> lock(mutex_);
#ifdef DEBUG
		assert(n < MemoryAlign::NPAGES);
#endif // DEBUG
		if (!spanlists_[n].Empty())
			return spanlists_[n].PopFront();
		for (size_t i = n + 1; i < MemoryAlign::NPAGES; ++i)
		{
			if (!spanlists_[i].Empty())
			{
				Span* span = spanlists_[i].PopFront();
				Span* split = new Span;
				split->pageid_ = span->pageid_;
				split->npage_ = n;
				span->pageid_ = span->pageid_ + n;
				span->npage_ = span->npage_ - n;
				for (size_t j = 0; j < n; ++j)
					block_to_span_[split->pageid_ + j] = split;
				spanlists_[span->npage_].PushFront(span);
				return split;
			}
		}

		Span* span = new Span;

		// TODO: Virtual
		void* ptr;
		//void* ptr = VirtualAlloc(0, (MemoryAlign::NPAGES - 1) * (1 << MemoryAlign::PAGE_SHIFE), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

		span->pageid_ = (size_t)ptr >> MemoryAlign::PAGE_SHIFE;
		span->npage_ = MemoryAlign::NPAGES - 1;
		for (size_t i = 0; i < span->npage_; ++i)
		{
			block_to_span_[span->pageid_ + i] = span;
		}

		if (n == span->npage_)
			return span;

		Span* split = new Span;
		split->pageid_ = span->pageid_;
		split->npage_ = n;
		span->pageid_ = span->pageid_ + n;
		span->npage_ = span->npage_ - n;
		for (size_t i = 0; i < n; ++i)
			block_to_span_[split->pageid_ + i] = split;
		spanlists_[span->npage_].PushFront(span);
		return split;
	}
	Span* PageCache::BlockToSpan(void* block)
	{
		size_t pageid = (size_t)block >> MemoryAlign::PAGE_SHIFE;
		auto it = block_to_span_.find(pageid);
		if (it != block_to_span_.end())
		{
			return it->second;
		}
		else
		{
#ifdef DEBUG
			assert(false);
#endif // DEBUG
			return nullptr;
		}
	}
	void PageCache::TakebackSpan(Span* cur)
	{
		std::unique_lock<std::mutex> lock(mutex_);
		if (cur->npage_ >= MemoryAlign::NPAGES)
		{
			void* ptr = (void*)(cur->pageid_ << MemoryAlign::PAGE_SHIFE);
			block_to_span_.erase(cur->pageid_);
			// TODO: Virtual
			//VirtualFree(ptr, 0, MEM_RELEASE);
			delete cur;
			return;
		}
		while (true)
		{
			size_t curid = cur->pageid_;
			size_t nextid = curid + cur->npage_;
			auto it = block_to_span_.find(nextid);
			if (it == block_to_span_.end())
				break;
			if (it->second->usecount_ != 0)
				break;
			Span* next = it->second;
			if (cur->npage_ + next->npage_ >= MemoryAlign::NPAGES)
				break;
			spanlists_[next->npage_].Erase(next);
			cur->npage_ += next->npage_;
			for (size_t i = 0; i < next->npage_; ++i)
				block_to_span_[next->pageid_ + i] = cur;
			delete next;
		}
		while (true)
		{
			size_t curid = cur->pageid_;
			size_t previd = curid - 1;
			auto it = block_to_span_.find(previd);
			if (it == block_to_span_.end())
				break;
			if (it->second->usecount_ != 0)
				break;
			Span* prev = it->second;
			if (cur->npage_ + prev->npage_ >= MemoryAlign::NPAGES)
				break;
			spanlists_[prev->npage_].Erase(prev);
			prev->npage_ += cur->npage_;
			for (size_t i = 0; i < cur->npage_; ++i)
				block_to_span_[cur->pageid_ + i] = prev;
			delete cur;
			cur = prev;
		}
		spanlists_[cur->npage_].PushFront(cur);
	}
} // namespace Chimes
