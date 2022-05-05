// 04/05/2022 by BKHao in Chimes.

#pragma once

#include "Chimes/Core/MemoryPool/thread_cache.h"
#include "Chimes/Core/MemoryPool/page_cache.h"

namespace Chimes
{
	template<typename T>
	class MemoryPtr
	{
		// TODO: At some point in the future, the concurrency here will need to be improved.
	public:
		MemoryPtr(): MemoryPtr(1)
		{

		}
		MemoryPtr(size_t n)
		{
			if (n == 0)
			{
				block_ = nullptr;
				ptr_ = nullptr;
				return;
			}
			block_ = new MemoryBlock;
			size_t byte_size = n * sizeof(T) + sizeof(void*);
			if (byte_size > MemoryAlign::MAX_BYTES)
			{
				Span* span = PageCache::GetInstance().AllocateBigPageBlock(byte_size);
				block_->ptr_ = (void*)(span->pageid_ << MemoryAlign::PAGE_SHIFE);
			}
			else
			{
				block_->ptr_ = ThreadCache::GetInstance().Allocate(byte_size);
			}
			block_->ref_count_ = 1;
			ptr_ = (T*)((size_t)block_->ptr_ + sizeof(void*));
		}

		MemoryPtr(const MemoryPtr& ptr)
		{
			if(ptr->block_)
			{
				std::unique_lock<std::mutex> lock(ptr.block_->mutex_);
				++ptr.block_->ref_count_;
			}
			ptr_ = ptr.ptr_;
			block_ = ptr.block_;
		}

		MemoryPtr(MemoryPtr&& ptr)
		{
			ptr_ = ptr.ptr_;
			block_ = ptr.block_;
			ptr.ptr_ = nullptr;
			ptr.block_ = nullptr;
		}

		~MemoryPtr()
		{
			Dereference();
		}

		MemoryPtr& operator=(const MemoryPtr & ptr)
		{
			Dereference();
			{
				std::unique_lock<std::mutex> lock(ptr.block_->mutex_);
				++ptr.block_->ref_count_;
			}			
			block_ = ptr.block_;
			ptr_ = ptr.ptr_;			
			return *this;
		}

		MemoryPtr& operator=(MemoryPtr&& ptr)
		{
			Dereference();
			block_ = ptr.block_;
			ptr_ = ptr.ptr_;
			ptr.ptr_ = nullptr;
			ptr.block_ = nullptr;
			return *this;
		}

		T* Get()
		{
			return ptr_;
		}

		T& operator*()
		{
			return *ptr_;
		}

		T& operator[](size_t index)
		{
			return *(ptr_ + index);
		}

		const T& operator[](size_t index) const
		{
			return *(ptr_ + index);
		}

	private:
		void Dereference()
		{
			if (block_ == nullptr)
				return;
			bool need_delete = false;
			void* delete_ptr = nullptr;
			{
				// TODO: When reaching this point, the block may be nullptr under concurrency.
				std::unique_lock<std::mutex> lock(block_->mutex_);
				--block_->ref_count_;
				if (block_->ref_count_ == 0)
				{
					need_delete = true;
					delete_ptr = block_->ptr_;
				}
			}
			if (need_delete)
			{
				delete block_;
				Span* span = PageCache::GetInstance().BlockToSpan(delete_ptr);
				size_t byte_size = span->block_byte_size_;
				if (byte_size > MemoryAlign::MAX_BYTES)
				{
					PageCache::GetInstance().DeallocateBigPageBlock(delete_ptr, span);
				}
				else
				{
					ThreadCache::GetInstance().Deallocate(delete_ptr, byte_size);
				}
			}
			block_ = nullptr;
			ptr_ = nullptr;
		}
		MemoryBlock* block_ = nullptr;
		T* ptr_ = nullptr;
	};
} // namespace Chimes

