#pragma once
#include <volk/volk.h>
#include <tabris/types.hpp>
#include <tabris/vec.hpp>
#include <mutex>

namespace pl {
	struct Allocation {
		VkDeviceMemory mem = {};
	};

	class DeviceMemoryAllocator {
		public:
			Allocation alloc(VkMemoryRequirements memReqs, VkMemoryPropertyFlags properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
			void free(Allocation alloc);

			DeviceMemoryAllocator(VkPhysicalDevice physicalDevice, VkDevice device);
			DeviceMemoryAllocator(DeviceMemoryAllocator&& src);
			DeviceMemoryAllocator& operator=(DeviceMemoryAllocator&& src);

			DeviceMemoryAllocator(const DeviceMemoryAllocator&) = delete;
			DeviceMemoryAllocator& operator=(const DeviceMemoryAllocator&) = delete;
		private:
			u32 getMemoryTypeIndex(VkMemoryPropertyFlags flags, u32 mask);

			VkDevice m_device = {};
			VkPhysicalDeviceMemoryProperties m_memProps = {};

			std::mutex m_lock;
			tbrs::Vec<VkDeviceMemory> m_allocations;
	};
}