#pragma once
#include "device_memory_allocator.hpp"
#include <volk/volk.h>
#include <tabris/types.hpp>
#include <tabris/vec.hpp>

namespace pl {
	struct TransientBuffer {
		VkDeviceAddress deviceAddress = {};
		void* hostAddress = nullptr;
		u64 writeOffset = 0;
		u64 size : 63 = 0;
		u64 deviceLocal : 1 = 0; // steal one bit to disambiguate staging vs scratch
	};

	struct BackedBuffer {
		VkDeviceMemory memory;
		VkBuffer buffer;
	};

	class TransientAllocator {
		public:
			TransientBuffer allocate(u64 size, b8 deviceLocal = false);
			void free(TransientBuffer buffer);

			TransientAllocator() = default;
			~TransientAllocator();

			TransientAllocator(TransientAllocator&& src) = delete;
			TransientAllocator& operator=(TransientAllocator&& src) = delete;
			TransientAllocator(const TransientAllocator&) = delete;
			TransientAllocator& operator=(const TransientAllocator&) = delete;

		private:
			static constexpr u64 m_scratchBufferSize = 1024ull * 1024ull;
			static constexpr u64 m_stagingBufferSize = 64ull * 1024ull * 1024ull;

			tbrs::Vec<Allocation> m_allocations;
			tbrs::Vec<TransientBuffer> m_freeList;
	};
}