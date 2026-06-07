#include "device_memory_allocator.hpp"
#include "device_impl.hpp"
#include "vk_util.hpp"

namespace pl {
	DeviceMemory DeviceMemoryAllocator::alloc(VkBuffer buffer) {
		VkMemoryRequirements mrq;
		vkGetBufferMemoryRequirements(Device::get().vkDevice(), buffer, &mrq);
		u16 memTypeIndex = getMemoryTypeIndex(m_memProps, mrq.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		VkDeviceMemory mem;
		vkAllocateMemory(Device::get().vkDevice(), &VkMemoryAllocateInfo{
			.pNext = &VkMemoryAllocateFlagsInfo{ .flags = VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT },
			.allocationSize = mrq.size,
			.memoryTypeIndex = memTypeIndex
		}, nullptr, &mem);
		vkBindBufferMemory(Device::get().vkDevice(), buffer, mem, 0);

		return mem;
	}

	DeviceMemory DeviceMemoryAllocator::alloc(VkImage image) {
		VkMemoryRequirements mrq;
		vkGetImageMemoryRequirements(Device::get().vkDevice(), image, &mrq);
		u16 memTypeIndex = getMemoryTypeIndex(m_memProps, mrq.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		VkDeviceMemory mem;
		vkAllocateMemory(Device::get().vkDevice(), &VkMemoryAllocateInfo{
			.allocationSize = mrq.size,
			.memoryTypeIndex = memTypeIndex
		}, nullptr, &mem);
		vkBindImageMemory(Device::get().vkDevice(), image, mem, 0);

		return mem;
	}

	void DeviceMemoryAllocator::free(VkDeviceMemory alloc) {
		vkFreeMemory(Device::get().vkDevice(), alloc, nullptr);
	}

	void DeviceMemoryAllocator::initialize() {
		vkGetPhysicalDeviceMemoryProperties(Device::get().vkPhysicalDevice(), &m_memProps);
	}

	void DeviceMemoryAllocator::finalize() {
		// this will do something some day
	}
}