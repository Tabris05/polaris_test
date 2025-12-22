#pragma once

#include "queue.hpp"
#include "sync.hpp"

namespace pl {

	struct PresentInfo {
		SampledHandle texture;
		Queue& queue;
		std::optional<const EventInfo> waitEvent;
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
			alignas(8) byte reserved[136];
	};
}
