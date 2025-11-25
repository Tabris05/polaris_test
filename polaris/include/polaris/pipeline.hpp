#pragma once

#include <polaris/pod_types.hpp>

namespace pl {
	class Pipeline {
		public:
			Pipeline(const RasterPipelineCreateInfo& ci);
			Pipeline(const ComputePipelineCreateInfo& ci);
			Pipeline(Pipeline&& src);
			Pipeline& operator=(Pipeline&& src);
			~Pipeline();

			Pipeline(const Pipeline& src) = delete;
			Pipeline& operator=(const Pipeline& src) = delete;

		private:
			alignas(8) byte reserved[24];

	};
}