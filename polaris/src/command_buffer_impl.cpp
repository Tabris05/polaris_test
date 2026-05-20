#include "command_buffer_impl.hpp"
#include <tabris/types.hpp>
#include "vk_util.hpp"

namespace pl {
	void CommandBuffer::barrier(PipelineStage src, PipelineStage dst) {
		vkCmdPipelineBarrier2(m_cmd, &VkDependencyInfo{
			.memoryBarrierCount = 1,
			.pMemoryBarriers = &VkMemoryBarrier2{
				.srcStageMask = vkStageMask(src),
				.srcAccessMask = vkAccessMask(src),
				.dstStageMask = vkStageMask(dst),
				.dstAccessMask = vkAccessMask(dst)
			}
		});
	}

	void CommandBuffer::beginRenderPass(const RenderPassBeginInfo& info) {
		VkRenderingAttachmentInfo colorAttachments[8];
		for(u8 i = 0; i < info.colorTargets.count(); i++) {
			colorAttachments[i] = VkRenderingAttachmentInfo{
				.imageView = info.colorTargets[i].target.vkImageView(),
				.imageLayout = VK_IMAGE_LAYOUT_GENERAL,
				.loadOp = vkLoadOp(info.colorTargets[i].loadOp),
				.storeOp = vkStoreOp(info.colorTargets[i].storeOp),
				.clearValue = std::bit_cast<VkClearValue>(info.colorTargets[i].clearValue)
			};
		}

		VkRenderingAttachmentInfo depthAttachment;
		if(info.depthTarget.has_value()) {
			depthAttachment = VkRenderingAttachmentInfo{
				.imageView = info.depthTarget.value().target.vkImageView(),
				.imageLayout = VK_IMAGE_LAYOUT_GENERAL,
				.loadOp = vkLoadOp(info.depthTarget.value().loadOp),
				.storeOp = vkStoreOp(info.depthTarget.value().storeOp),
				.clearValue = std::bit_cast<VkClearValue>(info.depthTarget.value().clearValue)
			};
		}

		VkRenderingAttachmentInfo stencilAttachment;
		if(info.stencilTarget.has_value()) {
			stencilAttachment = VkRenderingAttachmentInfo{
				.imageView = info.stencilTarget.value().target.vkImageView(),
				.imageLayout = VK_IMAGE_LAYOUT_GENERAL,
				.loadOp = vkLoadOp(info.stencilTarget.value().loadOp),
				.storeOp = vkStoreOp(info.stencilTarget.value().storeOp),
				.clearValue = std::bit_cast<VkClearValue>(info.stencilTarget.value().clearValue)
			};
		}

		vkCmdBeginRendering(m_cmd, &VkRenderingInfo{
			.renderArea = { static_cast<i32>(info.renderArea.x), static_cast<i32>(info.renderArea.y), info.renderArea.width, info.renderArea.height },
			.layerCount = 1,
			.colorAttachmentCount = static_cast<u32>(info.colorTargets.count()),
			.pColorAttachments = colorAttachments,
			.pDepthAttachment = info.depthTarget.has_value() ? &depthAttachment : nullptr,
			.pStencilAttachment = info.stencilTarget.has_value() ? &stencilAttachment : nullptr
		});
	}

	void CommandBuffer::bindShaders(View<std::reference_wrapper<const Shader>> shaders) {
		VkShaderStageFlagBits stages[3];
		VkShaderEXT shaderHandles[3];

		for(u8 i = 0; i < shaders.count(); i++) {
			stages[i] = shaders[i].get().vkShaderStageBits();
			shaderHandles[i] = shaders[i].get().vkShader();
		}

		vkCmdBindShadersEXT(m_cmd, shaders.count(), stages, shaderHandles);
	}

	void CommandBuffer::clearBuffer(BufferRange range, u32 value) {
		vkCmdFillMemoryKHR(m_cmd, &VkDeviceAddressRangeKHR{
			.address = range.address,
			.size = range.size
		}, VK_ADDRESS_COMMAND_FULLY_BOUND_BIT_KHR, value);
	}

	void CommandBuffer::clearTexture(const Texture& texture, ClearValue value, TextureRegion region) {
		VkImageSubresourceRange range = texture.vkImageViewCreateInfo().subresourceRange;
		range.baseMipLevel = region.baseLevel;
		range.baseArrayLayer = region.baseLayer;
		range.levelCount = region.numLevels;
		range.layerCount = region.numLayers;
		if(range.aspectMask & VK_IMAGE_ASPECT_COLOR_BIT) {
			vkCmdClearColorImage(m_cmd, texture.vkImage(), VK_IMAGE_LAYOUT_GENERAL, reinterpret_cast<VkClearColorValue*>(&value), 1, &range);
		}
		else {
			vkCmdClearDepthStencilImage(m_cmd, texture.vkImage(), VK_IMAGE_LAYOUT_GENERAL, reinterpret_cast<VkClearDepthStencilValue*>(&value), 1, &range);
		}
	}

	void CommandBuffer::copyTexture(const Texture& src, const Texture& dst, TextureRegion srcRegion, TextureRegion dstRegion) {
		vkCmdCopyImage(m_cmd, src.vkImage(), VK_IMAGE_LAYOUT_GENERAL, dst.vkImage(), VK_IMAGE_LAYOUT_GENERAL, 1,
			&VkImageCopy{
				.srcSubresource{
					src.vkImageViewCreateInfo().subresourceRange.aspectMask,
					srcRegion.baseLevel,
					srcRegion.baseLayer,
					srcRegion.numLayers
				},
				.srcOffset{ 0, 0, 0 },
				.dstSubresource{
					dst.vkImageViewCreateInfo().subresourceRange.aspectMask,
					dstRegion.baseLevel,
					dstRegion.baseLayer,
					srcRegion.numLayers
				},
				.dstOffset{ 0, 0, 0 },
				.extent{
					std::max(src.extent().x >> srcRegion.baseLevel, 1u),
					std::max(src.extent().y >> srcRegion.baseLevel, 1u),
					std::max(src.extent().z >> srcRegion.baseLevel, 1u)
				}
			}
		);
	}

	void CommandBuffer::draw(u32 groupsX, u32 groupsY, u32 groupsZ) {
		vkCmdDrawMeshTasksEXT(m_cmd, groupsX, groupsY, groupsZ);
	}

	void CommandBuffer::drawIndirect(DeviceAddress indirectBuffer) {
		vkCmdDrawMeshTasksIndirect2EXT(m_cmd, &VkDrawIndirect2InfoKHR{
			.addressRange{
				.address = indirectBuffer,
				.size = sizeof(IndirectCommand),
				.stride = sizeof(IndirectCommand)
			},
			.addressFlags = VK_ADDRESS_COMMAND_FULLY_BOUND_BIT_KHR,
			.drawCount = 1
		});
	}

	void CommandBuffer::dispatch(u32 groupsX, u32 groupsY, u32 groupsZ) {
		vkCmdDispatch(m_cmd, groupsX, groupsY, groupsZ);
	}

	void CommandBuffer::dispatchIndirect(DeviceAddress indirectBuffer) {
		vkCmdDispatchIndirect2KHR(m_cmd, &VkDispatchIndirect2InfoKHR{
			.addressRange{
				.address = indirectBuffer,
				.size = sizeof(IndirectCommand)
			},
			.addressFlags = VK_ADDRESS_COMMAND_FULLY_BOUND_BIT_KHR,
		});
	}

	void CommandBuffer::endRenderPass() {
		vkCmdEndRendering(m_cmd);
	}

	void CommandBuffer::setDepthStencilState(DepthStencilState state) {
		vkCmdSetDepthTestEnable(m_cmd, state.depthTestEnable);
		if(state.depthTestEnable){
			vkCmdSetDepthWriteEnable(m_cmd, state.depthWriteEnable);
			vkCmdSetDepthCompareOp(m_cmd, vkCompareOp(state.depthCompareOp));
		}

		vkCmdSetStencilTestEnable(m_cmd, state.stencilTestEnable);
		if(state.stencilTestEnable) {
			vkCmdSetStencilOp(m_cmd, VK_STENCIL_FACE_FRONT_BIT, vkStencilOp(state.front.failOp), vkStencilOp(state.front.depthFailOp), vkStencilOp(state.front.passOp), vkCompareOp(state.front.compareOp));
			vkCmdSetStencilCompareMask(m_cmd, VK_STENCIL_FACE_FRONT_BIT, state.front.compareMask);
			vkCmdSetStencilWriteMask(m_cmd, VK_STENCIL_FACE_FRONT_BIT, state.front.writeMask);
			vkCmdSetStencilReference(m_cmd, VK_STENCIL_FACE_FRONT_BIT, state.front.reference);

			vkCmdSetStencilOp(m_cmd, VK_STENCIL_FACE_BACK_BIT, vkStencilOp(state.back.failOp), vkStencilOp(state.back.depthFailOp), vkStencilOp(state.back.passOp), vkCompareOp(state.back.compareOp));
			vkCmdSetStencilCompareMask(m_cmd, VK_STENCIL_FACE_BACK_BIT, state.back.compareMask);
			vkCmdSetStencilWriteMask(m_cmd, VK_STENCIL_FACE_BACK_BIT, state.back.writeMask);
			vkCmdSetStencilReference(m_cmd, VK_STENCIL_FACE_BACK_BIT, state.back.reference);
		}

		vkCmdSetDepthBoundsTestEnable(m_cmd, state.depthBoundsTestEnable);
		if(state.depthBoundsTestEnable) {
			vkCmdSetDepthBounds(m_cmd, state.minDepthBounds, state.maxDepthBounds);
		}
	}
	
	void CommandBuffer::setAttachmentColorState(AttachmentColorState state) {
		u32 blendEnable = state.blendEnable;
		vkCmdSetColorBlendEnableEXT(m_cmd, state.attachmentIndex, 1, &blendEnable);
		if(state.blendEnable) {
			vkCmdSetColorBlendEquationEXT(m_cmd, state.attachmentIndex, 1, &VkColorBlendEquationEXT{
				.srcColorBlendFactor = vkBlendFactor(state.colorBlend.srcFactor),
				.dstColorBlendFactor = vkBlendFactor(state.colorBlend.dstFactor),
				.colorBlendOp = vkBlendOp(state.colorBlend.blendOp),
				.srcAlphaBlendFactor = vkBlendFactor(state.alphaBlend.srcFactor),
				.dstAlphaBlendFactor = vkBlendFactor(state.alphaBlend.dstFactor),
				.alphaBlendOp = vkBlendOp(state.alphaBlend.blendOp)
			});
		}

		VkColorComponentFlags writeMask = vkWriteMask(state.writeMask);
		vkCmdSetColorWriteMaskEXT(m_cmd, state.attachmentIndex, 1, &writeMask);
	}

	void CommandBuffer::setColorState(ColorState state) {
		vkCmdSetLogicOpEnableEXT(m_cmd, state.logicOpEnable);
		if(state.logicOpEnable) {
			vkCmdSetLogicOpEXT(m_cmd, vkLogicOp(state.logicOp));
		}
		else {
			vkCmdSetBlendConstants(m_cmd, state.blendConstants);
		}
	}

	void CommandBuffer::setMultisampleState(MultisampleState state) {
		u32 sampleMask = state.sampleMask;
		vkCmdSetRasterizationSamplesEXT(m_cmd, vkSampleCount(state.sampleCount));
		vkCmdSetSampleMaskEXT(m_cmd, vkSampleCount(state.sampleCount), &sampleMask);
		vkCmdSetAlphaToCoverageEnableEXT(m_cmd, state.alphaToCoverageEnable);
		vkCmdSetAlphaToOneEnableEXT(m_cmd, state.alphaToOneEnable);
	}

	void CommandBuffer::setRasterizerState(RasterizerState state) {
		vkCmdSetDepthClampEnableEXT(m_cmd, state.depthClampEnable);
		vkCmdSetRasterizerDiscardEnable(m_cmd, state.rasterizerDiscardEnable);
		vkCmdSetPolygonModeEXT(m_cmd, vkPolygonMode(state.polygonMode));
		vkCmdSetCullMode(m_cmd, vkCullMode(state.cullMode));
		vkCmdSetFrontFace(m_cmd, vkFrontFace(state.windingOrder));

		vkCmdSetDepthBiasEnable(m_cmd, state.depthBiasEnable);
		if(state.depthBiasEnable) {
			vkCmdSetDepthBias(m_cmd, state.depthBiasConstantFactor, state.depthBiasClamp, state.depthBiasSlopeFactor);
		}

		vkCmdSetLineWidth(m_cmd, state.lineWidth);
	}

	void CommandBuffer::setScissor(Rect2D<u32> scissor) {
		vkCmdSetScissorWithCount(m_cmd, 1, &VkRect2D{ static_cast<i32>(scissor.x), static_cast<i32>(scissor.y), scissor.width, scissor.height });
	}

	void CommandBuffer::setViewport(Rect3D<f32> viewport) {
		vkCmdSetViewportWithCount(m_cmd, 1, &VkViewport{ viewport.x, viewport.y, viewport.width, viewport.height, viewport.z, viewport.z + viewport.depth });
	}

	void CommandBuffer::unbindShaders(View<ShaderStage> stages) {
		VkShaderStageFlagBits stageBits[3];
		for(u8 i = 0; i < stages.count(); i++) {
			stageBits[i] = vkShaderStage(stages[i]);
		}

		vkCmdBindShadersEXT(m_cmd, stages.count(), stageBits, nullptr);
	}

	VkCommandBuffer CommandBuffer::vkCommandBuffer() const {
		return m_cmd;
	}

	tbrs::Vec<StagingBuffer>&& CommandBuffer::getStagingBuffers() const {
		return std::move(m_stagingBuffers);
	}

	CommandBuffer::CommandBuffer(VkCommandBuffer cmd, StagingAllocator* stagingAllocator)
		: m_cmd(cmd), m_allocator(stagingAllocator) {
	}

	void CommandBuffer::pushConstantsImpl(const void* constants, u64 size) {
		vkCmdPushDataEXT(m_cmd, &VkPushDataInfoEXT{
			.offset = 0,
			.data{
				.address = constants,
				.size = size
			}
		});
	}

	void CommandBuffer::writeBufferImpl(DeviceAddress address, const void* data, u64 size) {
		if(size < 65536) {
			vkCmdUpdateMemoryKHR(m_cmd, &VkDeviceAddressRangeKHR{ address, size }, VK_ADDRESS_COMMAND_FULLY_BOUND_BIT_KHR, size, data);
		}
		else {
			if(m_stagingBuffers.empty() || m_stagingBuffers.back().size - m_stagingBuffers.back().writeOffset < size) {
				m_stagingBuffers.push(m_allocator->alloc(size));
			}
			StagingBuffer& stagingBuffer = m_stagingBuffers.back();

			memcpy(static_cast<byte*>(stagingBuffer.cpuPtr) + stagingBuffer.writeOffset, data, size);
			vkCmdCopyMemoryKHR(m_cmd, &VkCopyDeviceMemoryInfoKHR{
				.regionCount = 1,
				.pRegions = &VkDeviceMemoryCopyKHR{
					.srcRange{
						.address = stagingBuffer.gpuPtr + stagingBuffer.writeOffset,
						.size = size
					},
					.srcFlags = VK_ADDRESS_COMMAND_FULLY_BOUND_BIT_KHR,
					.dstRange{
						.address = address,
						.size = size
					},
					.dstFlags = VK_ADDRESS_COMMAND_FULLY_BOUND_BIT_KHR
				}
			});

			stagingBuffer.writeOffset += size;
		}
	}

	void CommandBuffer::writeTextureImpl(const Texture& texture, const void* data, TextureRegion region) {
		u32 maxLevel = region.numLevels == TextureRegion::RemainingLevels ? texture.vkImageViewCreateInfo().subresourceRange.levelCount : region.baseLevel + region.numLevels;
		for(u32 level = region.baseLevel; level < maxLevel; level++) {
			u32 width = std::max(texture.extent().x >> level, 1u);
			u32 height = std::max(texture.extent().y >> level, 1u);
			u32 depth = std::max(texture.extent().z >> level, 1u);
			u64 levelSize = width * height * depth * vkFormatTexelBlockSize(texture.vkImageViewCreateInfo().format);

			if(m_stagingBuffers.empty() || m_stagingBuffers.back().size - m_stagingBuffers.back().writeOffset < levelSize) {
				m_stagingBuffers.push(m_allocator->alloc(levelSize));
			}
			StagingBuffer& stagingBuffer = m_stagingBuffers.back();

			memcpy(static_cast<byte*>(stagingBuffer.cpuPtr) + stagingBuffer.writeOffset, data, levelSize);

			// foo: batch VkBufferImageCopys while staging buffer has room
			vkCmdCopyMemoryToImageKHR(m_cmd, &VkCopyDeviceMemoryImageInfoKHR{
				.image = texture.vkImage(),
				.regionCount = 1,
				.pRegions = &VkDeviceMemoryImageCopyKHR{
					.addressRange{
						.address = stagingBuffer.gpuPtr + stagingBuffer.writeOffset,
						.size = levelSize
					},
					.addressFlags = VK_ADDRESS_COMMAND_FULLY_BOUND_BIT_KHR,
					.imageSubresource = {
						.aspectMask = region.aspect == DepthStencilAspect::Default ? vkAspectMask(texture.vkImageViewCreateInfo().format) : vkAspectMask(region.aspect),
						.mipLevel = level,
						.baseArrayLayer = region.baseLayer,
						.layerCount = region.numLayers
					},
					.imageLayout = VK_IMAGE_LAYOUT_GENERAL,
					.imageExtent = {
						width,
						height,
						depth
					}
				}
			});

			stagingBuffer.writeOffset += levelSize;
			data = static_cast<const byte*>(data) + levelSize;
		}
	}
};