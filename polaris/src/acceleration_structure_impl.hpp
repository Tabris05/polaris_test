#pragma once

#include <tabris/vec.hpp>
#include <polaris/pod_types.hpp>
#include <volk/volk.h>

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

			// "public" functions that should not be included in the public header
			VkAccelerationStructureKHR vkAccelerationStructure() const;
			VkAccelerationStructureBuildGeometryInfoKHR vkBuildInfo() const;
			const tbrs::Vec<VkAccelerationStructureBuildRangeInfoKHR>& vkBuildRanges() const;
			u64 buildScratchSize() const; 
			u64 updateScratchSize() const; 

		private:
			VkDeviceAddress m_address;
			VkAccelerationStructureKHR m_accelerationStructure;
			tbrs::Vec<VkAccelerationStructureGeometryKHR> m_geometryInfos;
			tbrs::Vec<VkAccelerationStructureBuildRangeInfoKHR> m_buildRangeInfos;
			VkBuildAccelerationStructureFlagsKHR m_flags;
			VkAccelerationStructureTypeKHR m_type;
			u64 m_buildScratchSize;
			u64 m_updateScratchSize;
	};
}
