#include "shader_impl.hpp"
#include "device_impl.hpp"
#include "vk_util.hpp"

namespace pl {
	Shader::Shader(const ShaderCreateInfo& ci) : m_stage(vkShaderStage(ci.stage)) {
		vkCreateShadersEXT(Device::get().vkDevice(), 1, &VkShaderCreateInfoEXT{
			.flags = VK_SHADER_CREATE_DESCRIPTOR_HEAP_BIT_EXT | (ci.stage == ShaderStage::Mesh ? VK_SHADER_CREATE_NO_TASK_SHADER_BIT_EXT : 0u),
			.stage = m_stage,
			.nextStage = ci.stage == ShaderStage::Mesh ? VK_SHADER_STAGE_FRAGMENT_BIT : 0u,
			.codeType = VK_SHADER_CODE_TYPE_SPIRV_EXT,
			.codeSize = ci.code.size(),
			.pCode = ci.code.data(),
			.pName = ci.entryPoint
		}, nullptr, &m_shader);
	}

	Shader::Shader(Shader&& src) {
		memcpy(this, &src, sizeof(Shader));
		memset(&src, 0, sizeof(Shader));
	}

	Shader& Shader::operator=(Shader&& src) {
		this->~Shader();
		new (this) Shader(std::move(src));
		return *this;
	}

	Shader::~Shader() {
		vkDestroyShaderEXT(Device::get().vkDevice(), m_shader, nullptr);
	}

	VkShaderEXT Shader::vkShader() const {
		return m_shader;
	}

	VkShaderStageFlagBits Shader::vkShaderStageBits() const {
		return m_stage;
	}
}