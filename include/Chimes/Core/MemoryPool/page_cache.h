// 04/05/2022 by BKHao in Chimes.

#pragma once
#include <unordered_map>
#include "Chimes/Core/MemoryPool/memory_common.h"

namespace Chimes
{
	class PageCache
	{
	public:
		static PageCache& GetInstance();
		~PageCache();
		Span* AllocateBigPageBlock(size_t byte_size);
		void DeallocateBigPageBlock(void* ptr, Span* span);
		Span* NewSpan(size_t n);
		Span* BlockToSpan(void* block);
		void TakebackSpan(Span* span);

	private:
		PageCache();
		PageCache(const PageCache&) = delete;
		PageCache& operator=(const PageCache*) = delete;

	private:
		SpanList* spanlists_;
		std::unordered_map<size_t, Span*> block_to_span_;
		std::mutex mutex_;
	};
} // namespace Chimes
