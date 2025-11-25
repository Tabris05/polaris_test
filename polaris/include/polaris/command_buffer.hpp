#pragma once

#include "pod_types.hpp"
#include "texture.hpp"
#include "pipeline.hpp"

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
			void beginRenderPass(const RenderPassBeginInfo& info);
			void bindPipeline(const Pipeline& pipeline);
			void clearTexture(const Texture& tex, ClearValue value);
			void draw(u32 vertexCount, u32 instanceCount = 1, u32 firstVertex = 0, u32 firstInstance = 0);
			void endRenderPass();
			void pushConstants(const auto& constants) {
				static_assert(sizeof(constants) <= 256, "Push constants must be 256 bytes or less.");
				pushConstantsImpl(&constants, sizeof(constants));
			}
			void setViewport(Rect<f32> viewport);
			void setScissor(Rect<u32> scissor);

			CommandBuffer() = delete;
			CommandBuffer(const CommandBuffer&) = delete;
			CommandBuffer& operator=(const CommandBuffer&) = delete;
			CommandBuffer(CommandBuffer&&) = default;
			CommandBuffer& operator=(CommandBuffer&&) = default;

		private:
			void pushConstantsImpl(const void* constants, u64 size);

			alignas(8) byte reserved[16];
	};
}
