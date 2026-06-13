#include "buffer_impl.hpp"
#include "device_impl.hpp"

namespace pl {
	DeviceAddress Buffer::deviceAddress() const {
		return m_deviceAddress;
	}

	byte* Buffer::hostAddress() const {
		return m_hostAddress;
	}

	Buffer::Buffer(const BufferCreateInfo& ci) {
		m_deviceAddress = Device::get().deviceMemoryAllocator().allocate(ci.size, ci.align, &m_hostAddress);
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