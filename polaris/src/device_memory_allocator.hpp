#pragma once
#include <volk/volk.h>
#include <tabris/types.hpp>
#include <tabris/vec.hpp>
#include <mutex>

namespace pl {
	using DeviceMemory = u16;

	class DeviceMemoryAllocator {
		public:
			DeviceMemory alloc(VkBuffer buffer);
			DeviceMemory alloc(VkImage image);
			void free(DeviceMemory alloc);

			DeviceMemoryAllocator(VkPhysicalDevice physicalDevice, VkDevice device);
			DeviceMemoryAllocator(DeviceMemoryAllocator&& src);
			DeviceMemoryAllocator& operator=(DeviceMemoryAllocator&& src);

			DeviceMemoryAllocator(const DeviceMemoryAllocator&) = delete;
			DeviceMemoryAllocator& operator=(const DeviceMemoryAllocator&) = delete;

		private:
			VkDevice m_device = {};
			VkPhysicalDeviceMemoryProperties m_memProps = {};

			std::mutex m_lock;
			tbrs::Vec<VkDeviceMemory> m_allocations;
	};
}