#pragma once

#include "pod_types.hpp"

namespace pl {
	class AccelerationStructure {
		public:
			DeviceAddress deviceAddress() const;

			AccelerationStructure(const AccelerationStructureCreateInfo& ci);
			AccelerationStructure(AccelerationStructure&& src);
			AccelerationStructure& operator=(AccelerationStructure&& src);
			~AccelerationStructure();

			AccelerationStructure(const AccelerationStructure&) = delete;
			AccelerationStructure& operator=(const AccelerationStructure&) = delete;



		private:
			alignas(8) byte reserved[88];
	};
}