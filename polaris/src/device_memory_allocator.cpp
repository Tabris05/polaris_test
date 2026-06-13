#include "device_memory_allocator.hpp"
#include "device_impl.hpp"
#include "vk_util.hpp"

namespace pl {
	VkDeviceAddress DeviceMemoryAllocator::allocate(u64 size, u64 align, byte** hostAddress) {
		Allocation alloc;

		vkCreateBuffer(Device::get().vkDevice(), &VkBufferCreateInfo{
			.size = size,
			.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT |
					 VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR
		}, nullptr, &alloc.buffer);

		BufferBindResult res = Device::get().bindBufferMemory(alloc.buffer);
		alloc.memory = res.memory;

		{
			std::scoped_lock scope(m_lock);
			m_allocations[res.deviceAddress] = alloc;
		}

		if(hostAddress) {
			*hostAddress = res.hostAddress;
		}

		return res.deviceAddress;
	}

	void DeviceMemoryAllocator::bindImageMemory(VkImage image, VkDeviceAddress address) {
		std::scoped_lock scope(m_lock);
		vkBindImageMemory(Device::get().vkDevice(), image, m_allocations[address].memory, 0);
	}

	void DeviceMemoryAllocator::free(VkDeviceAddress address) {
		if(address) {
			Allocation alloc;

			{
				std::scoped_lock scope(m_lock);
				auto it = m_allocations.find(address);
				alloc = it->second;
				m_allocations.erase(it);
			}

			vkDestroyBuffer(Device::get().vkDevice(), alloc.buffer, nullptr);
			vkFreeMemory(Device::get().vkDevice(), alloc.memory, nullptr);
		}
	}

	void DeviceMemoryAllocator::initialize() {
		// this might do something some day
	}

	void DeviceMemoryAllocator::finalize() {
		// this will do something some day
	}
}