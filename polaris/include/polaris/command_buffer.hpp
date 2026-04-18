#pragma once

#include "pod_types.hpp"
#include "texture.hpp"
#include "shader.hpp"
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
			void bindShaders(View<std::reference_wrapper<const Shader>> shaders);
			void bindShaders(const Shader& shader) { bindShaders({ shader }); }
			void clearBuffer(BufferOffset offset, u32 value, u64 size = 0xFFFFFFFFFFFFFFFF);
			void clearTexture(const Texture& texture, ClearValue value, TextureRegion region = {});
			void copyTexture(const Texture& src, const Texture& dst, TextureRegion srcRegion = {}, TextureRegion dstRegion = {});
			void draw(u32 groupsX = 1, u32 groupsY = 1, u32 groupsZ = 1);
			void drawIndirect(BufferOffset indirectBuffer);
			void dispatch(u32 groupsX = 1, u32 groupsY = 1, u32 groupsZ = 1);
			void dispatchIndirect(BufferOffset indirectBuffer);
			void endRenderPass();
			template<typename T> void pushConstants(const T& constants) {
				static_assert(sizeof(T) <= 256, "Push constants must be 256 bytes or less.");
				pushConstantsImpl(&constants, sizeof(T));
			}
			void setAttachmentColorState(AttachmentColorState state);
			void setColorState(ColorState state);
			void setDepthStencilState(DepthStencilState state);
			void setMultisampleState(MultisampleState state);
			void setRasterizerState(RasterizerState state);
			void setScissor(Rect2D<u32> scissor);
			void setViewport(Rect3D<f32> viewport);
			void unbindShaders(View<ShaderStage> stages);
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

			alignas(8) byte reserved[40];
	};
}
