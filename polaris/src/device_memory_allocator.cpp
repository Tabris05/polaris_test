#include "device_memory_allocator.hpp"
#include "vk_util.hpp"

namespace pl {
	Allocation DeviceMemoryAllocator::alloc(VkMemoryRequirements memReqs, VkMemoryPropertyFlags properties) {
		u32 memTypeIndex = getMemoryTypeIndex(properties, memReqs.memoryTypeBits);

		VkDeviceMemory mem;
		vkAllocateMemory(m_device, ptr(VkMemoryAllocateInfo{
			.allocationSize = memReqs.size,
			.memoryTypeIndex = memTypeIndex
		}), nullptr, &mem);

		return Allocation{ mem };
	}
	void DeviceMemoryAllocator::free(Allocation alloc) {
		vkFreeMemory(m_device, alloc.mem, nullptr);
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

	u32 DeviceMemoryAllocator::getMemoryTypeIndex(VkMemoryPropertyFlags flags, u32 mask) {
		for(u32 idx = 0; idx < m_memProps.memoryTypeCount; idx++) {
			if(((1 << idx) & mask) && (m_memProps.memoryTypes[idx].propertyFlags & flags) == flags) {
				return idx;
			}
		}
		return ~0;
	}
}