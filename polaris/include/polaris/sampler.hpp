#pragma once
#include "pod_types.hpp"

namespace pl {
	class Sampler {
		public:
			Sampler(const SamplerCreateInfo& ci);
			Sampler(Sampler&& src);
			Sampler& operator=(Sampler&& src);
			~Sampler();

			Sampler(const Sampler& src) = delete;
			Sampler& operator=(const Sampler& src) = delete;

		private:
			alignas(8) byte reserved[16];
	};
};
