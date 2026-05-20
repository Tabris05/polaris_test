#pragma once

#include <polaris/pod_types.hpp>
#include <volk/volk.h>

#include "device_memory_allocator.hpp"

namespace pl {
	class Buffer {
		public:
			DeviceAddress deviceAddress() const;

			Buffer(const BufferCreateInfo& ci);
			Buffer(Buffer&& src);
			Buffer& operator=(Buffer&& src);
			~Buffer();

			Buffer(const Buffer&) = delete;
			Buffer& operator=(const Buffer&) = delete;

			// "public" functions that should not be included in the public header
			VkBuffer vkBuffer() const;

		private:
			VkDevice m_device = {};
			VkBuffer m_buffer = {};
			VkDeviceAddress m_deviceAddress = {};

			DeviceMemoryAllocator* m_allocator = nullptr;

			DeviceMemory m_backingMem = {};
	};
}
