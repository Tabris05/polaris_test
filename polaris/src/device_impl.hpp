#pragma once

#include <polaris/util_types.hpp>
#include <polaris/pod_types.hpp>

#include "descriptor_heap.hpp"
#include "device_memory_allocator.hpp"

namespace pl {
	struct BufferBindResult {
		VkDeviceMemory memory;
		VkDeviceAddress deviceAddress;
		byte* hostAddress;
	};

	class Device {
		public:
			static void idle();
			static void initialize(const DeviceCreateInfo& ci);

			~Device();

			Device(Device&&) = delete;
			Device& operator=(Device&&) = delete;
			Device(const Device&) = delete;
			Device& operator=(const Device&) = delete;

			// "public" functions that should not be included in the public header
			static Device& get();
			void initializeImpl(const DeviceCreateInfo& ci);
			VkInstance vkInstance() const;
			VkPhysicalDevice vkPhysicalDevice() const;
			VkDevice vkDevice() const;
			u32 vkQueueFamily(QueueType type) const;
			VkQueue vkQueue(QueueType type) const;
			DescriptorHeap& descriptorHeap();
			DeviceMemoryAllocator& deviceMemoryAllocator();
			BufferBindResult bindBufferMemory(VkBuffer buffer, bool deviceLocal = true);

		private:
			Device() = default;
			u32 getQueueFamilyIndex(VkQueueFlags include, VkQueueFlags exclude = 0);

			VkInstance m_instance = {};
			VkPhysicalDevice m_physicalDevice = {};
			VkDevice m_device = {};

			struct QueueData {
				u32 family;
				VkQueue queue;
			};

			QueueData m_queues[3];

			DescriptorHeap m_heap;
			DeviceMemoryAllocator m_allocator;
			VkPhysicalDeviceMemoryProperties m_memoryProperties;
	};
}