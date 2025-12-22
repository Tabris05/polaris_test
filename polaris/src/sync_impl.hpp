#pragma once

#include <polaris/pod_types.hpp>
#include <volk/volk.h>
#include <atomic>

namespace pl {
	class Event {
		public:
			u64 value() const;
			bool completed() const;

			void wait() const;
			void signal(); // foo: const qualifying this does nothing because you can and should be able to make a non const copy of a const object

			Event();

			// "public" functions that should not be included in the public header
			VkSemaphore vkSemaphore() const;
			Event(VkDevice device, VkSemaphore semaphore, u64 value);

		private:
			VkDevice m_device = {};
			VkSemaphore m_semaphore = {};
			u64 m_value = 0;
	};

	class Sync {
	public:
		u64 value() const;

		Event next();
		Event current() const;

		Sync(const SyncCreateInfo& ci);
		Sync(Sync&& src);
		Sync& operator=(Sync&& src);
		~Sync();

		Sync(const Sync&) = delete;
		Sync& operator=(const Sync&) = delete;

		// "public" functions that should not be included in the public header
		Sync() = default;

	private:
		VkDevice m_device = {};
		VkSemaphore m_semaphore = {};
		std::atomic<u64> m_value = 0;
	};
}
