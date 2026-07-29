#pragma once

#include "pod_types.hpp"

namespace pl {
	class AccelStruct {
		public:
			DeviceAddress deviceAddress() const;

			AccelStruct(const AccelStructCreateInfo& ci);
			AccelStruct(AccelStruct&& src);
			AccelStruct& operator=(AccelStruct&& src);
			~AccelStruct();

			AccelStruct(const AccelStruct&) = delete;
			AccelStruct& operator=(const AccelStruct&) = delete;



		private:
			alignas(8) byte reserved[88];
	};
}