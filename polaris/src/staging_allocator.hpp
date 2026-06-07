#pragma once

#include <volk/volk.h>
#include <tabris/types.hpp>
#include <tabris/vec.hpp>

namespace pl {
	struct StagingBuffer {
		VkDeviceAddress gpuPtr = {};
		void* cpuPtr = nullptr;

		VkDeviceMemory memory = {};
		VkBuffer buffer = {};

		u64 writeOffset = 0;
		u64 size = 0;
	};

	class StagingAllocator {
		public:
			StagingBuffer alloc(u64 size);
			void free(StagingBuffer buffer);

			StagingAllocator();
			StagingAllocator(StagingAllocator&& src);
			StagingAllocator& operator=(StagingAllocator&& src);
			~StagingAllocator();

			StagingAllocator(const StagingAllocator&) = delete;
			StagingAllocator& operator=(const StagingAllocator&) = delete;

		private:
			VkPhysicalDeviceMemoryProperties m_memProps = {};

			tbrs::Vec<StagingBuffer> m_freeList;
	};
}