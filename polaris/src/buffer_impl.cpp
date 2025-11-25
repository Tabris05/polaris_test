#include "buffer_impl.hpp"
#include "device_impl.hpp"
#include "vk_util.hpp"

namespace pl {
	Buffer::Buffer(const BufferCreateInfo& ci)
		: m_device(ci.device.vkDevice()), m_allocator(ci.device.deviceMemoryAllocator()) {
		vkCreateBuffer(m_device, ptr(VkBufferCreateInfo{
			.size = ci.size,
			.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT| VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
			.sharingMode = ci.device.vkActiveQueueFamilies().size() > 1 ? VK_SHARING_MODE_CONCURRENT : VK_SHARING_MODE_EXCLUSIVE,
			.queueFamilyIndexCount = static_cast<u32>(ci.device.vkActiveQueueFamilies().size()),
			.pQueueFamilyIndices = ci.device.vkActiveQueueFamilies().data()
		}), nullptr, &m_buffer);

		VkMemoryRequirements mrq;
		vkGetBufferMemoryRequirements(m_device, m_buffer, &mrq);
		m_backingMem = m_allocator->alloc(mrq);
		vkBindBufferMemory(m_device, m_buffer, m_backingMem.mem, 0);

		m_bufferDeviceAddress = vkGetBufferDeviceAddress(m_device, ptr(VkBufferDeviceAddressInfo{ .buffer = m_buffer }));
	}

	Buffer::Buffer(Buffer&& src) {
		memcpy(this, &src, sizeof(Buffer));
		memset(&src, 0, sizeof(Buffer));
	}

	Buffer& Buffer::operator=(Buffer&& src) {
		this->~Buffer();
		new (this) Buffer(std::move(src));
		return *this;
	}

	Buffer::~Buffer() {
		if(m_device) {
			vkDestroyBuffer(m_device, m_buffer, nullptr);
			m_allocator->free(m_backingMem);
		}
	}

	void* Buffer::deviceAddressImpl() const {
		return reinterpret_cast<void*>(m_bufferDeviceAddress);
	}
}