#include "command_buffer_impl.hpp"
#include <tabris/types.hpp>
#include "vk_util.hpp"

namespace pl {
	void CommandBuffer::barrier(PipelineStage src, PipelineStage dst) {
		vkCmdPipelineBarrier2(m_cmd, ptr(VkDependencyInfo{
			.memoryBarrierCount = 1,
			.pMemoryBarriers = ptr(VkMemoryBarrier2{
				.srcStageMask = vkStageMask(src),
				.srcAccessMask = vkAccessMask(src),
				.dstStageMask = vkStageMask(dst),
				.dstAccessMask = vkAccessMask(dst)
			})
		}));
	}

	void CommandBuffer::beginRenderPass(const RenderPassBeginInfo& info) {
		VkRenderingAttachmentInfo colorAttachments[8];
		for(u8 i = 0; i < info.colorTargets.count(); i++) {
			colorAttachments[i] = VkRenderingAttachmentInfo{
				.imageView = info.colorTargets[i].handle.view,
				.imageLayout = VK_IMAGE_LAYOUT_GENERAL,
				.loadOp = vkLoadOp(info.colorTargets[i].loadOp),
				.storeOp = vkStoreOp(info.colorTargets[i].storeOp),
				.clearValue = std::bit_cast<VkClearValue>(info.colorTargets[i].clearValue)
			};
		}

		VkRenderingAttachmentInfo depthAttachment;
		if(info.depthTarget.has_value()) {
			depthAttachment = VkRenderingAttachmentInfo{
				.imageView = info.depthTarget.value().handle.view,
				.imageLayout = VK_IMAGE_LAYOUT_GENERAL,
				.loadOp = vkLoadOp(info.depthTarget.value().loadOp),
				.storeOp = vkStoreOp(info.depthTarget.value().storeOp),
				.clearValue = std::bit_cast<VkClearValue>(info.depthTarget.value().clearValue)
			};
		}

		VkRenderingAttachmentInfo stencilAttachment;
		if(info.stencilTarget.has_value()) {
			stencilAttachment = VkRenderingAttachmentInfo{
				.imageView = info.stencilTarget.value().handle.view,
				.imageLayout = VK_IMAGE_LAYOUT_GENERAL,
				.loadOp = vkLoadOp(info.stencilTarget.value().loadOp),
				.storeOp = vkStoreOp(info.stencilTarget.value().storeOp),
				.clearValue = std::bit_cast<VkClearValue>(info.stencilTarget.value().clearValue)
			};
		}

		vkCmdBeginRendering(m_cmd, ptr(VkRenderingInfo{
			.renderArea = { static_cast<i32>(info.renderArea.x), static_cast<i32>(info.renderArea.y), info.renderArea.width, info.renderArea.height },
			.layerCount = 1,
			.colorAttachmentCount = static_cast<u32>(info.colorTargets.count()),
			.pColorAttachments = colorAttachments,
			.pDepthAttachment = info.depthTarget.has_value() ? &depthAttachment : nullptr,
			.pStencilAttachment = info.stencilTarget.has_value() ? &stencilAttachment : nullptr
			}));
	}

	void CommandBuffer::bindPipeline(const Pipeline& pipeline) {
		pipeline.bind(m_cmd);
	}

	void CommandBuffer::clearTexture(const Texture& tex, ClearValue value) {
		if(tex.vkImageViewCreateInfo().subresourceRange.aspectMask & VK_IMAGE_ASPECT_COLOR_BIT) {
			vkCmdClearColorImage(m_cmd, tex.vkImage(), VK_IMAGE_LAYOUT_GENERAL, reinterpret_cast<VkClearColorValue*>(&value), 1, ptr(tex.vkImageViewCreateInfo().subresourceRange));
		}
		else {
			vkCmdClearDepthStencilImage(m_cmd, tex.vkImage(), VK_IMAGE_LAYOUT_GENERAL, reinterpret_cast<VkClearDepthStencilValue*>(&value), 1, ptr(tex.vkImageViewCreateInfo().subresourceRange));
		}
	}

	void CommandBuffer::draw(u32 vertexCount, u32 instanceCount, u32 firstVertex, u32 firstInstance) {
		vkCmdDraw(m_cmd, vertexCount, instanceCount, firstVertex, firstInstance);
	}

	void CommandBuffer::dispatch(u32 groupsX, u32 groupsY, u32 groupsZ) {
		vkCmdDispatch(m_cmd, groupsX, groupsY, groupsZ);
	}

	void CommandBuffer::endRenderPass() {
		vkCmdEndRendering(m_cmd);
	}

	void CommandBuffer::setViewport(Rect<f32> viewport) {
		vkCmdSetViewport(m_cmd, 0, 1, ptr(VkViewport{ viewport.x, viewport.y, viewport.width, viewport.height }));
	}

	void CommandBuffer::setScissor(Rect<u32> scissor) {
		vkCmdSetScissor(m_cmd, 0, 1, ptr(VkRect2D{ static_cast<i32>(scissor.x), static_cast<i32>(scissor.y), scissor.width, scissor.height }));
	}

	VkCommandBuffer CommandBuffer::vkCommandBuffer() const {
		return m_cmd;
	}

	tbrs::Vec<StagingBuffer>&& CommandBuffer::getStagingBuffers() const {
		return std::move(m_stagingBuffers);
	}

	CommandBuffer::CommandBuffer(VkCommandBuffer cmd, VkPipelineLayout layout, StagingAllocator* stagingAllocator)
		: m_cmd(cmd), m_layout(layout), m_allocator(stagingAllocator) {
	}

	void CommandBuffer::pushConstantsImpl(const void* constants, u64 size) {
		vkCmdPushConstants(m_cmd, m_layout, VK_SHADER_STAGE_ALL, 0, size, constants);
	}

	void CommandBuffer::writeBufferImpl(const Buffer& buffer, const void* data, u64 size, u64 offset) {
		if(size < 65536) {
			vkCmdUpdateBuffer(m_cmd, buffer.vkBuffer(), offset, size, data);
		}
		else {
			u64 writtenSize = 0;
			while(writtenSize < size) {
				if(m_stagingBuffers.empty() || m_stagingBuffers.back().writeOffset == StagingAllocator::PageSize) {
					m_stagingBuffers.push(m_allocator->alloc());
				}
				StagingBuffer stagingBuffer = m_stagingBuffers.back();

				u64 copySize = std::min(size - writtenSize, StagingAllocator::PageSize - stagingBuffer.writeOffset);
				memcpy(static_cast<byte*>(stagingBuffer.mappedPtr) + stagingBuffer.writeOffset, static_cast<const byte*>(data) + writtenSize, copySize);
				vkCmdCopyBuffer(m_cmd, stagingBuffer.buffer, buffer.vkBuffer(), 1, ptr(VkBufferCopy{
					.srcOffset = stagingBuffer.writeOffset,
					.dstOffset = offset + writtenSize,
					.size = copySize
				}));

				stagingBuffer.writeOffset += copySize;
				writtenSize += copySize;
			}
		}
	}

	void CommandBuffer::writeTextureImpl(const Texture& texture, const void* data, TextureRegion region) {

	}
};