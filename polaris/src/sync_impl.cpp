#include "sync_impl.hpp"
#include "device_impl.hpp"
#include "vk_util.hpp"

namespace pl {
	u64 Event::value() const {
		return m_value;
	}

	void Event::wait() const {
		if(m_semaphore) {
			vkWaitSemaphores(Device::get().vkDevice(), &VkSemaphoreWaitInfo{
				.semaphoreCount = 1,
				.pSemaphores = &m_semaphore,
				.pValues = &m_value,
			}, std::numeric_limits<u64>::max());
		}
	}

	b8 Event::completed() const {
		if(m_semaphore) {
			u64 currentValue;
			vkGetSemaphoreCounterValue(Device::get().vkDevice(), m_semaphore, &currentValue);
			return currentValue >= m_value;
		}
		return true;
	}

	void Event::signal() {
		if(m_semaphore) {
			vkSignalSemaphore(Device::get().vkDevice(), &VkSemaphoreSignalInfo{
				.semaphore = m_semaphore,
				.value = m_value
			});
		}
	}

	VkSemaphore Event::vkSemaphore() const {
		return m_semaphore;
	}

	Event::Event() = default;

	Event::Event(VkSemaphore semaphore, u64 value) : m_semaphore(semaphore), m_value(value) {}

	u64 Sync::value() const {
		u64 ret;
		vkGetSemaphoreCounterValue(Device::get().vkDevice(), m_semaphore, &ret);

		return ret;
	}

	Event Sync::advance(u64 amount) {
		// + amount since fetch_add returns the old value
		return Event(m_semaphore, m_value.fetch_add(amount, std::memory_order_relaxed) + amount);
	}

	Event Sync::event() const {
		return Event(m_semaphore, m_value.load(std::memory_order_relaxed));
	}

	Sync::Sync(const SyncCreateInfo& ci) {
		vkCreateSemaphore(Device::get().vkDevice(), &VkSemaphoreCreateInfo{
			.pNext = &VkSemaphoreTypeCreateInfo {
				.semaphoreType = VK_SEMAPHORE_TYPE_TIMELINE,
				.initialValue = ci.initialValue
			}
		}, nullptr, &m_semaphore);
	}

	Sync::Sync(Sync&& src) {
		memcpy(this, &src, sizeof(Sync));
		memset(&src, 0, sizeof(Sync));
	}

	Sync& Sync::operator=(Sync&& src) {
		this->~Sync();
		new (this) Sync(std::move(src)); return *this;
	};

	Sync::~Sync() {
		vkDestroySemaphore(Device::get().vkDevice(), m_semaphore, nullptr);
	}
}