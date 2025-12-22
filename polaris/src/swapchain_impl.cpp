#include "swapchain_impl.hpp"
#include "device_impl.hpp"
#include "command_buffer_impl.hpp"
#include "vk_util.hpp"
#include "shaders/push_constants.h"
#include "shaders/blit_comp.h"

namespace pl {
	const Event Swapchain::present(const PresentInfo& info) {
		u64 newHead;
		for(newHead = 0; newHead < m_submittedSems.count(); newHead++) {
			if(m_submittedSems[newHead].second.completed()) {
				m_freeSems.push(m_submittedSems[newHead].first);
			}
			else {
				break;
			}
		}

		// really janky way to erase the first N elements
		memmove(m_submittedSems.data(), m_submittedSems.data() + newHead, (m_submittedSems.count() - newHead) * sizeof(m_submittedSems.front()));
		m_submittedSems.setCount(m_submittedSems.count() - newHead);

		VkSemaphore sem;
		if(m_freeSems.empty()) {
			vkCreateSemaphore(m_device, ptr(VkSemaphoreCreateInfo{}), nullptr, &sem);
		}
		else {
			sem = m_freeSems.back();
			m_freeSems.pop();
		}

		u32 acquired;
		VkResult res = vkAcquireNextImageKHR(m_device, m_swapchain, std::numeric_limits<u64>::max(), sem, {}, &acquired);
		while(res == VK_ERROR_OUT_OF_DATE_KHR) {
			update(m_width, m_height, m_presentMode);
			res = vkAcquireNextImageKHR(m_device, m_swapchain, std::numeric_limits<u64>::max(), sem, {}, &acquired);
		}

		CommandBuffer cmd = info.queue.beginRecording();

		cmd.pushConstants(PushConstants{ info.texture.handle, m_swapchainImages[acquired].descriptorHandle });

		vkCmdPipelineBarrier2(cmd.vkCommandBuffer(), ptr(VkDependencyInfo{
			.imageMemoryBarrierCount = 1,
			.pImageMemoryBarriers = ptr(VkImageMemoryBarrier2{
				.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
				.srcStageMask = VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT,
				.dstStageMask = VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT,
				.dstAccessMask = VK_ACCESS_2_SHADER_WRITE_BIT,
				.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
				.newLayout = VK_IMAGE_LAYOUT_GENERAL,
				.image = m_swapchainImages[acquired].image,
				.subresourceRange = {
					.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
					.baseMipLevel = 0,
					.levelCount = 1,
					.baseArrayLayer = 0,
					.layerCount = 1
				}
			})
		}));

		vkCmdBindPipeline(cmd.vkCommandBuffer(), VK_PIPELINE_BIND_POINT_COMPUTE, m_blitPipeline);
		cmd.dispatch((m_width + 7) / 8, (m_height + 7) / 8);

		vkCmdPipelineBarrier2(cmd.vkCommandBuffer(), ptr(VkDependencyInfo{
			.imageMemoryBarrierCount = 1,
			.pImageMemoryBarriers = ptr(VkImageMemoryBarrier2{
				.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
				.srcStageMask = VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT,
				.srcAccessMask = VK_ACCESS_2_SHADER_WRITE_BIT,
				.dstStageMask = VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT,
				.oldLayout = VK_IMAGE_LAYOUT_GENERAL,
				.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
				.image = m_swapchainImages[acquired].image,
				.subresourceRange = {
					.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
					.baseMipLevel = 0,
					.levelCount = 1,
					.baseArrayLayer = 0,
					.layerCount = 1
				}
			})
		}));

		const Event event = info.queue.submit(std::move(cmd), info.waitInfo, sem, m_swapchainImages[acquired].signalSem);
		m_submittedSems.push(sem, event);

		res = vkQueuePresentKHR(info.queue.vkQueue(), ptr(VkPresentInfoKHR{
			.waitSemaphoreCount = 1,
			.pWaitSemaphores = &m_swapchainImages[acquired].signalSem,
			.swapchainCount = 1,
			.pSwapchains = &m_swapchain,
			.pImageIndices = &acquired
		}));

		if(res != VK_SUCCESS) {
			update(m_width, m_height, m_presentMode);
		}

		return event;
	}

	void Swapchain::update(u32 width, u32 height, PresentMode mode) {
		m_width = width;
		m_height = height;

		if(static_cast<u8>(mode) != 0xff) {
			m_presentMode = mode;
		}

		// foo: eventually should try to reuse old swapchain
		vkDeviceWaitIdle(m_device);
		destroySwapchain();
		createSwapchain();
	}

	Swapchain::Swapchain(const SwapchainCreateInfo& ci)
		: m_instance(ci.device.vkInstance()), m_device(ci.device.vkDevice()), m_width(ci.width), m_height(ci.height), m_presentMode(ci.mode), m_heap(ci.device.descriptorHeap()) {

		switch(ci.nativeWindow.type) {
			case NativeWindowType::Headless: {
				vkCreateHeadlessSurfaceEXT(m_instance, ptr(VkHeadlessSurfaceCreateInfoEXT{}), nullptr, &m_surface);
				break;
			}
			case NativeWindowType::Win32: {
				vkCreateWin32SurfaceKHR(m_instance, ptr(VkWin32SurfaceCreateInfoKHR{
					.hinstance = ci.nativeWindow.win32.hinstance,
					.hwnd = ci.nativeWindow.win32.hwnd
				}), nullptr, &m_surface);
				break;
			}
			case NativeWindowType::Xcb: {
				vkCreateXcbSurfaceKHR(m_instance, ptr(VkXcbSurfaceCreateInfoKHR{
					.connection = ci.nativeWindow.xcb.connection,
					.window = ci.nativeWindow.xcb.window
				}), nullptr, &m_surface);
				break;
			}
			case NativeWindowType::Wayland: {
				vkCreateWaylandSurfaceKHR(m_instance, ptr(VkWaylandSurfaceCreateInfoKHR{
					.display = ci.nativeWindow.wayland.display,
					.surface = ci.nativeWindow.wayland.surface
				}), nullptr, &m_surface);
				break;
			}
			case NativeWindowType::Metal: {
				vkCreateMetalSurfaceEXT(m_instance, ptr(VkMetalSurfaceCreateInfoEXT{
					.pLayer = ci.nativeWindow.metal.pLayer
				}), nullptr, &m_surface);
				break;
			}
			case NativeWindowType::Android: {
				vkCreateAndroidSurfaceKHR(m_instance, ptr(VkAndroidSurfaceCreateInfoKHR{
					.window = ci.nativeWindow.android.window
				}), nullptr, &m_surface);
				break;
			}
			case NativeWindowType::Vi: {
				vkCreateViSurfaceNN(m_instance, ptr(VkViSurfaceCreateInfoNN{
					.window = ci.nativeWindow.vi.window
				}), nullptr, &m_surface);
				break;
			}
		}

		vkCreateComputePipelines(m_device, {}, 1, ptr(VkComputePipelineCreateInfo{
			.stage = {
				.pNext = ptr(VkShaderModuleCreateInfo{
					.codeSize = sizeof(blit_comp),
					.pCode = blit_comp
				}),
				.stage = VK_SHADER_STAGE_COMPUTE_BIT,
				.pName = "csmain"
			},
			.layout = ci.device.descriptorHeap()->vkPipelineLayout()
		}), nullptr, &m_blitPipeline);

		createSwapchain();
	}

	Swapchain::Swapchain(Swapchain&& src) {
		memcpy(this, &src, sizeof(Swapchain));
		memset(&src, 0, sizeof(Swapchain));
	}

	Swapchain& Swapchain::operator=(Swapchain&& src) {
		this->~Swapchain();
		new (this) Swapchain(std::move(src));
		return *this;
	}

	Swapchain::~Swapchain() {
		if(m_device) {
			destroySwapchain();

			for(VkSemaphore sem : m_freeSems) {
				vkDestroySemaphore(m_device, sem, nullptr);
			}

			for(auto [sem, _] : m_submittedSems) {
				vkDestroySemaphore(m_device, sem, nullptr);
			}

			vkDestroyPipeline(m_device, m_blitPipeline, nullptr);
			vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
		}
	}

	void Swapchain::createSwapchain() {
		vkCreateSwapchainKHR(m_device, ptr(VkSwapchainCreateInfoKHR{
			.surface = m_surface,
			.minImageCount = 3,
			.imageFormat = VK_FORMAT_R8G8B8A8_UNORM,
			.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
			.imageExtent = { m_width, m_height },
			.imageArrayLayers = 1,
			.imageUsage = VK_IMAGE_USAGE_STORAGE_BIT,
			.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
			.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
			.presentMode = vkPresentMode(m_presentMode),
			.clipped = true,
		}), nullptr, &m_swapchain);

		u32 count;
		vkGetSwapchainImagesKHR(m_device, m_swapchain, &count, nullptr);

		tbrs::Vec<VkImage> images(count);
		vkGetSwapchainImagesKHR(m_device, m_swapchain, &count, images.data());

		VkSemaphore sem;
		for(VkImage& img : images) {
			vkCreateSemaphore(m_device, ptr(VkSemaphoreCreateInfo{}), nullptr, &sem);
			m_swapchainImages.push(img, sem, m_heap->allocImageHandle(ptr(VkImageViewCreateInfo{
				.image = img,
				.viewType = VK_IMAGE_VIEW_TYPE_2D,
				.format = VK_FORMAT_R8G8B8A8_UNORM,
				.subresourceRange = {
					.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
					.baseMipLevel = 0,
					.levelCount = 1,
					.baseArrayLayer = 0,
					.layerCount = 1
				}
			}), VK_DESCRIPTOR_TYPE_STORAGE_IMAGE));
		}
	}

	void Swapchain::destroySwapchain() {
		for(auto [_, signalSem, descriptorHandle] : m_swapchainImages) {
			m_heap->freeImageHandle(descriptorHandle);
			vkDestroySemaphore(m_device, signalSem, nullptr);
		}
		m_swapchainImages.clear();
		vkDestroySwapchainKHR(m_device, m_swapchain, nullptr);
	}
}