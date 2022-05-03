// 03/05/2022 by BKHao in Chimes.
#pragma once

#include "Chimes/Core/MemoryPool/memory_common.h"

namespace Chimes
{
	class ThreadCache
	{
	public:
		static ThreadCache& GetInstance();

		void* Allocate(size_t byte_size);

		void Deallocate(void* ptr, size_t byte_size);

	private:
		ThreadCache();

		~ThreadCache();

		void* FetchFromCentralCache(size_t index, size_t byte_size);

		void ReleaseLongList(BlockList* blocklist, size_t byte_size);

	private:
		BlockList* blocklists_;
	};
} // namespace Chimes