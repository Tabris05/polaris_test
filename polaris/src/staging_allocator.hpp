#pragma once

#include <volk/volk.h>
#include <tabris/types.hpp>
#include <tabris/vec.hpp>

namespace pl {
	struct StagingBuffer {
		VkBuffer buffer = {};
		VkDeviceMemory memory = {};
		void* mappedPtr = nullptr;
		u64 writeOffset = 0;
	};

	class StagingAllocator {
	public:
		StagingBuffer alloc();
		void free(StagingBuffer buffer);

		StagingAllocator(VkPhysicalDevice physicalDevice, VkDevice device);
		StagingAllocator(StagingAllocator&& src);
		StagingAllocator& operator=(StagingAllocator&& src);
		~StagingAllocator();

		StagingAllocator(const StagingAllocator&) = delete;
		StagingAllocator& operator=(const StagingAllocator&) = delete;

		static constexpr u64 PageSize = 64 * 1024 * 1024;
	private:
		VkDevice m_device = {};
		VkPhysicalDeviceMemoryProperties m_memProps = {};

		tbrs::Vec<StagingBuffer> m_freePages;

	};
}