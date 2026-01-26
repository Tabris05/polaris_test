#pragma once

#include <tabris/types.hpp>
#include <polaris/pod_types.hpp>
#include <volk/volk.h>
#include "texture_impl.hpp"
#include "pipeline_impl.hpp"
#include "buffer_impl.hpp"
#include "staging_allocator.hpp"

namespace pl {
	struct RenderTargetInfo {
		RenderTarget target;
		LoadOp loadOp;
		StoreOp storeOp;
		ClearValue clearValue;
	};

	struct RenderPassBeginInfo {
		Rect2D<u32> renderArea;
		View<const RenderTargetInfo> colorTargets;
		std::optional<RenderTargetInfo> depthTarget;
		std::optional<RenderTargetInfo> stencilTarget;
	};

	class CommandBuffer {
		public:
			void barrier(PipelineStage src, PipelineStage dst);
			void beginRenderPass(const RenderPassBeginInfo& info);
			void bindIndexBuffer(BufferRegion buffer, IndexType indexType);
			void bindPipeline(const Pipeline& pipeline);
			void clearTexture(const Texture& tex, ClearValue value, TextureRegion region = {});
			void draw(u32 vertexCount, u32 instanceCount = 1, u32 firstVertex = 0, u32 firstInstance = 0);
			void drawIndexed(u32 indexCount, u32 instanceCount = 1, u32 firstIndex = 0, i32 vertexOffset = 0, u32 firstInstance = 0);
			void drawIndexedIndirect(BufferRegion indirectBuffer, u32 drawCount, u32 stride = 0);
			void drawIndexedIndirectCount(BufferRegion indirectBuffer, BufferRegion countBuffer, u32 maxCount, u32 stride = 0);
			void drawIndirect(BufferRegion indirectBuffer, u32 drawCount, u32 stride = 0);
			void drawIndirectCount(BufferRegion indirectBuffer, BufferRegion countBuffer, u32 maxCount, u32 stride = 0);
			void dispatch(u32 groupsX = 1, u32 groupsY = 1, u32 groupsZ = 1);
			void endRenderPass();
			void pushConstants(const auto& constants) {
				static_assert(sizeof(constants) <= 256, "Push constants must be 256 bytes or less.");
				pushConstantsImpl(&constants, sizeof(constants));
			}
			void setViewport(Rect3D<f32> viewport);
			void setScissor(Rect2D<u32> scissor);

			CommandBuffer(CommandBuffer&&) = default;
			CommandBuffer& operator=(CommandBuffer&&) = default;

			CommandBuffer(const CommandBuffer&) = delete;
			CommandBuffer& operator=(const CommandBuffer&) = delete;

			// "public" functions that should not be included in the public header
			VkCommandBuffer vkCommandBuffer() const;
			tbrs::Vec<StagingBuffer>&& getStagingBuffers() const;
			CommandBuffer(VkCommandBuffer cmd, VkPipelineLayout layout, StagingAllocator* stagingAllocator);

		private:
			void pushConstantsImpl(const void* constants, u64 size);
			void writeBufferImpl(const Buffer& buffer, const void* data, u64 size, u64 offset);
			void writeTextureImpl(const Texture& texture, const void* data, TextureRegion region);

			VkCommandBuffer m_cmd = {};
			VkPipelineLayout m_layout = {};
			StagingAllocator* m_allocator = nullptr;

			// mutable is evil but you can't move from const objects and I need to steal this in Queue::Submit
			mutable tbrs::Vec<StagingBuffer> m_stagingBuffers;
	};
}