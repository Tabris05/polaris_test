#pragma once

#include <polaris/util_types.hpp>
#include <polaris/pod_types.hpp>
#include <volk/volk.h>
#include <tabris/vec.hpp>
#include <utility>

#include "command_buffer_impl.hpp"
#include "sync_impl.hpp"
#include "descriptor_heap.hpp"
#include "staging_allocator.hpp"


namespace pl {
	struct EventInfo {
		PipelineStage stage;
		Event event;
	};

	struct SubmitInfo {
		CommandBuffer commandBuffer;
		std::optional<const EventInfo> waitEvent;
		std::optional<EventInfo> signalEvent;
	};

	class Queue {
		public:
			CommandBuffer beginRecording();
			const Event submit(const SubmitInfo& si);

			Queue(const QueueCreateInfo& ci);
			Queue(Queue&&);
			Queue& operator=(Queue&&);
			~Queue();

			Queue(const Queue&) = delete;
			Queue& operator=(const Queue&) = delete;

			// "public" functions that should not be included in the public header
			const Event submit(CommandBuffer commandBuffer, std::optional<const Event> waitEvent, VkSemaphore waitSem, VkSemaphore signalSem);
			VkQueue vkQueue() const;

		private:
			struct Submission {
				VkCommandBuffer cmd;
				Event event;
				tbrs::Vec<StagingBuffer> stagingBuffers;
			};

			VkQueue m_queue = {};
			VkCommandPool m_pool = {};

			QueueType m_type;

			Sync m_sync;
			tbrs::Vec<VkCommandBuffer> m_freeCmds;
			tbrs::Vec<Submission> m_submittedCmds;

			StagingAllocator* m_allocator = nullptr;
	};
}
