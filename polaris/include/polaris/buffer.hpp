#pragma once

#include "pod_types.hpp"

namespace pl {
	class Buffer {
		public:
			template<typename T>
			T* deviceAddress() const {
				return reinterpret_cast<T*>(deviceAddress());
			}
			DeviceAddress deviceAddress() const;

			Buffer(const BufferCreateInfo& ci);
			Buffer(Buffer&& src);
			Buffer& operator=(Buffer&& src);
			~Buffer();

			Buffer(const Buffer&) = delete;
			Buffer& operator=(const Buffer&) = delete;

		private:
			alignas(8) byte reserved[40];
	};
}
