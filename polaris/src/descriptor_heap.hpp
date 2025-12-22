#pragma once

#include <volk/volk.h>
#include <tabris/types.hpp>
#include <tabris/vec.hpp>
#include <polaris/pod_types.hpp>
#include <mutex>

namespace pl {
	class DescriptorHeap {
		public:
			VkPipelineLayout vkPipelineLayout() const;
			void bind(VkCommandBuffer cmd, QueueType type) const;

			u32 allocImageHandle(const VkImageViewCreateInfo* ci, VkDescriptorType type);
			u32 allocSamplerHandle(const VkSamplerCreateInfo* ci);

			void freeImageHandle(u32 handle);
			void freeSamplerHandle(u32 handle);

			DescriptorHeap(VkDevice device);
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
			VkDescriptorSetLayout m_setLayout = {};
			VkPipelineLayout m_pipeLayout = {};
			VkDescriptorPool m_pool = {};
			VkDescriptorSet m_set = {};

			std::mutex m_imageLock;
			FreeRanges m_imageFreeRanges;
			tbrs::Vec<VkImageView> m_imageViews;

			std::mutex m_samplerLock;
			FreeRanges m_samplerFreeRanges;
			tbrs::Vec<VkSampler> m_samplers;

			static constexpr u32 m_imageHandleCount = 1 << 20;
			static constexpr u32 m_samplerHandleCount = 1 << 12;
	};
}