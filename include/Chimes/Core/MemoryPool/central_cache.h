// 04/05/2022 by BKHao in Chimes.

#pragma once
#include "Chimes/Core/MemoryPool/memory_common.h"

namespace Chimes
{
	class CentralCache
	{
	public:
		// TODO: Singleton base class
		static CentralCache& GetInstance();

		~CentralCache();
		
		Span* GetSpan(SpanList& spanlist, size_t byte_size);

		size_t FecthListBlock(void*& start, void*& end, size_t n, size_t byte_size);

		void TakebackBlocklist(void* start, size_t byte_size);

	private:
		CentralCache();
		CentralCache(const CentralCache&) = delete;
		CentralCache& operator=(const CentralCache&) = delete;

	private:
		SpanList* spanlists_;

	};
} // namespace Chimes