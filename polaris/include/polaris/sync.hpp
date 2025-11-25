#pragma once

#include "util_types.hpp"
#include "pod_types.hpp"

namespace pl {
	class Event {
		public:
			u64 value() const;
			bool completed() const;

			void wait() const;
			void signal();

		private:
			alignas(8) byte reserved[24];
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

		private:
			alignas(8) byte reserved[24];
	};
}