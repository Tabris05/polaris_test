#pragma once
#include <volk/volk.h>
#include <tabris/types.hpp>
#include <mutex>
#include <unordered_map>

namespace pl {
	struct Allocation {
		VkDeviceMemory memory = {};
		VkBuffer buffer = {};
	};

	class DeviceMemoryAllocator {
		public:
			VkDeviceAddress allocate(u64 size, u64 align, byte** hostAddress = nullptr);
			void bindImageMemory(VkImage image, VkDeviceAddress address);
			void free(VkDeviceAddress address);

			void initialize();
			void finalize();

			DeviceMemoryAllocator() = default;
			~DeviceMemoryAllocator() = default;
			DeviceMemoryAllocator(DeviceMemoryAllocator&& src) = delete;
			DeviceMemoryAllocator& operator=(DeviceMemoryAllocator&& src) = delete;
			DeviceMemoryAllocator(const DeviceMemoryAllocator&) = delete;
			DeviceMemoryAllocator& operator=(const DeviceMemoryAllocator&) = delete;

		private:
			std::mutex m_lock;
			std::unordered_map<VkDeviceAddress, Allocation> m_allocations;
	};
}