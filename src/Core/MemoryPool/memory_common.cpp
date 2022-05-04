// 03/05/2022 by BKHao in Chimes.
#include "Chimes/Core/MemoryPool/memory_common.h"

namespace Chimes
{
	const size_t MemoryAlign::MAX_BYTES = 65536;
	const size_t MemoryAlign::NLISTS = 184;
	const size_t MemoryAlign::PAGE_SHIFE = 12;
	const size_t MemoryAlign::NPAGES = 129;

	void*& MemoryAlign::next_block(void* block)
	{
		return *((void**)block);
	}

	size_t MemoryAlign::Index(size_t byte_size)
	{
#ifdef _DEBUG
		assert(byte_size <= MAX_BYTES);
#endif // _DEBUG
		if (byte_size <= 128)
		{
			return Index(byte_size, 3);
		}
		else if (byte_size <= 1024)
		{
			return Index(byte_size - 128, 4) + 16;
		}
		else if (byte_size <= 8192)
		{
			return Index(byte_size - 1024, 7) + 72;
		}
		else
		{
			return Index(byte_size - 8192, 10) + 128;
		}
	}
	size_t MemoryAlign::Roundup(size_t byte_size)
	{
#ifdef _DEBUG
		assert(byte_size <= MAX_BYTES);
#endif // _DEBUG
		if (byte_size <= 128)
		{
			return Roundup(byte_size, 3);
		}
		else if (byte_size <= 1024)
		{
			return Roundup(byte_size, 4);
		}
		else if (byte_size <= 8192)
		{
			return Index(byte_size, 7);
		}
		else
		{
			return Index(byte_size, 10);
		}
	}
	size_t MemoryAlign::BlockNumberNeedMove(size_t byte_size)
	{
		if (byte_size == 0)
		{
			return 0;
		}
		int num = int(MAX_BYTES / byte_size);
		if (num < 2)
		{
			return 2;
		}
		if (num > 512)
		{
			return 512;
		}
		return num;
	}
	size_t MemoryAlign::PageNumberNeedMove(size_t byte_size)
	{
		size_t num = BlockNumberNeedMove(byte_size);
		size_t npage = num * byte_size;
		// TODO: +1?
		npage >>= PAGE_SHIFE;
		if (npage == 0)
			return 1;
		return npage;
	}
	size_t MemoryAlign::Index(size_t byte_size, size_t align)
	{
		size_t alignnum = 1 << align;
		return (byte_size + alignnum - 1) & (~(alignnum - 1));
	}
	size_t MemoryAlign::Roundup(size_t size, size_t align)
	{
		size_t alignnum = 1 << align;
		return (size + alignnum - 1) & (~(alignnum - 1));
	}

	void BlockList::Push(void* block)
	{
		MemoryAlign::next_block(block) = list_;
		list_ = block;
		++size_;
	}

	void BlockList::PushList(void* start, void* end, size_t n)
	{
		MemoryAlign::next_block(end) = list_;
		list_ = start;
		size_ += n;
	}

	void* BlockList::Pop()
	{
		void* block = list_;
		list_ = MemoryAlign::next_block(block);
		--size_;
		return block;
	}

	void* BlockList::PopList()
	{
		size_ = 0;
		void* list = list_;
		list_ = nullptr;
		return list;
	}

	bool BlockList::Empty() const
	{
		return list_ == nullptr;
	}

	size_t BlockList::Size() const
	{
		return size_;
	}

	size_t BlockList::MaxSize() const
	{
		return maxsize_;
	}

	void BlockList::SetMaxSize(size_t maxsize)
	{
		maxsize_ = maxsize;
	}

	SpanList::SpanList()
	{
		head_ = new Span;
		head_->next_ = head_;
		head_->prev_ = head_;
	}
	SpanList::~SpanList()
	{
		Span* cur = head_->next_;
		while (cur != head_)
		{
			Span* next = cur->next_;
			delete cur;
			cur = next;
		}
		delete head_;
		head_ = nullptr;
	}
	
	Span* SpanList::Begin()
	{
		return head_->next_;
	}

	Span* SpanList::End()
	{
		return head_;
	}

	bool SpanList::Empty() const
	{
		return head_->next_ = head_;
	}

	void SpanList::Insert(Span* cur, Span* newspan)
	{
		Span* prev = cur->prev_;
		prev->next_ = newspan;
		newspan->next_ = cur;
		newspan->prev_ = prev;
		cur->prev_ = newspan;
	}

	void SpanList::Erase(Span* cur)
	{
		Span* prev = cur->prev_;
		Span* next = cur->next_;
		prev->next_ = next;
		next->prev_ = prev;
	}

	void SpanList::PushBack(Span* newspan)
	{
		Insert(End(), newspan);
	}

	void SpanList::PushFront(Span* newspan)
	{
		Insert(Begin(), newspan);
	}

	Span* SpanList::PopBack()
	{
		Span* span = head_->prev_;
		Erase(span);
		return span;
	}

	Span* SpanList::PopFront()
	{
		Span* span = head_->next_;
		Erase(span);
		return span;
	}

	std::mutex& SpanList::GetMutex()
	{
		return mutex_;
	}
} // namespace Chimes