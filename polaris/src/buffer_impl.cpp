#include "buffer_impl.hpp"
#include "device_impl.hpp"
#include "vk_util.hpp"

namespace pl {
	DeviceAddress Buffer::deviceAddress() const {
		return m_deviceAddress;
	}

	byte* Buffer::hostAddress() const {
		return m_hostAddress;
	}

	Buffer::Buffer(const BufferCreateInfo& ci) {
		vkCreateBuffer(Device::get().vkDevice(), &VkBufferCreateInfo{
			.size = ci.size,
			.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT |
			VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR
		}, nullptr, &m_buffer);
		m_backingMem = Device::get().deviceMemoryAllocator().alloc(m_buffer);

		m_deviceAddress = vkGetBufferDeviceAddress(Device::get().vkDevice(), &VkBufferDeviceAddressInfo{ .buffer = m_buffer });
		vkMapMemory(Device::get().vkDevice(), m_backingMem, 0, ci.size, 0, reinterpret_cast<void**>(&m_hostAddress));
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
		vkDestroyBuffer(Device::get().vkDevice(), m_buffer, nullptr);
		Device::get().deviceMemoryAllocator().free(m_backingMem);
	}
}