#include "texture_impl.hpp"
#include "device_impl.hpp"
#include "vk_util.hpp"

namespace pl {

	SampledHandle::SampledHandle(u32 handle)
		: handle(handle) {}

	SampledHandle::SampledHandle(SampledHandle texture, const Sampler& sampler)
		: handle(texture.handle | sampler.handle() << 20) {}

	RenderTargetHandle Texture::getRenderTargetHandle(const TextureView& view) {
		VkImageViewInfo info = vkImageViewInfo(view);
		info.viewCI.pNext = &info.viewUsageCI;

		VkImageView viewHandle;
		vkCreateImageView(m_device, &info.viewCI, nullptr, &viewHandle);
		m_renderTargetViews.push(viewHandle);

		return RenderTargetHandle{ viewHandle };
	}

	SampledHandle Texture::getSampledHandle(const TextureView& view) {
		VkImageViewInfo info = vkImageViewInfo(view);
		info.viewCI.pNext = &info.viewUsageCI;

		u32 handle = m_heap->allocImageHandle(&info.viewCI, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE);
		m_shaderResourceViews.push(handle);

		return SampledHandle{ handle };
	}

	RWHandle Texture::getRWHandle(const TextureView& view) {
		VkImageViewInfo info = vkImageViewInfo(view);
		info.viewCI.pNext = &info.viewUsageCI;

		u32 handle = m_heap->allocImageHandle(&info.viewCI, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE);
		m_shaderResourceViews.push(handle);

		return RWHandle{ handle };
	}
	
	Texture::Texture(const TextureCreateInfo& ci)
		: m_device(ci.device.vkDevice()), m_physicalDevice(ci.device.vkPhysicalDevice()), m_dimensions(ci.width, ci.height, ci.depth),
		m_heap(ci.device.descriptorHeap()), m_allocator(ci.device.deviceMemoryAllocator()) {

		VkImageCreateInfo vkci{
			.flags = VK_IMAGE_CREATE_MUTABLE_FORMAT_BIT | VK_IMAGE_CREATE_EXTENDED_USAGE_BIT,
			.imageType = VK_IMAGE_TYPE_1D,
			.format = vkFormat(ci.format),
			.extent = VkExtent3D{
				.width = ci.width,
				.height = ci.height,
				.depth = ci.depth
			},
			.mipLevels = ci.levels,
			.arrayLayers = ci.layers,
			.samples = vkSampleCount(ci.samples),
			.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_HOST_TRANSFER_BIT,
		};

		if(vkFormatIsDepthOrStencil(vkci.format)) {
			vkci.usage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
		} else {
			vkci.usage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		}

		if(ci.height > 1) {
			vkci.imageType = VK_IMAGE_TYPE_2D;
		}

		if(ci.depth > 1) {
			vkci.imageType = VK_IMAGE_TYPE_3D;
		}

		if(vkci.imageType == VK_IMAGE_TYPE_2D && ci.width == ci.height && ci.layers >= 6 && ci.samples == 1) {
			vkci.flags |= VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
		}

		vkCreateImage(m_device, &vkci, nullptr, &m_image);
		m_backingMem = m_allocator->alloc(m_image);

		vkTransitionImageLayout(m_device, 1, ptr(VkHostImageLayoutTransitionInfo{
			.image = m_image,
			.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
			.newLayout = VK_IMAGE_LAYOUT_GENERAL,
			.subresourceRange = {
				.aspectMask = vkAspectMask(vkci.format),
				.levelCount = vkci.mipLevels,
				.layerCount = vkci.arrayLayers,
			}
		}));

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

	vec3<u32> Texture::dimensions() const {
		return m_dimensions;
	}

	Texture::VkImageViewInfo Texture::vkImageViewInfo(const TextureView& view) {
		VkImageViewCreateInfo vci = m_imageViewCI;

		if(view.format != Format::None) {
			vci.format = vkFormat(view.format);
		}

		vci.subresourceRange.baseMipLevel = view.baseLevel;
		vci.subresourceRange.baseArrayLayer = view.baseLayer;

		if(view.levelCount != 0) {
			vci.subresourceRange.levelCount = view.levelCount;
		}

		if(view.layerCount != 0) {
			vci.subresourceRange.layerCount = view.layerCount;
		}

		vci.components = VkComponentMapping{
			.r = static_cast<VkComponentSwizzle>(view.swizzleR),
			.g = static_cast<VkComponentSwizzle>(view.swizzleG),
			.b = static_cast<VkComponentSwizzle>(view.swizzleB),
			.a = static_cast<VkComponentSwizzle>(view.swizzleA),
		};

		if(view.aspect != DepthStencilAspect::Default) {
			switch(view.aspect) {
				case DepthStencilAspect::Depth:
					vci.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
					break;
				case DepthStencilAspect::Stencil:
					vci.subresourceRange.aspectMask = VK_IMAGE_ASPECT_STENCIL_BIT;
					break;
				default:
					break;
			}
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