#pragma once

#include <polaris/pod_types.hpp>
#include <volk/volk.h>
#include <tabris/vec.hpp>

#include "descriptor_heap.hpp"
#include "device_memory_allocator.hpp"
#include "sampler_impl.hpp"

namespace pl {
	class RenderTarget {
		public:
			// "public" functions that should not be included in the public header
			RenderTarget(VkImageView view);
			VkImageView vkImageView() const;

		private:
			VkImageView view = {};
	};

	class TextureHandle {
		public:
			TextureHandle();
			TextureHandle(TextureHandle texture, const Sampler& sampler);
			TextureHandle(vec4<f32> invalidColor);

			// "public" functions that should not be included in the public header
			TextureHandle(u32 handle);

		private:
			u32 handle = 0x000FFFFF;
	};

	class ImageHandle {
		public:
			ImageHandle();
			ImageHandle(vec4<f32> invalidColor);

			// "public" functions that should not be included in the public header
			ImageHandle(u32 handle);

		private:
			u32 handle = 0x000FFFFF;
	};

	class Texture {
		public:
			RenderTarget makeRenderTarget(TextureView view = {});
			TextureHandle makeTextureHandle(TextureView view = {});
			ImageHandle makeImageHandle(TextureView view = {});

			Texture(const TextureCreateInfo& ci);
			Texture(Texture&& src);
			Texture& operator=(Texture&& src);
			~Texture();

			Texture(const Texture&) = delete;
			Texture& operator=(const Texture&) = delete;

			// "public" functions that should not be included in the public header
			VkImage vkImage() const;
			VkImageViewCreateInfo vkImageViewCreateInfo() const;
			vec3<u32> extent() const; // foo: should maybe add subresource to this

		private:
			struct VkImageViewInfo {
				VkImageViewCreateInfo viewCI = {};
				VkImageViewUsageCreateInfo viewUsageCI = {};
			};

			VkImageViewInfo vkImageViewInfo(const TextureView& view);

			VkDevice m_device = {};
			VkPhysicalDevice m_physicalDevice = {};
			VkImage m_image = {};

			VkImageViewCreateInfo m_imageViewCI = {};
			vec3<u32> m_extent;

			tbrs::Vec<VkImageView> m_renderTargetViews;
			tbrs::Vec<u32> m_shaderResourceViews;

			DescriptorHeap* m_heap = nullptr;
			DeviceMemoryAllocator* m_allocator = nullptr;

			DeviceMemory m_backingMem = {};
	};
}
