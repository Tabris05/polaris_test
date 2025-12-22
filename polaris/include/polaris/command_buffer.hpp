#pragma once

#include "pod_types.hpp"
#include "texture.hpp"
#include "pipeline.hpp"
#include "buffer.hpp"

namespace pl {
	struct RenderTargetInfo {
		RenderTargetHandle handle;
		LoadOp loadOp;
		StoreOp storeOp;
		ClearValue clearValue;
	};

	struct RenderPassBeginInfo {
		Rect<u32> renderArea;
		View<const RenderTargetInfo> colorTargets;
		std::optional<RenderTargetInfo> depthTarget;
		std::optional<RenderTargetInfo> stencilTarget;
	};

	class CommandBuffer {
		public:
			void barrier(PipelineStage src, PipelineStage dst);
			void beginRenderPass(const RenderPassBeginInfo& info);
			void bindPipeline(const Pipeline& pipeline);
			void clearTexture(const Texture& tex, ClearValue value); // foo: needs TextureRegion
			void draw(u32 vertexCount, u32 instanceCount = 1, u32 firstVertex = 0, u32 firstInstance = 0);
			void dispatch(u32 groupsX = 1, u32 groupsY = 1, u32 groupsZ = 1);
			void endRenderPass();
			template<typename T> void pushConstants(const T& constants) {
				static_assert(sizeof(T) <= 256, "Push constants must be 256 bytes or less.");
				pushConstantsImpl(&constants, sizeof(T));
			}
			void setViewport(Rect<f32> viewport);
			void setScissor(Rect<u32> scissor);
			template<typename T> void writeBuffer(const Buffer& buffer, View<const T> data, u64 offset = 0) {
				writeBufferImpl(buffer, data.data(), data.size(), offset);
			}
			template<typename T> void writeTexture(const Texture& texture, View<const T> data, TextureRegion region) {
				writeTextureImpl(texture, data.data(), region);
			}

			CommandBuffer(CommandBuffer&&) = default;
			CommandBuffer& operator=(CommandBuffer&&) = default;

			CommandBuffer() = delete;
			CommandBuffer(const CommandBuffer&) = delete;
			CommandBuffer& operator=(const CommandBuffer&) = delete;

		private:
			void pushConstantsImpl(const void* constants, u64 size);
			void writeBufferImpl(const Buffer& buffer, const void* data, u64 size, u64 offset);
			void writeTextureImpl(const Texture& texture, const void* data, TextureRegion region);

			alignas(8) byte reserved[48];
	};
}
