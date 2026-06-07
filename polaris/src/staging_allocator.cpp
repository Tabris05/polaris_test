#include "staging_allocator.hpp"
#include "device_impl.hpp"
#include "vk_util.hpp"

namespace pl {
	StagingBuffer StagingAllocator::alloc(u64 size) {
		u64 candidateIndex = -1;
		for(u64 i = 0; i < m_freeList.count(); i++) {
			if(m_freeList[i].size >= size) {
				if(candidateIndex == -1) {
					candidateIndex = i;

				}
				else if(m_freeList[i].size < m_freeList[candidateIndex].size) {
					candidateIndex = i;
				}
			}
		}

		if(candidateIndex != -1) {
			StagingBuffer ret = m_freeList[candidateIndex];
			m_freeList.remove(candidateIndex);
			return ret;
		}

		StagingBuffer ret;
		ret.size = std::max(size, 64ull * 1024ull * 1024ull);
		vkCreateBuffer(Device::get().vkDevice(), &VkBufferCreateInfo{
			.size = ret.size,
			.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
		}, nullptr, &ret.buffer);

		VkMemoryRequirements mrq;
		vkGetBufferMemoryRequirements(Device::get().vkDevice(), ret.buffer, &mrq);
		u16 memTypeIndex = getMemoryTypeIndex(m_memProps, mrq.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT);

		vkAllocateMemory(Device::get().vkDevice(), &VkMemoryAllocateInfo{
			.pNext = &VkMemoryAllocateFlagsInfo{.flags = VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT },
			.allocationSize = mrq.size,
			.memoryTypeIndex = memTypeIndex
		}, nullptr, &ret.memory);
		vkBindBufferMemory(Device::get().vkDevice(), ret.buffer, ret.memory, 0);
		vkMapMemory(Device::get().vkDevice(), ret.memory, 0, VK_WHOLE_SIZE, 0, &ret.cpuPtr);
		ret.gpuPtr = vkGetBufferDeviceAddress(Device::get().vkDevice(), &VkBufferDeviceAddressInfo{ .buffer = ret.buffer });

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

	StagingAllocator::StagingAllocator() {
		vkGetPhysicalDeviceMemoryProperties(Device::get().vkPhysicalDevice(), &m_memProps);
	}

	StagingAllocator::StagingAllocator(StagingAllocator&& src) {
		memcpy(this, &src, sizeof(StagingAllocator));
		memset(&src, 0, sizeof(StagingAllocator));
	}

	StagingAllocator::~StagingAllocator() {
		for(const StagingBuffer& buffer : m_freeList) {
			vkDestroyBuffer(Device::get().vkDevice(), buffer.buffer, nullptr);
			vkFreeMemory(Device::get().vkDevice(), buffer.memory, nullptr);
		}
	}

	StagingAllocator& StagingAllocator::operator=(StagingAllocator&& src) {
		this->~StagingAllocator();
		new (this) StagingAllocator(std::move(src)); return *this;
	}
}