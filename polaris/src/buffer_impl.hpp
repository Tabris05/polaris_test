#pragma once

#include <polaris/pod_types.hpp>
#include <volk/volk.h>

#include "device_memory_allocator.hpp"

namespace pl {
	class Buffer {
		public:
			Buffer(const BufferCreateInfo& ci);
			Buffer(Buffer&& src);
			Buffer& operator=(Buffer&& src);
			~Buffer();

			Buffer(const Buffer&) = delete;
			Buffer& operator=(const Buffer&) = delete;

		private:
			void* deviceAddressImpl() const;

			VkDevice m_device = {};
			VkBuffer m_buffer = {};
			VkDeviceAddress m_bufferDeviceAddress = {};

			DeviceMemoryAllocator* m_allocator = nullptr;

			Allocation m_backingMem = {};
	};
}
