#include "acceleration_structure_impl.hpp"
#include "device_impl.hpp"
#include "vk_util.hpp"

namespace pl {
	DeviceAddress AccelerationStructure::deviceAddress() const {
		return m_address;
	}

	AccelerationStructure::AccelerationStructure(const AccelerationStructureCreateInfo& ci) : 
		m_flags(static_cast<VkBuildAccelerationStructureFlagsKHR>(ci.flags)), m_type(VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR) {
		tbrs::Vec<u32> maxPrimitives;
		maxPrimitives.reserve(ci.geometries.count());
		m_geometryInfos.reserve(ci.geometries.count());
		m_buildRangeInfos.reserve(ci.geometries.count());
		for(const AccelerationStructureGeometryInfo& gi : ci.geometries) {
			VkAccelerationStructureGeometryKHR geometry{};
			switch(gi.type) {
				case GeometryType::Triangles:
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
				case GeometryType::AABBs:
					geometry = VkAccelerationStructureGeometryKHR{
						.geometryType = VK_GEOMETRY_TYPE_AABBS_KHR,
						.geometry{ .aabbs{
							.data = gi.aabbs.data,
							.stride = sizeof(AABB)
						} },
						.flags = static_cast<VkGeometryFlagsKHR>(gi.aabbs.flags)
					};
					break;
				case GeometryType::Instances:
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

	AccelerationStructure::AccelerationStructure(AccelerationStructure&& src) {
		memcpy(this, &src, sizeof(AccelerationStructure));
		memset(&src, 0, sizeof(AccelerationStructure));
	}

	AccelerationStructure& AccelerationStructure::operator=(AccelerationStructure&& src) {
		this->~AccelerationStructure();
		new (this) AccelerationStructure(std::move(src));

		return *this;
	}

	AccelerationStructure::~AccelerationStructure() {
		vkDestroyAccelerationStructureKHR(Device::get().vkDevice(), m_accelerationStructure, nullptr);
		Device::get().deviceMemoryAllocator().free(m_address);
	}

	VkAccelerationStructureKHR AccelerationStructure::vkAccelerationStructure() const {
		return m_accelerationStructure;
	}

	VkAccelerationStructureBuildGeometryInfoKHR AccelerationStructure::vkBuildInfo() const {
		return VkAccelerationStructureBuildGeometryInfoKHR{
			.type = m_type,
			.flags = m_flags,
			.srcAccelerationStructure = m_accelerationStructure,
			.dstAccelerationStructure = m_accelerationStructure,
			.geometryCount = static_cast<u32>(m_geometryInfos.count()),
			.pGeometries = m_geometryInfos.data()
		};
	}

	const tbrs::Vec<VkAccelerationStructureBuildRangeInfoKHR>& AccelerationStructure::vkBuildRanges() const {
		return m_buildRangeInfos;
	}

	u64 AccelerationStructure::buildScratchSize() const {
		return m_buildScratchSize;
	}

	u64 AccelerationStructure::updateScratchSize() const {
		return m_updateScratchSize;
	}
}