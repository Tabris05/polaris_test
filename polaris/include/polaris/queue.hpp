#pragma once

#include "pod_types.hpp"
#include "command_buffer.hpp"
#include "sync.hpp"

namespace pl {
	struct SubmitInfo {
		CommandBuffer commandBuffer;
		std::optional<const Event> waitEvent;
		std::optional<Event> signalEvent;
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

		private:
			alignas(8) byte reserved[112];
	};
}