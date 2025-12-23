#pragma once

#include "pod_types.hpp"
#include "sampler.hpp"

namespace pl {
	class RenderTargetHandle {
		alignas(8) byte reserved[8];
	};

	class SampledHandle {
		public:
			SampledHandle(SampledHandle texture, const Sampler& sampler);
		private:
			alignas(4) byte reserved[4];
	};

	struct RWHandle {
		alignas(4) byte reserved[4];
	};

	class Texture {
		public:
			RenderTargetHandle getRenderTargetHandle(const TextureView& view = {});
			SampledHandle getSampledHandle(const TextureView& view = {});
			RWHandle getRWHandle(const TextureView& view = {});

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
