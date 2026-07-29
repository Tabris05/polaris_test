#pragma once

#include <tabris/vec.hpp>
#include <polaris/pod_types.hpp>
#include <volk/volk.h>

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
