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

	void CommandBuffer::clearBuffer(BufferOffset offset, u32 value, u64 size) {
		vkCmdFillBuffer(m_cmd, offset.buffer.vkBuffer(), offset.offset, size, value);
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

	void CommandBuffer::drawIndirect(BufferOffset indirectBuffer) {
		vkCmdDrawMeshTasksIndirectEXT(m_cmd, indirectBuffer.buffer.vkBuffer(), indirectBuffer.offset, 1, sizeof(IndirectCommand));
	}

	void CommandBuffer::dispatch(u32 groupsX, u32 groupsY, u32 groupsZ) {
		vkCmdDispatch(m_cmd, groupsX, groupsY, groupsZ);
	}

	void CommandBuffer::dispatchIndirect(BufferOffset indirectBuffer) {
		vkCmdDispatchIndirect(m_cmd, indirectBuffer.buffer.vkBuffer(), indirectBuffer.offset);
	}

	void CommandBuffer::endRenderPass() {
		vkCmdEndRendering(m_cmd);
	}

	void CommandBuffer::setDepthStencilState(DepthStencilState state) {
		vkCmdSetDepthTestEnable(m_cmd, state.depthTestEnable);
		vkCmdSetDepthWriteEnable(m_cmd, state.depthWriteEnable);
		vkCmdSetDepthCompareOp(m_cmd, vkCompareOp(state.depthCompareOp));
	}
	
	void CommandBuffer::setRasterizerState(RasterizerState state) {
		vkCmdSetCullMode(m_cmd, vkCullMode(state.cullMode));
		vkCmdSetFrontFace(m_cmd, vkFrontFace(state.windingOrder));
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

	void CommandBuffer::writeBufferImpl(BufferOffset offset, const void* data, u64 size) {
		if(size < 65536) {
			vkCmdUpdateBuffer(m_cmd, offset.buffer.vkBuffer(), offset.offset, size, data);
		}
		else {
			if(m_stagingBuffers.empty() || m_stagingBuffers.back().size - m_stagingBuffers.back().writeOffset < size) {
				m_stagingBuffers.push(m_allocator->alloc(size));
			}
			StagingBuffer& stagingBuffer = m_stagingBuffers.back();

			memcpy(static_cast<byte*>(stagingBuffer.mappedPtr) + stagingBuffer.writeOffset, data, size);
			vkCmdCopyBuffer(m_cmd, stagingBuffer.buffer, offset.buffer.vkBuffer(), 1, &VkBufferCopy{
				.srcOffset = stagingBuffer.writeOffset,
				.dstOffset = offset.offset,
				.size = size
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

			memcpy(static_cast<byte*>(stagingBuffer.mappedPtr) + stagingBuffer.writeOffset, data, levelSize);

			// foo: batch VkBufferImageCopys while staging buffer has room
			vkCmdCopyBufferToImage(m_cmd, stagingBuffer.buffer, texture.vkImage(), VK_IMAGE_LAYOUT_GENERAL, 1, &VkBufferImageCopy{
				.bufferOffset = stagingBuffer.writeOffset,
				.imageSubresource = {
					.aspectMask = region.aspect == DepthStencilAspect::Default ? vkAspectMask(texture.vkImageViewCreateInfo().format) : vkAspectMask(region.aspect),
					.mipLevel = level,
					.baseArrayLayer = region.baseLayer,
					.layerCount = region.numLayers
				},
				.imageExtent = {
					width,
					height,
					depth
				}
			});

			stagingBuffer.writeOffset += levelSize;
			data = static_cast<const byte*>(data) + levelSize;
		}
	}
};