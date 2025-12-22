#include "queue_impl.hpp"
#include "device_impl.hpp"
#include "vk_util.hpp"

namespace pl {
	CommandBuffer Queue::beginRecording() {
		u64 newHead;
		for(newHead = 0; newHead < m_submittedCmds.count(); newHead++) {
			if(m_submittedCmds[newHead].event.completed()) {
				for(StagingBuffer buffer : m_submittedCmds[newHead].stagingBuffers) {
					m_allocator->free(buffer);
				};

				m_freeCmds.push(m_submittedCmds[newHead].cmd);
				vkResetCommandBuffer(m_freeCmds.back(), 0);
			}
			else {
				break;
			}
		}

		// really janky way to erase the first N elements
		memmove(m_submittedCmds.data(), m_submittedCmds.data() + newHead, (m_submittedCmds.count() - newHead) * sizeof(m_submittedCmds.front()));
		m_submittedCmds.setCount(m_submittedCmds.count() - newHead);

		VkCommandBuffer cmd;
		if(m_freeCmds.empty()) {
			vkAllocateCommandBuffers(m_device, ptr(VkCommandBufferAllocateInfo{
				.commandPool = m_pool,
				.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
				.commandBufferCount = 1
			}), &cmd);
		}
		else {
			cmd = m_freeCmds.back();
			m_freeCmds.pop();
		}

		vkBeginCommandBuffer(cmd, ptr(VkCommandBufferBeginInfo{ .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT }));
		m_heap->bind(cmd, m_type);

		return CommandBuffer(cmd, m_heap->vkPipelineLayout(), m_allocator);
	}

	const Event Queue::submit(const SubmitInfo& si) {
		VkCommandBuffer cmd = si.commandBuffer.vkCommandBuffer();
		vkEndCommandBuffer(cmd);

		vkQueueSubmit2(m_queue, 1, ptr(VkSubmitInfo2{
			.waitSemaphoreInfoCount = si.waitEvent.has_value() ? 1u : 0u,
			.pWaitSemaphoreInfos = si.waitEvent.has_value() ? ptr(VkSemaphoreSubmitInfo{
				.semaphore = si.waitEvent->event.vkSemaphore(),
				.value = si.waitEvent->event.value(),
				.stageMask = vkStageMask(si.waitEvent->stage)
			}) : nullptr,
			.commandBufferInfoCount = 1,
			.pCommandBufferInfos = ptr(VkCommandBufferSubmitInfo{ .commandBuffer = cmd }),
			.signalSemaphoreInfoCount = si.waitEvent.has_value() ? 2u : 1u,
			.pSignalSemaphoreInfos = ptr<VkSemaphoreSubmitInfo>({
				VkSemaphoreSubmitInfo{
					.semaphore = m_sync.next().vkSemaphore(),
					.value = m_sync.current().value(),
					.stageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT
				},
				si.signalEvent.has_value() ? VkSemaphoreSubmitInfo{
					.semaphore = si.signalEvent->event.vkSemaphore(),
					.value = si.signalEvent->event.value(),
					.stageMask = vkStageMask(si.signalEvent->stage)
				} : VkSemaphoreSubmitInfo{}
			})
		}), {});

		m_submittedCmds.push(cmd, m_sync.current(), si.commandBuffer.getStagingBuffers());
		return m_sync.current();
	}

	const Event Queue::submit(CommandBuffer commandBuffer, std::optional<const EventInfo> waitEvent, VkSemaphore waitSem, VkSemaphore signalSem) {
		VkCommandBuffer cmd = commandBuffer.vkCommandBuffer();
		vkEndCommandBuffer(cmd);

		{
			std::scoped_lock lock{ *m_submissionLock };
			vkQueueSubmit2(m_queue, 1, ptr(VkSubmitInfo2{
				.waitSemaphoreInfoCount = waitEvent.has_value() ? 2u : 1u,
				.pWaitSemaphoreInfos = ptr<VkSemaphoreSubmitInfo>({
					VkSemaphoreSubmitInfo{
						.semaphore = waitSem,
						.stageMask = VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT
					},
					waitEvent.has_value() ? VkSemaphoreSubmitInfo{
						.semaphore = waitEvent->event.vkSemaphore(),
						.value = waitEvent->event.value(),
						.stageMask = vkStageMask(waitEvent->stage)
					} : VkSemaphoreSubmitInfo{}
				}),
				.commandBufferInfoCount = 1,
				.pCommandBufferInfos = ptr(VkCommandBufferSubmitInfo{
					.commandBuffer = cmd
				}),
				.signalSemaphoreInfoCount = 2,
				.pSignalSemaphoreInfos = ptr<VkSemaphoreSubmitInfo>({
					VkSemaphoreSubmitInfo{
						.semaphore = signalSem,
						.stageMask = VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT
					},
					VkSemaphoreSubmitInfo{
						.semaphore = m_sync.next().vkSemaphore(),
						.value = m_sync.current().value(),
						.stageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT
					}
				})
			}), {});
		}

		m_submittedCmds.push(cmd, m_sync.current());
		return m_sync.current();
	}

	VkQueue Queue::vkQueue() const {
		return m_queue;
	}

	Queue::Queue(const QueueCreateInfo& ci)
		: m_device(ci.device.vkDevice()), m_queue(ci.device.vkQueue(ci.type)), m_type(ci.type), m_sync({ .device = ci.device }), m_heap(ci.device.descriptorHeap()),
		m_allocator(new StagingAllocator(ci.device.vkPhysicalDevice(), ci.device.vkDevice())), m_submissionLock(ci.device.vkQueueSubmissionLock(ci.type)) {
		vkCreateCommandPool(m_device, ptr(VkCommandPoolCreateInfo{
				.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
				.queueFamilyIndex = ci.device.vkQueueFamily(ci.type)
		}), nullptr, &m_pool);
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
			m_sync.current().wait();
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
