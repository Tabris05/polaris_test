#pragma once

#include <polaris/pod_types.hpp>
#include "descriptor_heap.hpp"

namespace pl {
	class Sampler {
		public:
			Sampler(const SamplerCreateInfo& ci);
			Sampler(Sampler&& src);
			Sampler& operator=(Sampler&& src);
			~Sampler();

			Sampler(const Sampler& src) = delete;
			Sampler& operator=(const Sampler& src) = delete;

			// "public" functions that should not be included in the public header
			u32 handle() const;

		private:
			DescriptorHeap* m_heap;
			u32 m_handle;
	};
}
