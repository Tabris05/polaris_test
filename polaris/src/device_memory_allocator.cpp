#include "device_memory_allocator.hpp"
#include "vk_util.hpp"

namespace pl {
	DeviceMemory DeviceMemoryAllocator::alloc(VkBuffer buffer) {
		VkMemoryRequirements mrq;
		vkGetBufferMemoryRequirements(m_device, buffer, &mrq);
		u16 memTypeIndex = getMemoryTypeIndex(m_memProps, mrq.memoryTypeBits);

		VkDeviceMemory mem;
		vkAllocateMemory(m_device, ptr(VkMemoryAllocateInfo{
			.pNext = ptr(VkMemoryAllocateFlagsInfo{.flags = VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT }),
			.allocationSize = mrq.size,
			.memoryTypeIndex = memTypeIndex
		}), nullptr, &mem);
		vkBindBufferMemory(m_device, buffer, mem, 0);

		return mem;
	}

	DeviceMemory DeviceMemoryAllocator::alloc(VkImage image) {
		VkMemoryRequirements mrq;
		vkGetImageMemoryRequirements(m_device, image, &mrq);
		u16 memTypeIndex = getMemoryTypeIndex(m_memProps, mrq.memoryTypeBits);

		VkDeviceMemory mem;
		vkAllocateMemory(m_device, ptr(VkMemoryAllocateInfo{
			.pNext = ptr(VkMemoryAllocateFlagsInfo{.flags = VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT }),
			.allocationSize = mrq.size,
			.memoryTypeIndex = memTypeIndex
		}), nullptr, &mem);
		vkBindImageMemory(m_device, image, mem, 0);

		return mem;
	}

	void DeviceMemoryAllocator::free(VkDeviceMemory alloc) {
		vkFreeMemory(m_device, alloc, nullptr);
	}

	DeviceMemoryAllocator::DeviceMemoryAllocator(VkPhysicalDevice physicalDevice, VkDevice device) :
		m_device(device) {
		vkGetPhysicalDeviceMemoryProperties(physicalDevice, &m_memProps);
	}

	DeviceMemoryAllocator::DeviceMemoryAllocator(DeviceMemoryAllocator&& src) {
		memcpy(this, &src, sizeof(DeviceMemoryAllocator));
		memset(&src, 0, sizeof(DeviceMemoryAllocator));
	}

	DeviceMemoryAllocator& DeviceMemoryAllocator::operator=(DeviceMemoryAllocator&& src) {
		this->~DeviceMemoryAllocator();
		new (this) DeviceMemoryAllocator(std::move(src)); return *this;
	}
}