#pragma once

#include <polaris/pod_types.hpp>
#include <volk/volk.h>
#include <tabris/types.hpp>
#include "texture_impl.hpp"
#include "pipeline_impl.hpp"

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

			CommandBuffer(const CommandBuffer&) = delete;
			CommandBuffer& operator=(const CommandBuffer&) = delete;
			CommandBuffer(CommandBuffer&&) = default;
			CommandBuffer& operator=(CommandBuffer&&) = default;

			// "public" functions that should not be included in the public header
			VkCommandBuffer vkCommandBuffer() const;
			CommandBuffer(VkCommandBuffer cmd, VkPipelineLayout layout);
		private:
			void pushConstantsImpl(const void* constants, u64 size);

			VkCommandBuffer m_cmd;
			VkPipelineLayout m_layout;
	};
}