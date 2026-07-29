#include "accel_struct_impl.hpp"
#include "device_impl.hpp"
#include "vk_util.hpp"

namespace pl {
	DeviceAddress AccelStruct::deviceAddress() const {
		return m_address;
	}

	AccelStruct::AccelStruct(const AccelStructCreateInfo& ci) :
		m_flags(static_cast<VkBuildAccelerationStructureFlagsKHR>(ci.flags)), m_type(VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR) {
		tbrs::Vec<u32> maxPrimitives;
		maxPrimitives.reserve(ci.geometries.count());
		m_geometryInfos.reserve(ci.geometries.count());
		m_buildRangeInfos.reserve(ci.geometries.count());
		for(const ASGeometryInfo& gi : ci.geometries) {
			VkAccelerationStructureGeometryKHR geometry{};
			switch(gi.type) {
				case ASGeometryType::Triangles:
					geometry = VkAccelerationStructureGeometryKHR{
						.geometryType = VK_GEOMETRY_TYPE_TRIANGLES_KHR,
						.geometry{ .triangles{
							.vertexFormat = VK_FORMAT_R32G32B32_SFLOAT,
							.vertexData = gi.triangles.vertexData,
							.vertexStride = gi.triangles.vertexStride,
							.maxVertex = gi.triangles.maxVertex,
							.indexType = gi.triangles.indexWidth == 0 ? VK_INDEX_TYPE_NONE_KHR : (gi.triangles.indexWidth == 2 ? VK_INDEX_TYPE_UINT16 : VK_INDEX_TYPE_UINT32),
							.indexData = gi.triangles.indexData,
							.transformData = gi.triangles.transform
						} },
						.flags = static_cast<VkGeometryFlagsKHR>(gi.triangles.flags)
					};
					break;
				case ASGeometryType::AABBs:
					geometry = VkAccelerationStructureGeometryKHR{
						.geometryType = VK_GEOMETRY_TYPE_AABBS_KHR,
						.geometry{ .aabbs{
							.data = gi.aabbs.data,
							.stride = sizeof(AABB)
						} },
						.flags = static_cast<VkGeometryFlagsKHR>(gi.aabbs.flags)
					};
					break;
				case ASGeometryType::Instances:
					m_type = VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR;
					geometry = VkAccelerationStructureGeometryKHR{
						.geometryType = VK_GEOMETRY_TYPE_INSTANCES_KHR,
						.geometry{ .instances{ .data = gi.instances.data, } },
					};
					break;
			}

			m_geometryInfos.push(geometry);
			maxPrimitives.push(gi.primitiveCount);
			m_buildRangeInfos.push(gi.primitiveCount);
		}

		VkAccelerationStructureBuildSizesInfoKHR sizeInfo{};
		vkGetAccelerationStructureBuildSizesKHR(Device::get().vkDevice(), VK_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR, &vkBuildInfo(), maxPrimitives.data(), &sizeInfo);
		m_buildScratchSize = sizeInfo.buildScratchSize;
		m_updateScratchSize = sizeInfo.updateScratchSize;

		m_address = Device::get().deviceMemoryAllocator().allocate(sizeInfo.accelerationStructureSize, 256);
		vkCreateAccelerationStructure2KHR(Device::get().vkDevice(), &VkAccelerationStructureCreateInfo2KHR{
			.addressRange{ m_address, sizeInfo.accelerationStructureSize },
			.addressFlags = VK_ADDRESS_COMMAND_FULLY_BOUND_BIT_KHR,
			.type = m_type
		}, nullptr, &m_accelerationStructure);
	}

	AccelStruct::AccelStruct(AccelStruct&& src) {
		memcpy(this, &src, sizeof(AccelStruct));
		memset(&src, 0, sizeof(AccelStruct));
	}

	AccelStruct& AccelStruct::operator=(AccelStruct&& src) {
		this->~AccelStruct();
		new (this) AccelStruct(std::move(src));

		return *this;
	}

	AccelStruct::~AccelStruct() {
		vkDestroyAccelerationStructureKHR(Device::get().vkDevice(), m_accelerationStructure, nullptr);
		Device::get().deviceMemoryAllocator().free(m_address);
	}

	VkAccelerationStructureKHR AccelStruct::vkAccelerationStructure() const {
		return m_accelerationStructure;
	}

	VkAccelerationStructureBuildGeometryInfoKHR AccelStruct::vkBuildInfo() const {
		return VkAccelerationStructureBuildGeometryInfoKHR{
			.type = m_type,
			.flags = m_flags,
			.srcAccelerationStructure = m_accelerationStructure,
			.dstAccelerationStructure = m_accelerationStructure,
			.geometryCount = static_cast<u32>(m_geometryInfos.count()),
			.pGeometries = m_geometryInfos.data()
		};
	}

	const tbrs::Vec<VkAccelerationStructureBuildRangeInfoKHR>& AccelStruct::vkBuildRanges() const {
		return m_buildRangeInfos;
	}

	u64 AccelStruct::buildScratchSize() const {
		return m_buildScratchSize;
	}

	u64 AccelStruct::updateScratchSize() const {
		return m_updateScratchSize;
	}
}