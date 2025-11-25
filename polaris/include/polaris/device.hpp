#pragma once

#include "util_types.hpp"
#include "pod_types.hpp"

namespace pl {
	class Device {
		public:
			void waitIdle();

			Device(const DeviceCreateInfo& ci);
			Device(Device&&);
			Device& operator=(Device&&);
			~Device();

			Device(const Device&) = delete;
			Device& operator=(const Device&) = delete;

		private:
			alignas(8) byte reserved[112];
	};
}

