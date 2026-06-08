#pragma once

#include "util_types.hpp"
#include "pod_types.hpp"

namespace pl {
	class Device {
		public:
			static void idle();
			static void initialize(const DeviceCreateInfo& ci);

			~Device();

			Device(Device&&) = delete;
			Device& operator=(Device&&) = delete;
			Device(const Device&) = delete;
			Device& operator=(const Device&) = delete;

		private:
			Device();
			alignas(8) byte reserved[1000];
	};
}

