#include "swapchain_impl.hpp"
#include "device_impl.hpp"
#include "command_buffer_impl.hpp"
#include "vk_util.hpp"
#include "shaders/push_constants.h"
#include "shaders/blit_comp.h"

namespace pl {
	const Event Swapchain::present(const PresentInfo& info) {
		u64 i;
		for(i = 0; i < m_submittedSems.count() && m_submittedSems[i].second.completed(); i++) {
			m_freeSems.push(m_submittedSems[i].first);
		}
		m_submittedSems.remove(0, i);

		VkSemaphore sem;
		if(m_freeSems.empty()) {
			vkCreateSemaphore(Device::get().vkDevice(), &VkSemaphoreCreateInfo{}, nullptr, &sem);
		}
		else {
			sem = m_freeSems.back();
			m_freeSems.pop();
		}

		u32 acquired;
		VkResult res = vkAcquireNextImageKHR(Device::get().vkDevice(), m_swapchain, std::numeric_limits<u64>::max(), sem, {}, &acquired);
		while(res == VK_ERROR_OUT_OF_DATE_KHR) {
			update(m_width, m_height, m_vsync);
			res = vkAcquireNextImageKHR(Device::get().vkDevice(), m_swapchain, std::numeric_limits<u64>::max(), sem, {}, &acquired);
		}

		CommandBuffer cmd = info.queue.beginRecording();

		cmd.pushConstants(PushConstants{ std::bit_cast<u32>(info.texture), m_swapchainImages[acquired].descriptorHandle });

		vkCmdPipelineBarrier2(cmd.vkCommandBuffer(), &VkDependencyInfo{
			.imageMemoryBarrierCount = 1,
			.pImageMemoryBarriers = &VkImageMemoryBarrier2{
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
			}
		});

		VkShaderStageFlagBits stage = VK_SHADER_STAGE_COMPUTE_BIT;
		vkCmdBindShadersEXT(cmd.vkCommandBuffer(), 1, &stage, &m_blitShader);
		cmd.dispatch((m_width + 7) / 8, (m_height + 7) / 8);

		vkCmdPipelineBarrier2(cmd.vkCommandBuffer(), &VkDependencyInfo{
			.imageMemoryBarrierCount = 1,
			.pImageMemoryBarriers = &VkImageMemoryBarrier2{
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
			}
		});

		const Event event = info.queue.submit(std::move(cmd), info.waitEvent, sem, m_swapchainImages[acquired].signalSem);
		m_submittedSems.push(sem, event);

		res = vkQueuePresentKHR(info.queue.vkQueue(), &VkPresentInfoKHR{
			.waitSemaphoreCount = 1,
			.pWaitSemaphores = &m_swapchainImages[acquired].signalSem,
			.swapchainCount = 1,
			.pSwapchains = &m_swapchain,
			.pImageIndices = &acquired
		});

		if(res != VK_SUCCESS) {
			update(m_width, m_height, m_vsync);
		}

		return event;
	}

	void Swapchain::update(u32 width, u32 height, b8 vsync) {
		m_width = width;
		m_height = height;
		m_vsync = vsync;

		// foo: eventually should try to reuse old swapchain
		Device::idle();
		destroySwapchain();
		createSwapchain();
	}

	Swapchain::Swapchain(const SwapchainCreateInfo& ci) : m_width(ci.width), m_height(ci.height), m_vsync(ci.vsync) {
		switch(ci.nativeWindow.type) {
			case NativeWindowType::Headless: {
				vkCreateHeadlessSurfaceEXT(Device::get().vkInstance(), &VkHeadlessSurfaceCreateInfoEXT{}, nullptr, &m_surface);
				break;
			}
			case NativeWindowType::Win32: {
				vkCreateWin32SurfaceKHR(Device::get().vkInstance(), &VkWin32SurfaceCreateInfoKHR{
					.hinstance = ci.nativeWindow.win32.hinstance,
					.hwnd = ci.nativeWindow.win32.hwnd
				}, nullptr, &m_surface);
				break;
			}
			case NativeWindowType::XCB: {
				vkCreateXcbSurfaceKHR(Device::get().vkInstance(), &VkXcbSurfaceCreateInfoKHR{
					.connection = ci.nativeWindow.xcb.connection,
					.window = ci.nativeWindow.xcb.window
				}, nullptr, &m_surface);
				break;
			}
			case NativeWindowType::Wayland: {
				vkCreateWaylandSurfaceKHR(Device::get().vkInstance(), &VkWaylandSurfaceCreateInfoKHR{
					.display = ci.nativeWindow.wayland.display,
					.surface = ci.nativeWindow.wayland.surface
				}, nullptr, &m_surface);
				break;
			}
			case NativeWindowType::Metal: {
				vkCreateMetalSurfaceEXT(Device::get().vkInstance(), &VkMetalSurfaceCreateInfoEXT{
					.pLayer = ci.nativeWindow.metal.pLayer
				}, nullptr, &m_surface);
				break;
			}
			case NativeWindowType::Android: {
				vkCreateAndroidSurfaceKHR(Device::get().vkInstance(), &VkAndroidSurfaceCreateInfoKHR{
					.window = ci.nativeWindow.android.window
				}, nullptr, &m_surface);
				break;
			}
			case NativeWindowType::Vi: {
				vkCreateViSurfaceNN(Device::get().vkInstance(), &VkViSurfaceCreateInfoNN{
					.window = ci.nativeWindow.vi.window
				}, nullptr, &m_surface);
				break;
			}
		}

		vkCreateShadersEXT(Device::get().vkDevice(), 1, &VkShaderCreateInfoEXT{
			.flags = VK_SHADER_CREATE_DESCRIPTOR_HEAP_BIT_EXT,
			.stage = VK_SHADER_STAGE_COMPUTE_BIT,
			.codeType = VK_SHADER_CODE_TYPE_SPIRV_EXT,
			.codeSize = sizeof(blit_comp),
			.pCode = blit_comp,
			.pName = "csmain"
		}, nullptr, &m_blitShader);

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
		destroySwapchain();

		for(VkSemaphore sem : m_freeSems) {
			vkDestroySemaphore(Device::get().vkDevice(), sem, nullptr);
		}

		for(auto [sem, _] : m_submittedSems) {
			vkDestroySemaphore(Device::get().vkDevice(), sem, nullptr);
		}

		vkDestroyShaderEXT(Device::get().vkDevice(), m_blitShader, nullptr);
		vkDestroySurfaceKHR(Device::get().vkInstance(), m_surface, nullptr);
	}

	void Swapchain::createSwapchain() {
		vkCreateSwapchainKHR(Device::get().vkDevice(), &VkSwapchainCreateInfoKHR{
			.surface = m_surface,
			.minImageCount = 3,
			.imageFormat = VK_FORMAT_R8G8B8A8_UNORM,
			.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
			.imageExtent = { m_width, m_height },
			.imageArrayLayers = 1,
			.imageUsage = VK_IMAGE_USAGE_STORAGE_BIT,
			.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
			.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
			.presentMode = m_vsync ? VK_PRESENT_MODE_FIFO_KHR : VK_PRESENT_MODE_IMMEDIATE_KHR,
			.clipped = true,
		}, nullptr, &m_swapchain);

		u32 count;
		vkGetSwapchainImagesKHR(Device::get().vkDevice(), m_swapchain, &count, nullptr);

		tbrs::Vec<VkImage> images(count);
		vkGetSwapchainImagesKHR(Device::get().vkDevice(), m_swapchain, &count, images.data());

		VkSemaphore sem;
		for(VkImage& img : images) {
			vkCreateSemaphore(Device::get().vkDevice(), &VkSemaphoreCreateInfo{}, nullptr, &sem);
			m_swapchainImages.push(img, sem, Device::get().descriptorHeap().allocImageHandle(&VkImageViewCreateInfo{
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
			}, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE));
		}
	}

	void Swapchain::destroySwapchain() {
		for(auto [_, signalSem, descriptorHandle] : m_swapchainImages) {
			Device::get().descriptorHeap().freeImageHandle(descriptorHandle);
			vkDestroySemaphore(Device::get().vkDevice(), signalSem, nullptr);
		}
		m_swapchainImages.clear();
		vkDestroySwapchainKHR(Device::get().vkDevice(), m_swapchain, nullptr);
	}
}