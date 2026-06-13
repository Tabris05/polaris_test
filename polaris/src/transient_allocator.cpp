#include "transient_allocator.hpp"
#include "device_impl.hpp"
#include "vk_util.hpp"

namespace pl {
	TransientBuffer TransientAllocator::allocate(u64 size, b8 deviceLocal) {
		u64 candidateIndex = -1;

		// stupid linear scan for now, this list is expected to stay small
		for(u64 i = 0; i < m_freeList.count(); i++) {
			if(m_freeList[i].size >= size && m_freeList[i].deviceLocal == deviceLocal) {
				if(candidateIndex == -1) {
					candidateIndex = i;

				}
				else if(m_freeList[i].size < m_freeList[candidateIndex].size) {
					candidateIndex = i;
				}
			}
		}

		if(candidateIndex != -1) {
			TransientBuffer ret = m_freeList[candidateIndex];
			m_freeList.remove(candidateIndex);
			return ret;
		}

		TransientBuffer ret;
		ret.size = std::max(size, deviceLocal ? m_scratchBufferSize : m_stagingBufferSize);

		Allocation alloc;
		vkCreateBuffer(Device::get().vkDevice(), &VkBufferCreateInfo{
			.size = ret.size,
			.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | (deviceLocal ? VK_BUFFER_USAGE_STORAGE_BUFFER_BIT : 0u)
		}, nullptr, &alloc.buffer);

		BufferBindResult res = Device::get().bindBufferMemory(alloc.buffer, deviceLocal);
		alloc.memory = res.memory;
		m_allocations.push(alloc);

		ret.deviceAddress = res.deviceAddress;
		ret.hostAddress = res.hostAddress;
		return ret;
	}

	void TransientAllocator::free(TransientBuffer buffer) {
		buffer.writeOffset = 0;
		m_freeList.push(buffer);
	}

	TransientAllocator::~TransientAllocator() {
		for(const Allocation& alloc : m_allocations) {
			vkDestroyBuffer(Device::get().vkDevice(), alloc.buffer, nullptr);
			vkFreeMemory(Device::get().vkDevice(), alloc.memory, nullptr);
		}
	}
}