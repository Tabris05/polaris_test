#include "pipeline_impl.hpp"
#include "device_impl.hpp"
#include "vk_util.hpp"

namespace pl {
	Pipeline::Pipeline(const RasterPipelineCreateInfo& ci)
		: m_device(ci.device.vkDevice()), m_bindPoint(VK_PIPELINE_BIND_POINT_GRAPHICS) {

		VkPipelineShaderStageCreateInfo stageCIs[5];
		VkShaderModuleCreateInfo moduleCIs[5];
		for(u8 i = 0; i < ci.shaders.count(); i++) {
			moduleCIs[i] = VkShaderModuleCreateInfo{
				.codeSize = ci.shaders[i].code.size(),
				.pCode = ci.shaders[i].code.data()
			};

			stageCIs[i] = VkPipelineShaderStageCreateInfo{
				.pNext = &moduleCIs[i],
				.stage = vkShaderStage(ci.shaders[i].stage),
				.pName = ci.shaders[i].entryPoint
			};
		}

		VkFormat colorFormats[8];
		for(u8 i = 0; i < ci.colorFormats.count(); i++) {
			colorFormats[i] = vkFormat(ci.colorFormats[i]);
		}

		vkCreateGraphicsPipelines(m_device, nullptr, 1, ptr(VkGraphicsPipelineCreateInfo{
			.pNext = ptr(VkPipelineRenderingCreateInfo{
				.pNext = ptr(VkPipelineCreateFlags2CreateInfo{
					.flags = VK_PIPELINE_CREATE_2_DESCRIPTOR_HEAP_BIT_EXT
				}),
				.colorAttachmentCount = static_cast<u32>(ci.colorFormats.count()),
				.pColorAttachmentFormats = colorFormats,
				.depthAttachmentFormat = vkFormat(ci.depthFormat)
			}),
			.stageCount = static_cast<u32>(ci.shaders.count()),
			.pStages = stageCIs,
			.pVertexInputState = ptr(VkPipelineVertexInputStateCreateInfo{}),
			.pInputAssemblyState = ptr(VkPipelineInputAssemblyStateCreateInfo{.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST }),
			.pViewportState = ptr(VkPipelineViewportStateCreateInfo{.viewportCount = 1, .scissorCount = 1}),
			.pRasterizationState = ptr(VkPipelineRasterizationStateCreateInfo{.cullMode = static_cast<VkCullModeFlags>(ci.cullFace), .frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE, .lineWidth = 1.0f}),
			.pMultisampleState = ptr(VkPipelineMultisampleStateCreateInfo{.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT }),
			.pDepthStencilState = ptr(VkPipelineDepthStencilStateCreateInfo{.depthTestEnable = true, .depthWriteEnable = ci.depthWriteEnable, .depthCompareOp = vkCompareOp(ci.depthCompareOp)}),
			.pColorBlendState = ptr(VkPipelineColorBlendStateCreateInfo{.attachmentCount = 1, .pAttachments = ptr(VkPipelineColorBlendAttachmentState{
				.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
			})}),
			.pDynamicState = ptr(VkPipelineDynamicStateCreateInfo{.dynamicStateCount = 2, .pDynamicStates = ptr({ VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR }) }),
		}), nullptr, &m_pipeline);
	}

	Pipeline::Pipeline(const ComputePipelineCreateInfo& ci)
		: m_device(ci.device.vkDevice()), m_bindPoint(VK_PIPELINE_BIND_POINT_COMPUTE) {
		vkCreateComputePipelines(m_device, {}, 1, ptr(VkComputePipelineCreateInfo{
			.pNext = ptr(VkPipelineCreateFlags2CreateInfo{
				.flags = VK_PIPELINE_CREATE_2_DESCRIPTOR_HEAP_BIT_EXT
			}),
			.stage = {
				.pNext = ptr(VkShaderModuleCreateInfo{
					.codeSize = static_cast<u32>(ci.shader.code.size()),
					.pCode = ci.shader.code.data()
				}),
				.stage = vkShaderStage(ci.shader.stage),
				.pName = ci.shader.entryPoint
			},
		}), nullptr, &m_pipeline);
	}

	Pipeline::Pipeline(Pipeline&& src) {
		memcpy(this, &src, sizeof(Pipeline));
		memset(&src, 0, sizeof(Pipeline));
	}

	Pipeline& Pipeline::operator=(Pipeline&& src) {
		this->~Pipeline();
		new (this) Pipeline(std::move(src));
		return *this;
	}

	Pipeline::~Pipeline() {
		if(m_device) {
			vkDestroyPipeline(m_device, m_pipeline, nullptr);
		}
	}

	void Pipeline::bind(VkCommandBuffer cmd) const {
		vkCmdBindPipeline(cmd, m_bindPoint, m_pipeline);
	}
}