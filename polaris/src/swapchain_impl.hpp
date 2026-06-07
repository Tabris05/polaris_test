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
		TextureHandle texture;
		Queue& queue;
		std::optional<const Event> waitEvent;
	};

	class Swapchain {
		public:
			const Event present(const PresentInfo& info);
			void update(u32 width, u32 height, b8 vsync);

			Swapchain(const SwapchainCreateInfo& ci);
			Swapchain(Swapchain&& src);
			Swapchain& operator=(Swapchain&& src);
			~Swapchain();

			Swapchain(const Swapchain&) = delete;
			Swapchain& operator=(const Swapchain&) = delete;

		private:
			void createSwapchain();
			void destroySwapchain();

			VkSurfaceKHR m_surface = {};
			VkSwapchainKHR m_swapchain = {};

			u32 m_width = 0;
			u32 m_height = 0;
			b8 m_vsync = false;

			struct SwapchainImage {
				VkImage image;
				VkSemaphore signalSem;
				u32 descriptorHandle;
			};

			tbrs::Vec<SwapchainImage> m_swapchainImages;
			tbrs::Vec<std::pair<VkSemaphore, const Event>> m_submittedSems;
			tbrs::Vec<VkSemaphore> m_freeSems;

			VkShaderEXT m_blitShader = {};
	};
}