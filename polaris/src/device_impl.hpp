#pragma once

#include <polaris/util_types.hpp>
#include <polaris/pod_types.hpp>
#include <volk/volk.h>
#include <tabris/vec.hpp>

#include "descriptor_heap.hpp"
#include "device_memory_allocator.hpp"

namespace pl {
	class Device {
		public:
			void waitIdle();

			Device(const DeviceCreateInfo& ci);
			Device(Device&&);
			Device& operator=(Device&&);
			~Device();

			Device(const Device&) = delete;
			Device& operator=(const Device&) = delete;

			// "public" functions that should not be included in the public header
			VkInstance vkInstance() const;
			VkPhysicalDevice vkPhysicalDevice() const;
			VkDevice vkDevice() const;
			u32 vkQueueFamily(QueueType type) const;
			VkQueue vkQueue(QueueType type) const;
			DescriptorHeap* descriptorHeap() const;
			DeviceMemoryAllocator* deviceMemoryAllocator() const;

		private:
			u32 getQueueFamilyIndex(VkQueueFlags include, VkQueueFlags exclude);

			VkInstance m_instance = {};
			VkPhysicalDevice m_physicalDevice = {};
			VkDevice m_device = {};

			struct QueueData {
				u32 family;
				VkQueue queue;
			};

			QueueData m_queues[3];

			DescriptorHeap* m_heap = nullptr;
			DeviceMemoryAllocator* m_allocator = nullptr;
	};
}