#include "descriptor_heap.hpp"
#include "vk_util.hpp"

namespace pl {

	void DescriptorHeap::bind(VkCommandBuffer cmd) const {
		vkCmdBindResourceHeapEXT(cmd, ptr(VkBindHeapInfoEXT{
			.heapRange{
				.address = m_deviceAddr,
				.size = m_imageHandleCount * m_imageDescriptorSize
			},
			.reservedRangeOffset = m_imageHandleCount * m_imageDescriptorSize - m_imageHeapReservedSize,
			.reservedRangeSize = m_imageHeapReservedSize,
		}));
		vkCmdBindSamplerHeapEXT(cmd, ptr(VkBindHeapInfoEXT{
			.heapRange{
				.address = m_deviceAddr + m_imageHandleCount * m_imageDescriptorSize,
				.size = m_samplerHandleCount * m_samplerDescriptorSize
			},
			.reservedRangeOffset = m_samplerHandleCount * m_samplerDescriptorSize - m_samplerHeapReservedSize,
			.reservedRangeSize = m_samplerHeapReservedSize,
		}));
	}

	u32 DescriptorHeap::allocImageHandle(const VkImageViewCreateInfo* ci, VkDescriptorType type) {
		std::scoped_lock lock{ m_imageLock };
		u32 handle = acquireHandle(m_imageFreeRanges);

		vkWriteResourceDescriptorsEXT(m_device, 1,
			ptr(VkResourceDescriptorInfoEXT{
				.type = type,
				.data{
					.pImage = ptr(VkImageDescriptorInfoEXT{
						.pView = ci,
						.layout = VK_IMAGE_LAYOUT_GENERAL
					})
				}
			}),
			ptr(VkHostAddressRangeEXT{
				.address = reinterpret_cast<void*>(m_hostAddr + handle * m_imageDescriptorSize),
				.size = m_imageDescriptorSize,
			})
		);

		return handle;
	}

	void DescriptorHeap::freeImageHandle(u32 handle) {
		std::scoped_lock lock{ m_imageLock };
		releaseHandle(m_imageFreeRanges, handle);
	}

	u32 DescriptorHeap::allocSamplerHandle(const VkSamplerCreateInfo* ci) {
		std::scoped_lock lock{ m_samplerLock };
		u32 handle = acquireHandle(m_samplerFreeRanges);

		vkWriteSamplerDescriptorsEXT(m_device, 1, ci, ptr(VkHostAddressRangeEXT{
			.address = reinterpret_cast<void*>(m_hostAddr + m_imageHandleCount * m_imageDescriptorSize + handle * m_samplerDescriptorSize),
			.size = m_samplerDescriptorSize,
		}));

		return handle;
	}

	void DescriptorHeap::freeSamplerHandle(u32 handle) {
		std::scoped_lock lock{ m_samplerLock };
		releaseHandle(m_samplerFreeRanges, handle);
	}

	DescriptorHeap::DescriptorHeap(VkPhysicalDevice physicalDevice, VkDevice device, DeviceMemoryAllocator* allocator) : m_device(device) {
		m_imageFreeRanges.push(0, m_imageHandleCount - 1);
		m_samplerFreeRanges.push(0, m_samplerHandleCount - 1);

		VkPhysicalDeviceDescriptorHeapPropertiesEXT props{};
		vkGetPhysicalDeviceProperties2(physicalDevice, ptr(VkPhysicalDeviceProperties2{ .pNext = &props }));
		m_imageHeapReservedSize = props.minResourceHeapReservedRange;
		m_samplerHeapReservedSize = props.minSamplerHeapReservedRange;
		m_imageDescriptorSize = props.imageDescriptorSize;
		m_samplerDescriptorSize = props.samplerDescriptorSize;

		vkCreateBuffer(m_device, ptr(VkBufferCreateInfo{
			.size = m_imageHandleCount * m_imageDescriptorSize + m_samplerHandleCount * m_samplerDescriptorSize,
			.usage = VK_BUFFER_USAGE_DESCRIPTOR_HEAP_BIT_EXT | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
		}), nullptr, &m_buffer);
		m_backingMem = allocator->alloc(m_buffer);
		m_deviceAddr = vkGetBufferDeviceAddress(m_device, ptr(VkBufferDeviceAddressInfo{ .buffer = m_buffer }));
		vkMapMemory(m_device, m_backingMem, 0, VK_WHOLE_SIZE, 0, reinterpret_cast<void**>(&m_hostAddr));
	}

	DescriptorHeap::~DescriptorHeap() {
		if(m_device) {
			vkFreeMemory(m_device, m_backingMem, nullptr);
		}
	}


	DescriptorHeap::DescriptorHeap(DescriptorHeap&& src) {
		memcpy(this, &src, sizeof(DescriptorHeap));
		memset(&src, 0, sizeof(DescriptorHeap));
	}

	DescriptorHeap& DescriptorHeap::operator=(DescriptorHeap&& src) {
		this->~DescriptorHeap();
		new (this) DescriptorHeap(std::move(src)); return *this;
	};

	u32 DescriptorHeap::acquireHandle(FreeRanges& ranges) {
		u32 result = ranges.front().first;
		ranges.front().first++;

		if(ranges.front().first > ranges.front().second) {
			// if we store the ranges backwards this could be pop back
			ranges.remove(0);
		}

		return result;
	}

	void DescriptorHeap::releaseHandle(FreeRanges& ranges, u32 handle) {
		for(size_t i = 0; i < ranges.size(); i++) {
			if(ranges[i].second == handle - 1) {
				ranges[i].second++;
				if(i < ranges.size() - 1 && ranges[i].second + 1 == ranges[i + 1].first) {
					ranges[i].second = ranges[i + 1].second;
					ranges.remove(i + 1);
				}
				return;
			}
			else if(ranges[i].first == handle + 1) {
				ranges[i].first--;
				return;
			}
			else if(ranges[i].first > handle) {
				ranges.insert(i, handle, handle);
				return;
			}
		}

		ranges.push(handle, handle);
	}
}