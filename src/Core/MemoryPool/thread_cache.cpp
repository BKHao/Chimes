// 03/05/2022 by BKHao in Chimes.
#include "Chimes/Core/MemoryPool/thread_cache.h"
#include "Chimes/Core/MemoryPool/central_cache.h"

namespace Chimes
{
	ThreadCache& ThreadCache::GetInstance()
	{
		thread_local static ThreadCache instance = ThreadCache();
		return instance;
	}

	void* ThreadCache::Allocate(size_t byte_size)
	{
		size_t index = MemoryAlign::Index(byte_size);
		BlockList& blocklist = blocklists_[index];
		if (!blocklist.Empty())
		{
			return blocklist.Pop();
		}
		else
		{
			return FetchFromCentralCache(index, MemoryAlign::Roundup(byte_size));
		}
	}

	void ThreadCache::Deallocate(void* ptr, size_t byte_size)
	{
		size_t index = MemoryAlign::Index(byte_size);
		BlockList& blocklist = blocklists_[index];
		blocklist.Push(ptr);
		
		if (blocklist.Size() >= blocklist.MaxSize())
		{
			// TODO: reference or point?
			ReleaseLongList(blocklist, byte_size);
		}
	}

	ThreadCache::ThreadCache()
	{
		blocklists_ = new BlockList[MemoryAlign::NLISTS];
	}

	ThreadCache::~ThreadCache()
	{
		// TODO: memory leak?
		delete[] blocklists_;
	}

	void* ThreadCache::FetchFromCentralCache(size_t index, size_t byte_size)
	{
		BlockList& blocklist = blocklists_[index];
		size_t maxsize = blocklist.MaxSize();
		size_t needNumber = std::min(MemoryAlign::BlockNumberNeedMove(byte_size), maxsize);

		void* start = nullptr;
		void* end = nullptr;

		size_t blocksize = CentralCache::GetInstance().FecthListBlock(start, end, needNumber, byte_size);
		if (blocksize > 1)
		{
			blocklist.PushList(MemoryAlign::next_block(start), end, blocksize - 1);
		}
		if (blocksize >= maxsize)
		{
			blocklist.SetMaxSize(maxsize + 1);
		}
		return start;

	}

	void ThreadCache::ReleaseLongList(BlockList& blocklist, size_t byte_size)
	{
		void* start = blocklist.PopList();
		CentralCache::GetInstance().TakebackBlocklist(start, byte_size);
	}
} // namespace Chimes