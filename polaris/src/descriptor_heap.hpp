#pragma once

#include <volk/volk.h>
#include <tabris/types.hpp>
#include <tabris/vec.hpp>
#include <polaris/pod_types.hpp>
#include <mutex>
#include "device_memory_allocator.hpp"

namespace pl {
	class DescriptorHeap {
		public:
			void bind(VkCommandBuffer cmd) const;

			u32 allocImageHandle(const VkImageViewCreateInfo* ci, VkDescriptorType type);
			u32 allocSamplerHandle(const VkSamplerCreateInfo* ci);

			void freeImageHandle(u32 handle);
			void freeSamplerHandle(u32 handle);

			DescriptorHeap(VkPhysicalDevice physicalDevice, VkDevice device, DeviceMemoryAllocator* allocator);
			~DescriptorHeap();

			DescriptorHeap(const DescriptorHeap&) = delete;
			DescriptorHeap& operator=(const DescriptorHeap&) = delete;
			DescriptorHeap(DescriptorHeap&& src);
			DescriptorHeap& operator=(DescriptorHeap&& src);

		private:
			using FreeRanges = tbrs::Vec<std::pair<u32, u32>>;
			u32 acquireHandle(FreeRanges& ranges);
			void releaseHandle(FreeRanges& ranges, u32 handle);

			VkDevice m_device = {};
			VkDeviceMemory m_backingMem = {};
			VkBuffer m_buffer = {};
			VkDeviceAddress m_deviceAddr = {};
			byte* m_hostAddr = nullptr;

			u32 m_imageHeapReservedSize = 0;
			u16 m_samplerHeapReservedSize = 0;
			u8 m_imageDescriptorSize = 0;
			u8 m_samplerDescriptorSize = 0;

			std::mutex m_imageLock;
			FreeRanges m_imageFreeRanges;

			std::mutex m_samplerLock;
			FreeRanges m_samplerFreeRanges;

			static constexpr u32 m_imageHandleCount = 1 << 20;
			static constexpr u32 m_samplerHandleCount = 1 << 12;
	};
}