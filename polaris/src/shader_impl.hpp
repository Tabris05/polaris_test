#pragma once

#include <polaris/pod_types.hpp>
#include <volk/volk.h>

namespace pl {
	class Shader {
		public:
			Shader(const ShaderCreateInfo& ci);
			Shader(Shader&& src);
			Shader& operator=(Shader&& src);
			~Shader();
	
			Shader(const Shader& src) = delete;
			Shader& operator=(const Shader& src) = delete;
	
			// "public" functions that should not be included in the public header
			VkShaderEXT vkShader() const;
			VkShaderStageFlagBits vkShaderStageBits() const;

		private:
			VkShaderEXT m_shader = {};
			VkShaderStageFlagBits m_stage = {};
	};
}