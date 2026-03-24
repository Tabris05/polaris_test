#include "queue_impl.hpp"
#include "device_impl.hpp"
#include "vk_util.hpp"

namespace pl {
	CommandBuffer Queue::beginRecording() {
		u64 i;
		for(i = 0; i < m_submittedCmds.count() && m_submittedCmds[i].event.completed(); i++) {
			for(StagingBuffer buffer : m_submittedCmds[i].stagingBuffers) {
				m_allocator->free(buffer);
			};
			m_freeCmds.push(m_submittedCmds[i].cmd);
			vkResetCommandBuffer(m_freeCmds.back(), 0);
		}
		m_submittedCmds.remove(0, i); // foo: should recycle stagingBuffers vector

		VkCommandBuffer cmd;
		if(m_freeCmds.empty()) {
			vkAllocateCommandBuffers(m_device, &VkCommandBufferAllocateInfo{
				.commandPool = m_pool,
				.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
				.commandBufferCount = 1
			}, &cmd);
		}
		else {
			cmd = m_freeCmds.back();
			m_freeCmds.pop();
		}

		vkBeginCommandBuffer(cmd, &VkCommandBufferBeginInfo{ .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT });

		if(m_type != QueueType::DMA) {
			m_heap->bind(cmd);
		}
		if(m_type == QueueType::Universal) {

			// viewport state
			vkCmdSetViewportWithCount(cmd, 1, &VkViewport{ .width = 1.0f });
			vkCmdSetScissorWithCount(cmd, 1, &VkRect2D{});

			// rasterization state
			vkCmdSetRasterizerDiscardEnable(cmd, false);
			vkCmdSetPolygonModeEXT(cmd, VK_POLYGON_MODE_FILL);
			vkCmdSetCullMode(cmd, VK_CULL_MODE_NONE);
			vkCmdSetDepthBiasEnable(cmd, false);

			// multisample state
			vkCmdSetRasterizationSamplesEXT(cmd, VK_SAMPLE_COUNT_1_BIT);
			vkCmdSetSampleMaskEXT(cmd, VK_SAMPLE_COUNT_1_BIT, nullptr);
			vkCmdSetAlphaToCoverageEnableEXT(cmd, false);

			// depth-stencil state
			vkCmdSetDepthTestEnable(cmd, false);
			vkCmdSetStencilTestEnable(cmd, false);
			
			// blend state
			VkBool32 bools[8] = {};
			vkCmdSetColorBlendEnableEXT(cmd, 0, 8, bools);

			VkColorComponentFlags colorMasks[8] = { 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF };
			vkCmdSetColorWriteMaskEXT(cmd, 0, 8, colorMasks);

			// VUID-vkCmdDrawMeshTasksEXT-None-08684 ????
			VkShaderStageFlagBits stages = VK_SHADER_STAGE_VERTEX_BIT;
			vkCmdBindShadersEXT(cmd, 1, &stages, nullptr);
		}

		return CommandBuffer(cmd, m_allocator);
	}

	const Event Queue::submit(const SubmitInfo& si) {
		VkCommandBuffer cmd = si.commandBuffer.vkCommandBuffer();
		vkEndCommandBuffer(cmd);

		tbrs::Vec<VkSemaphoreSubmitInfo> signalSemaphors = {
			VkSemaphoreSubmitInfo{
				.semaphore = m_sync.advance().vkSemaphore(),
				.value = m_sync.event().value(),
				.stageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT
			}
		};

		if(si.signalEvent.has_value()) {
			signalSemaphors.push(VkSemaphoreSubmitInfo{
				.semaphore = si.signalEvent->event.vkSemaphore(),
				.value = si.signalEvent->event.value(),
				.stageMask = vkStageMask(si.signalEvent->stage)
			});
		}

		vkQueueSubmit2(m_queue, 1, &VkSubmitInfo2{
			.waitSemaphoreInfoCount = si.waitEvent.has_value() ? 1u : 0u,
			.pWaitSemaphoreInfos = si.waitEvent.has_value() ? &VkSemaphoreSubmitInfo{
				.semaphore = si.waitEvent->event.vkSemaphore(),
				.value = si.waitEvent->event.value(),
				.stageMask = vkStageMask(si.waitEvent->stage)
			} : nullptr,
			.commandBufferInfoCount = 1,
			.pCommandBufferInfos = &VkCommandBufferSubmitInfo{ .commandBuffer = cmd },
			.signalSemaphoreInfoCount = static_cast<u32>(signalSemaphors.count()),
			.pSignalSemaphoreInfos = signalSemaphors.data()
		}, {});

		m_submittedCmds.push(cmd, m_sync.event(), si.commandBuffer.getStagingBuffers());
		return m_sync.event();
	}

	const Event Queue::submit(CommandBuffer commandBuffer, std::optional<const Event> waitEvent, VkSemaphore waitSem, VkSemaphore signalSem) {
		VkCommandBuffer cmd = commandBuffer.vkCommandBuffer();
		vkEndCommandBuffer(cmd);

		tbrs::Vec<VkSemaphoreSubmitInfo> waitSemaphores = {
			VkSemaphoreSubmitInfo{
				.semaphore = waitSem,
				.stageMask = VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT
			}
		};

		if(waitEvent.has_value()) {
			waitSemaphores.push(VkSemaphoreSubmitInfo{
				.semaphore = waitEvent->vkSemaphore(),
				.value = waitEvent->value(),
				.stageMask = VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT
			});
		}

		tbrs::Vec<VkSemaphoreSubmitInfo> signalSemaphores = {
			VkSemaphoreSubmitInfo{
				.semaphore = signalSem,
				.stageMask = VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT
			},
			VkSemaphoreSubmitInfo{
				.semaphore = m_sync.advance().vkSemaphore(),
				.value = m_sync.event().value(),
				.stageMask = VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT
			}
		};

		vkQueueSubmit2(m_queue, 1, &VkSubmitInfo2{
			.waitSemaphoreInfoCount = static_cast<u32>(waitSemaphores.count()),
			.pWaitSemaphoreInfos = waitSemaphores.data(),
			.commandBufferInfoCount = 1,
			.pCommandBufferInfos = &VkCommandBufferSubmitInfo{
				.commandBuffer = cmd
			},
			.signalSemaphoreInfoCount = static_cast<u32>(signalSemaphores.count()),
			.pSignalSemaphoreInfos = signalSemaphores.data()
		}, {});

		m_submittedCmds.push(cmd, m_sync.event());
		return m_sync.event();
	}

	VkQueue Queue::vkQueue() const {
		return m_queue;
	}

	Queue::Queue(const QueueCreateInfo& ci)
		: m_device(ci.device.vkDevice()), m_queue(ci.device.vkQueue(ci.type)), m_type(ci.type), m_sync({ .device = ci.device }), m_heap(ci.device.descriptorHeap()),
		m_allocator(new StagingAllocator(ci.device.vkPhysicalDevice(), ci.device.vkDevice())) {
		vkCreateCommandPool(m_device, &VkCommandPoolCreateInfo{
				.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
				.queueFamilyIndex = ci.device.vkQueueFamily(ci.type)
		}, nullptr, &m_pool);
	}

	Queue::Queue(Queue&& src) {
		memcpy(this, &src, sizeof(Queue));
		memset(&src, 0, sizeof(Queue));
	}

	Queue& Queue::operator=(Queue&& src) {
		this->~Queue();
		new (this) Queue(std::move(src));

		return *this;
	}

	Queue::~Queue() {
		if(m_device) {
			m_sync.event().wait();
			for(const Submission& submission : m_submittedCmds) {
				for(const StagingBuffer& buffer : submission.stagingBuffers) {
					m_allocator->free(buffer);
				}
			}
			vkDestroyCommandPool(m_device, m_pool, nullptr);
			delete m_allocator;
		}
	}
}
