// 03/05/2022 by BKHao in Chimes.

#pragma once
#include <mutex>
#include <assert.h>
namespace Chimes
{
	class MemoryAlign
	{
	public:
		static void*& next_block(void* block);
		static size_t Index(size_t byte_size);
		static size_t Roundup(size_t byte_size);
		static size_t BlockNumberNeedMove(size_t byte_size);
		static size_t PageNumberNeedMove(size_t byte_size);
		static size_t Index(size_t byte_size, size_t align);
		static size_t Roundup(size_t byte_size, size_t align);

	public:
		static const size_t MAX_BYTES;
		static const size_t NLISTS;
		static const size_t PAGE_SHIFE;
		static const size_t NPAGES;
	};

	struct Span
	{
		size_t pageid_ = 0;
		size_t npage_ = 0;
		Span* prev_ = nullptr;
		Span* next_ = nullptr;
		// TODO: redundant with pageid
		void* list_ = nullptr;
		size_t block_byte_size_ = 0;
		size_t usecount_ = 0;
	};

	class BlockList
	{
	public:
		void Push(void* block);

		void PushList(void* start, void* end, size_t n);

		void* Pop();

		void* PopList();

		bool Empty() const;

		size_t Size() const;

		size_t MaxSize() const;

		void SetMaxSize(size_t maxsize);

	private:
		void* list_ = nullptr;
		size_t size_ = 0;
		size_t maxsize_ = 1;
	};

	class SpanList
	{
	public:
		SpanList();

		~SpanList();

		SpanList(const SpanList&) = delete;

		SpanList& operator=(const SpanList&) = delete;

		Span* Begin();

		Span* End();

		bool Empty() const;

		void Insert(Span* cur, Span* newspan);

		void Erase(Span* cur);

		void PushBack(Span* newspan);

		void PushFront(Span* newspan);

		Span* PopBack();

		Span* PopFront();

		std::mutex& GetMutex();

	private:
		Span* head_;
		std::mutex mutex_;
	};

	struct MemoryBlock
	{
		void* ptr_;
		size_t ref_count_;
		std::mutex mutex_;
	};

} // namespace Chimes
