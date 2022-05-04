// 04/05/2022 by BKHao in Chimes.

#include "Chimes/Core/MemoryPool/central_cache.h"
#include "Chimes/Core/MemoryPool/page_cache.h"

namespace Chimes
{
	CentralCache& CentralCache::GetInstance()
	{
		static CentralCache instance = CentralCache();
		return instance;
	}

	CentralCache::~CentralCache()
	{
		delete[] spanlists_;
	}

	Span* CentralCache::GetSpan(SpanList& spanlist, size_t byte_size)
	{
		Span* span = spanlist.Begin();

		while (span!= spanlist.End())
		{
			if (span->list_ != nullptr)
				return span;
			else
				span = span->next_;
		}

		Span* newspan = PageCache::GetInstance().NewSpan(MemoryAlign::PageNumberNeedMove(byte_size));
		// TODO: reinterpret_cast?
		char* cur = (char*)(newspan->pageid_ << MemoryAlign::PAGE_SHIFE);
		char* end = cur + (newspan->npage_ << MemoryAlign::PAGE_SHIFE);
		newspan->list_ = cur;
		newspan->block_byte_size_ = byte_size;
		while (cur + byte_size < end)
		{
			char* next = cur + byte_size;
			MemoryAlign::next_block(cur) = next;
			cur = next;
		}
		MemoryAlign::next_block(cur) = nullptr;
		spanlist.PushFront(newspan);
		return newspan;
	}

	size_t CentralCache::FecthListBlock(void*& start, void*& end, size_t n, size_t byte_size)
	{
		size_t index = MemoryAlign::Index(byte_size);
		SpanList& spanlist = spanlists_[index];

		std::unique_lock<std::mutex> lock(spanlist.GetMutex());

		Span* span = GetSpan(spanlist, byte_size);

		size_t blocksize = 0;
		void* prev = nullptr;
		void* cur = span->list_;
		for (size_t i = 0; i < n; ++i)
		{
			prev = cur;
			cur = MemoryAlign::next_block(cur);
			++blocksize;
			if (cur == nullptr)
				break;
		}
		start = span->list_;
		end = prev;
		span->list_ = cur;
		span->usecount_ += blocksize;
		if (span->list_ == nullptr)
		{
			spanlist.Erase(span);
			spanlist.PushBack(span);
		}
		return blocksize;
	}

	void CentralCache::TakebackBlocklist(void* start, size_t byte_size)
	{
		size_t index = MemoryAlign::Index(byte_size);
		SpanList& spanlist = spanlists_[index];

		std::unique_lock<std::mutex> lock(spanlist.GetMutex());
		while (start)
		{
			void* next = MemoryAlign::next_block(start);
			Span* span = PageCache::GetInstance().BlockToSpan(start);
			MemoryAlign::next_block(start) = span->list_;
			span->list_ = start;

			if (--span->usecount_ == 0)
			{
				spanlist.Erase(span);
				PageCache::GetInstance().TakebackSpan(span);
			}
			start = next;
		}
	}

	CentralCache::CentralCache()
	{
		spanlists_ = new SpanList[MemoryAlign::NLISTS];
	}
} // namespace Chimes