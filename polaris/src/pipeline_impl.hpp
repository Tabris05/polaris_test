#pragma once

#include <polaris/pod_types.hpp>
#include <volk/volk.h>

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
	
			// "public" functions that should not be included in the public header
			void bind(VkCommandBuffer cmd) const;

		private:
			VkDevice m_device = {};
			VkPipeline m_pipeline = {};
			VkPipelineBindPoint m_bindPoint = {};
	
	};
}