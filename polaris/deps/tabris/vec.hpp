#pragma once

#include "types.hpp"
#include <initializer_list>
#include <ranges>

namespace tbrs {
	template <typename T>
	class Vec {
		public:
			Vec() = default;
			Vec(u64 count);
			Vec(std::initializer_list<T> list);
			
			template <std::ranges::contiguous_range R>
			Vec(R&& range);

			u64 size() const;
			u64 count() const;
			bool empty() const;
			bool contains(const T& element) const;

			template<typename Self>
			decltype(auto) data(this Self&& self);

			template<typename Self>
			decltype(auto) begin(this Self&& self);

			template<typename Self>
			decltype(auto) end(this Self&& self);

			template<typename Self>
			decltype(auto) front(this Self&& self);

			template<typename Self>
			decltype(auto) back(this Self&& self);

			template<typename... Args>
			void push(Args&&... args);
			void pop();
			
			template<typename... Args>
			void insert(u64 index, Args&&... args);
			void remove(u64 index);

			void setCount(u64 newCount);
			void clear();

			template<typename Self>
			decltype(auto) operator[](this Self&& self, u64 index);

			~Vec();

			Vec(const Vec& src);
			Vec& operator=(const Vec& src);

			Vec(Vec&& src);
			Vec& operator=(Vec&& src);

		private:
			u64 m_count = 0;
			u64 m_capacity = 0;
			T* m_elems = nullptr;
	};
}

#include "vec.cpp"
