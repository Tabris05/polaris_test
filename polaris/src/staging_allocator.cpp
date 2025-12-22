#include "staging_allocator.hpp"
#include "vk_util.hpp"

namespace pl {
	StagingBuffer StagingAllocator::alloc() {
		if(m_freePages.count() > 0) {
			StagingBuffer buffer = m_freePages.back();
			m_freePages.pop();
			return buffer;
		}

		StagingBuffer ret;
		vkCreateBuffer(m_device, ptr(VkBufferCreateInfo{
			.size = PageSize,
			.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT
		}), nullptr, &ret.buffer);

		VkMemoryRequirements mrq;
		vkGetBufferMemoryRequirements(m_device, ret.buffer, &mrq);
		u16 memTypeIndex = getMemoryTypeIndex(m_memProps, mrq.memoryTypeBits, true);

		vkAllocateMemory(m_device, ptr(VkMemoryAllocateInfo{
			.pNext = ptr(VkMemoryAllocateFlagsInfo{.flags = VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT }),
			.allocationSize = mrq.size,
			.memoryTypeIndex = memTypeIndex
		}), nullptr, &ret.memory);
		vkBindBufferMemory(m_device, ret.buffer, ret.memory, 0);
		vkMapMemory(m_device, ret.memory, 0, mrq.size, 0, &ret.mappedPtr);

		return ret;
	}

	void StagingAllocator::free(StagingBuffer buffer) {
		buffer.writeOffset = 0;
		m_freePages.push(buffer);
	}

	StagingAllocator::StagingAllocator(VkPhysicalDevice physicalDevice, VkDevice device) :
		m_device(device) {
		vkGetPhysicalDeviceMemoryProperties(physicalDevice, &m_memProps);
	}

	StagingAllocator::StagingAllocator(StagingAllocator&& src) {
		memcpy(this, &src, sizeof(StagingAllocator));
		memset(&src, 0, sizeof(StagingAllocator));
	}

	StagingAllocator::~StagingAllocator() {
		for(u64 i = 0; i < m_freePages.count(); i++) {
			vkUnmapMemory(m_device, m_freePages[i].memory);
			vkDestroyBuffer(m_device, m_freePages[i].buffer, nullptr);
			vkFreeMemory(m_device, m_freePages[i].memory, nullptr);
		}
	}

	StagingAllocator& StagingAllocator::operator=(StagingAllocator&& src) {
		this->~StagingAllocator();
		new (this) StagingAllocator(std::move(src)); return *this;
	}
}