#pragma once

#include <polaris/pod_types.hpp>
#include <volk/volk.h>

#include "device_memory_allocator.hpp"

namespace pl {
	class Buffer {
		public:
			DeviceAddress deviceAddress() const;
			byte* hostAddress() const;

			Buffer(const BufferCreateInfo& ci);
			Buffer(Buffer&& src);
			Buffer& operator=(Buffer&& src);
			~Buffer();

			Buffer(const Buffer&) = delete;
			Buffer& operator=(const Buffer&) = delete;

		private:
			VkBuffer m_buffer = {};
			VkDeviceAddress m_deviceAddress = {};
			byte* m_hostAddress = nullptr;
			DeviceMemory m_backingMem = {};
	};
}
