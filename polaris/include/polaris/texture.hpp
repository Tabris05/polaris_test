#pragma once

#include "pod_types.hpp"
#include "sampler.hpp"

namespace pl {
	class RenderTarget {
		alignas(8) byte reserved[8];
	};

	class TextureHandle {
		public:
			TextureHandle(TextureHandle texture, const Sampler& sampler);
			TextureHandle(vec4<f32> invalidColor);
		private:
			alignas(4) byte reserved[4];
	};

	struct ImageHandle {
		alignas(4) byte reserved[4];
	};

	class Texture {
		public:
			RenderTarget makeRenderTarget(const TextureView& view = {});
			TextureHandle makeTextureHandle(const TextureView& view = {});
			ImageHandle makeImageHandle(const TextureView& view = {});

			Texture(const TextureCreateInfo& ci);
			Texture(Texture&& src);
			Texture& operator=(Texture&& src);
			~Texture();

			Texture(const Texture&) = delete;
			Texture& operator=(const Texture&) = delete;

		private:
			alignas(8) byte reserved[192];
	};
}
