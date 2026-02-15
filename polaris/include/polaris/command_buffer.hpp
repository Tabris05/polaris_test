#pragma once

#include "pod_types.hpp"
#include "texture.hpp"
#include "pipeline.hpp"
#include "buffer.hpp"

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
			void bindIndexBuffer(BufferOffset region, IndexType indexType);
			void bindPipeline(const Pipeline& pipeline);
			void clearBuffer(BufferOffset offset, u32 value, u64 size = 0xFFFFFFFFFFFFFFFF);
			void clearTexture(const Texture& texture, ClearValue value, TextureRegion region = {});
			void copyTexture(const Texture& src, const Texture& dst, TextureRegion srcRegion = {}, TextureRegion dstRegion = {});
			void draw(u32 vertexCount, u32 instanceCount = 1, u32 firstVertex = 0, u32 firstInstance = 0);
			void drawIndexed(u32 indexCount, u32 instanceCount = 1, u32 firstIndex = 0, i32 vertexOffset = 0, u32 firstInstance = 0);
			void drawIndexedIndirect(BufferOffset indirectBuffer, u32 drawCount, u32 stride = 0);
			void drawIndexedIndirectCount(BufferOffset indirectBuffer, BufferOffset countBuffer, u32 maxCount, u32 stride = 0);
			void drawIndirect(BufferOffset indirectBuffer, u32 drawCount, u32 stride = 0);
			void drawIndirectCount(BufferOffset indirectBuffer, BufferOffset countBuffer, u32 maxCount, u32 stride = 0);
			void dispatch(u32 groupsX = 1, u32 groupsY = 1, u32 groupsZ = 1);
			void endRenderPass();
			template<typename T> void pushConstants(const T& constants) {
				static_assert(sizeof(T) <= 256, "Push constants must be 256 bytes or less.");
				pushConstantsImpl(&constants, sizeof(T));
			}
			void setViewport(Rect3D<f32> viewport);
			void setScissor(Rect2D<u32> scissor);
			template<typename T> void writeBuffer(BufferOffset offset, View<const T> data) {
				writeBufferImpl(offset, data.data(), data.size());
			}
			template<typename T> void writeTexture(const Texture& texture, View<const T> data, TextureRegion region = {}) {
				writeTextureImpl(texture, data.data(), region);
			}

			CommandBuffer(CommandBuffer&&) = default;
			CommandBuffer& operator=(CommandBuffer&&) = default;

			CommandBuffer() = delete;
			CommandBuffer(const CommandBuffer&) = delete;
			CommandBuffer& operator=(const CommandBuffer&) = delete;

		private:
			void pushConstantsImpl(const void* constants, u64 size);
			void writeBufferImpl(BufferOffset offset, const void* data, u64 size);
			void writeTextureImpl(const Texture& texture, const void* data, TextureRegion region);

			alignas(8) byte reserved[48];
	};
}
