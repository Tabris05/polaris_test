#pragma once

#include <tabris/types.hpp>
#include <polaris/pod_types.hpp>
#include <volk/volk.h>
#include "texture_impl.hpp"
#include "shader_impl.hpp"
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
			void bindShaders(View<std::reference_wrapper<const Shader>> shaders);
			void clearBuffer(BufferRange range, u32 value);
			void clearTexture(const Texture& texture, ClearValue value, TextureRegion region = {});
			void copyTexture(const Texture& src, const Texture& dst, TextureRegion srcRegion = {}, TextureRegion dstRegion = {});
			void draw(u32 groupsX = 1, u32 groupsY = 1, u32 groupsZ = 1);
			void drawIndirect(DeviceAddress indirectBuffer);
			void dispatch(u32 groupsX = 1, u32 groupsY = 1, u32 groupsZ = 1);
			void dispatchIndirect(DeviceAddress indirectBuffer);
			void endRenderPass();
			void pushConstants(const auto& constants, u16 offset = 0) {
				static_assert(sizeof(constants) <= 256, "Push constants must be 256 bytes or less.");
				pushConstantsImpl(&constants, sizeof(constants), offset);
			}
			void setAttachmentColorState(AttachmentColorState state);
			void setColorState(ColorState state);
			void setDepthStencilState(DepthStencilState state);
			void setMultisampleState(MultisampleState state);
			void setRasterizerState(RasterizerState state);
			void setScissor(Rect2D<u32> scissor);
			void setViewport(Rect3D<f32> viewport);
			void unbindShaders(View<ShaderStage> stages);

			CommandBuffer(CommandBuffer&&) = default;
			CommandBuffer& operator=(CommandBuffer&&) = default;

			CommandBuffer(const CommandBuffer&) = delete;
			CommandBuffer& operator=(const CommandBuffer&) = delete;

			// "public" functions that should not be included in the public header
			VkCommandBuffer vkCommandBuffer() const;
			tbrs::Vec<StagingBuffer>&& getStagingBuffers() const;
			CommandBuffer(VkCommandBuffer cmd, StagingAllocator* stagingAllocator);

		private:
			void pushConstantsImpl(const void* constants, u16 size, u16 offset);
			void writeBufferImpl(DeviceAddress address, const void* data, u64 size);
			void writeTextureImpl(const Texture& texture, const void* data, TextureRegion region);

			VkCommandBuffer m_cmd = {};
			StagingAllocator* m_allocator = nullptr;

			// mutable is evil but you can't move from const objects and I need to steal this in Queue::Submit
			mutable tbrs::Vec<StagingBuffer> m_stagingBuffers;
	};
}