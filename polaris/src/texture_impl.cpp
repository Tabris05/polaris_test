#include "texture_impl.hpp"
#include "device_impl.hpp"
#include "vk_util.hpp"

namespace pl {

	RenderTarget::RenderTarget(VkImageView view)
		: view(view) {}

	VkImageView RenderTarget::vkImageView() const {
		return view;
	}

	TextureHandle::TextureHandle() = default;

	TextureHandle::TextureHandle(TextureHandle texture, const Sampler& sampler)
		: handle(texture.handle | sampler.handle() << 20) {}

	TextureHandle::TextureHandle(vec4<f32> invalidColor) {
		handle = 0x000FFFFF;
		handle |= static_cast<u32>(invalidColor.x * 4.0f) << 20;
		handle |= static_cast<u32>(invalidColor.y * 4.0f) << 23;
		handle |= static_cast<u32>(invalidColor.z * 4.0f) << 26;
		handle |= static_cast<u32>(invalidColor.w * 4.0f) << 29;
	}

	TextureHandle::TextureHandle(u32 handle) : handle(handle) {}

	ImageHandle::ImageHandle() = default;

	ImageHandle::ImageHandle(vec4<f32> invalidColor) {
		handle = 0x000FFFFF;
		handle |= static_cast<u32>(invalidColor.x * 4.0f) << 20;
		handle |= static_cast<u32>(invalidColor.y * 4.0f) << 23;
		handle |= static_cast<u32>(invalidColor.z * 4.0f) << 26;
		handle |= static_cast<u32>(invalidColor.w * 4.0f) << 29;
	}

	ImageHandle::ImageHandle(u32 handle) : handle(handle) {}

	RenderTarget Texture::makeRenderTarget(TextureView view) {
		VkImageViewInfo info = vkImageViewInfo(view);
		info.viewCI.pNext = &info.viewUsageCI;

		VkImageView viewHandle;
		vkCreateImageView(m_device, &info.viewCI, nullptr, &viewHandle);
		m_renderTargetViews.push(viewHandle);

		return RenderTarget{ viewHandle };
	}

	TextureHandle Texture::makeTextureHandle(TextureView view) {
		VkImageViewInfo info = vkImageViewInfo(view);
		info.viewCI.pNext = &info.viewUsageCI;

		u32 handle = m_heap->allocImageHandle(&info.viewCI, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE);
		m_shaderResourceViews.push(handle);

		return TextureHandle{ handle };
	}

	ImageHandle Texture::makeImageHandle(TextureView view) {
		VkImageViewInfo info = vkImageViewInfo(view);
		info.viewCI.pNext = &info.viewUsageCI;

		u32 handle = m_heap->allocImageHandle(&info.viewCI, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE);
		m_shaderResourceViews.push(handle);

		return ImageHandle{ handle };
	}
	
	Texture::Texture(const TextureCreateInfo& ci)
		: m_device(ci.device.vkDevice()), m_physicalDevice(ci.device.vkPhysicalDevice()), m_extent({ ci.width, ci.height, ci.depth }),
		m_heap(ci.device.descriptorHeap()), m_allocator(ci.device.deviceMemoryAllocator()) {

		VkImageCreateInfo vkci{
			.flags = VK_IMAGE_CREATE_MUTABLE_FORMAT_BIT | VK_IMAGE_CREATE_EXTENDED_USAGE_BIT,
			.imageType = vkImageType(ci.type),
			.format = vkFormat(ci.format),
			.extent = VkExtent3D{
				.width = ci.width,
				.height = ci.height,
				.depth = ci.depth
			},
			.mipLevels = ci.levels,
			.arrayLayers = ci.layers,
			.samples = static_cast<VkSampleCountFlagBits>(ci.samples),
			.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_STORAGE_BIT,
		};


		if(vkFormatIsDepthOrStencil(vkci.format)) {
			vkci.usage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
		} else {
			vkci.usage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		}

		if(ci.type == TextureType::Type2D && ci.width == ci.height && ci.layers >= 6 && ci.samples == 1) {
			vkci.flags |= VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
		}

		vkCreateImage(m_device, &vkci, nullptr, &m_image);
		m_backingMem = m_allocator->alloc(m_image);

		// foo: this is illegal without VK_IMAGE_USAGE_HOST_TRANSFER_BIT, but that disables DCC on NVIDIA
		// in mesa this function is a NO-OP for all 3 desktop vendors, so this is mainly for tooling (RenderDoc won't track undefined images)
		vkTransitionImageLayout(m_device, 1, &VkHostImageLayoutTransitionInfo{
			.image = m_image,
			.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
			.newLayout = VK_IMAGE_LAYOUT_GENERAL,
			.subresourceRange = {
				.aspectMask = vkAspectMask(vkci.format),
				.levelCount = VK_REMAINING_MIP_LEVELS,
				.layerCount = VK_REMAINING_ARRAY_LAYERS
			},
		});

		m_imageViewCI = VkImageViewCreateInfo{
			.image = m_image,
			.viewType = static_cast<VkImageViewType>(vkci.imageType),
			.format = vkci.format,
			.subresourceRange = {
				.aspectMask = vkAspectMask(vkci.format),
				.levelCount = vkci.mipLevels,
				.layerCount = vkci.arrayLayers,
			},
		};
	}

	Texture::Texture(Texture&& src) {
		memcpy(this, &src, sizeof(Texture));
		memset(&src, 0, sizeof(Texture));
	}

	Texture& Texture::operator=(Texture&& src) {
		this->~Texture();
		new (this) Texture(std::move(src));
		return *this;
	}

	Texture::~Texture() {
		if(m_device) {
			for(u32 srv : m_shaderResourceViews) {
				m_heap->freeImageHandle(srv);
			}

			for(VkImageView rtv : m_renderTargetViews) {
				vkDestroyImageView(m_device, rtv, nullptr);
			}

			vkDestroyImage(m_device, m_image, nullptr);
			m_allocator->free(m_backingMem);
		}
	}

	VkImage Texture::vkImage() const {
		return m_image;
	}

	VkImageViewCreateInfo Texture::vkImageViewCreateInfo() const {
		return m_imageViewCI;
	}

	vec3<u32> Texture::extent() const {
		return m_extent;
	}

	Texture::VkImageViewInfo Texture::vkImageViewInfo(const TextureView& view) {
		VkImageViewCreateInfo vci = m_imageViewCI;

		if(view.format != Format::None) {
			vci.format = vkFormat(view.format);
		}

		if(view.type != TextureViewType::Default) {
			vci.viewType = vkImageViewType(view.type);
		}

		vci.subresourceRange.baseMipLevel = view.region.baseLevel;
		vci.subresourceRange.baseArrayLayer = view.region.baseLayer;
		vci.subresourceRange.levelCount = view.region.numLevels;
		vci.subresourceRange.layerCount = view.region.numLayers;

		vci.components = VkComponentMapping{
			.r = static_cast<VkComponentSwizzle>(view.swizzleR),
			.g = static_cast<VkComponentSwizzle>(view.swizzleG),
			.b = static_cast<VkComponentSwizzle>(view.swizzleB),
			.a = static_cast<VkComponentSwizzle>(view.swizzleA),
		};

		if(view.region.aspect != DepthStencilAspect::Default) {
			vci.subresourceRange.aspectMask = vkAspectMask(view.region.aspect);
		}

		VkImageViewUsageCreateInfo vuci = {};
		VkFormatProperties props = {};
		vkGetPhysicalDeviceFormatProperties(m_physicalDevice, vci.format, &props);
		for(u32 bit = VK_IMAGE_USAGE_TRANSFER_SRC_BIT; bit <= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT; bit <<= 1) {
			if(vkImageUsageToFormatFeatures(static_cast<VkImageUsageFlagBits>(bit)) & props.optimalTilingFeatures) {
				vuci.usage |= bit;
			}
		}

		return { vci, vuci };
	}
}