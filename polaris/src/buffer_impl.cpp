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
		AllocationResult alloc = Device::get().deviceMemoryAllocator().allocate(ci.size, 0);
		m_deviceAddress = alloc.deviceAddress;
		m_hostAddress = alloc.hostAddress;
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
		Device::get().deviceMemoryAllocator().free(m_deviceAddress);
	}
}