#include "staging_allocator.hpp"
#include "vk_util.hpp"

namespace pl {
	StagingBuffer StagingAllocator::alloc(u64 size) {
		for(u64 i = 0; i < m_freeList.count(); i++) {
			if(m_freeList[i].size >= size) {
				StagingBuffer buffer = m_freeList[i];
				m_freeList.remove(i);
				return buffer;
			}
		}

		StagingBuffer ret;
		ret.size = std::max(size, 64ull * 1024ull * 1024ull);
		vkCreateBuffer(m_device, ptr(VkBufferCreateInfo{
			.size = ret.size,
			.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT
		}), nullptr, &ret.buffer);

		VkMemoryRequirements mrq;
		vkGetBufferMemoryRequirements(m_device, ret.buffer, &mrq);
		u16 memTypeIndex = getMemoryTypeIndex(m_memProps, mrq.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT);

		vkAllocateMemory(m_device, ptr(VkMemoryAllocateInfo{
			.pNext = ptr(VkMemoryAllocateFlagsInfo{.flags = VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT }),
			.allocationSize = mrq.size,
			.memoryTypeIndex = memTypeIndex
		}), nullptr, &ret.memory);
		vkBindBufferMemory(m_device, ret.buffer, ret.memory, 0);
		vkMapMemory(m_device, ret.memory, 0, VK_WHOLE_SIZE, 0, &ret.mappedPtr);

		return ret;
	}

	void StagingAllocator::free(StagingBuffer buffer) {
		buffer.writeOffset = 0;
		for(u64 i = 0; i < m_freeList.count(); i++) {
			if(m_freeList[i].size > buffer.size) {
				m_freeList.insert(i, buffer);
				return;
			}
		}
		m_freeList.push(buffer);
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
		for(const StagingBuffer& buffer : m_freeList) {
			vkDestroyBuffer(m_device, buffer.buffer, nullptr);
			vkFreeMemory(m_device, buffer.memory, nullptr);
		}
	}

	StagingAllocator& StagingAllocator::operator=(StagingAllocator&& src) {
		this->~StagingAllocator();
		new (this) StagingAllocator(std::move(src)); return *this;
	}
}