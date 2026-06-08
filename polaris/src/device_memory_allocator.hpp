#pragma once
#include <tabris/types.hpp>
#include <volk/volk.h>
#include <mutex>
#include <unordered_map>

namespace pl {
	struct AllocationResult {
		VkDeviceAddress deviceAddress;
		byte* hostAddress;
	};

	class DeviceMemoryAllocator {
		public:
			AllocationResult allocate(u64 size, u64 align);
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
			struct Allocation {
				VkDeviceMemory memory;
				VkBuffer buffer;
			};

			std::mutex m_lock;
			std::unordered_map<VkDeviceAddress, Allocation> m_allocations;
	};
}