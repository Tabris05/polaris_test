#pragma once

#include <polaris/pod_types.hpp>
#include <volk/volk.h>
#include "sync_impl.hpp"
#include "texture_impl.hpp"
#include "queue_impl.hpp"
#include "sync_impl.hpp"
#include "descriptor_heap.hpp"

namespace pl {

	struct PresentInfo {
		SampledHandle texture;
		Queue& queue;
		const Event waitEvent;
	};

	class Swapchain {
		public:
			const Event present(const PresentInfo& info);
			void update(u32 width, u32 height, PresentMode mode = static_cast<PresentMode>(0xff));

			Swapchain(const SwapchainCreateInfo& ci);
			Swapchain(Swapchain&& src);
			Swapchain& operator=(Swapchain&& src);
			~Swapchain();

			Swapchain(const Swapchain&) = delete;
			Swapchain& operator=(const Swapchain&) = delete;
		private:
			void createSwapchain();
			void destroySwapchain();

			VkInstance m_instance = {};
			VkDevice m_device = {};
			VkSurfaceKHR m_surface = {};
			VkSwapchainKHR m_swapchain = {};

			u32 m_width = 0;
			u32 m_height = 0;
			PresentMode m_presentMode = PresentMode::VSync;

			struct SwapchainImage {
				VkImage image;
				VkSemaphore signalSem;
				u32 descriptorHandle;
			};

			tbrs::Vec<SwapchainImage> m_swapchainImages;
			tbrs::Vec<u32> m_activeQueueFamilies;
			tbrs::Vec<std::pair<VkSemaphore, const Event>> m_submittedSems;
			tbrs::Vec<VkSemaphore> m_freeSems;

			VkPipeline m_blitPipeline = {};

			DescriptorHeap* m_heap = nullptr;
	};
}