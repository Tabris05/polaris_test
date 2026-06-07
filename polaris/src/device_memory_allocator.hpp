#pragma once
#include <volk/volk.h>
#include <tabris/types.hpp>
#include <tabris/vec.hpp>
#include <mutex>

namespace pl {
	using DeviceMemory = VkDeviceMemory;

	class DeviceMemoryAllocator {
		public:
			DeviceMemory alloc(VkBuffer buffer);
			DeviceMemory alloc(VkImage image);
			void free(DeviceMemory alloc);

			void initialize();
			void finalize();

			DeviceMemoryAllocator() = default;
			~DeviceMemoryAllocator() = default;
			DeviceMemoryAllocator(DeviceMemoryAllocator&& src) = delete;
			DeviceMemoryAllocator& operator=(DeviceMemoryAllocator&& src) = delete;
			DeviceMemoryAllocator(const DeviceMemoryAllocator&) = delete;
			DeviceMemoryAllocator& operator=(const DeviceMemoryAllocator&) = delete;

		private:
			VkPhysicalDeviceMemoryProperties m_memProps = {};
	};
}