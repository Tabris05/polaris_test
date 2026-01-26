#include "vec.hpp"
#include <cstdlib>
#include <type_traits>
#include <iterator>

namespace tbrs {
	template<typename T>
	Vec<T>::Vec(u64 count) : m_count(count), m_capacity(count), m_elems(static_cast<T*>(malloc(m_capacity * sizeof(T)))) {
		if constexpr(!std::is_trivially_destructible_v<T>) {
			for(u64 i = 0; i < m_count; i++) {
				new (m_elems + i) T();
			}
		}
	}

	template<typename T>
	Vec<T>::Vec(std::initializer_list<T> list) : m_count(list.size()), m_capacity(list.size()), m_elems(static_cast<T*>(malloc(m_capacity * sizeof(T)))) {
		if constexpr(std::is_trivially_move_constructible_v<T>) {
			memcpy(m_elems, std::data(list), size());
		}
		else {
			for(u64 i = 0; i < m_count; i++) {
				new (m_elems + i) T(std::move(list[i]));
			}
		}
	}

	template<typename T>
	template<std::ranges::contiguous_range R>
	Vec<T>::Vec(R&& range) : m_count(range.count()), m_capacity(range.count()), m_elems(static_cast<T*>(malloc(m_capacity * sizeof(T)))) {
		if constexpr(std::is_trivially_copy_constructible_v<std::ranges::range_value_t<R>>) {
			memcpy(m_elems, range.data(), range.size());
		}
		else {
			for(u64 i = 0; i < m_count; i++) {
				new (m_elems + i) T(range[i]);
			}
		}
	}

	template<typename T>
	u64 Vec<T>::size() const {
		return m_count * sizeof(T);
	}

	template<typename T>
	u64 Vec<T>::count() const {
		return m_count;
	}

	template<typename T>
	bool Vec<T>::empty() const {
		return m_count == 0;
	}

	template<typename T>
	bool Vec<T>::contains(const T& element) const {
		for(u64 i = 0; i < m_count; i++) {
			if(m_elems[i] == element) {
				return true;
			}
		}

		return false;
	}

	template<typename T>
	template<typename Self>
	decltype(auto) Vec<T>::data(this Self&& self) {
		return std::forward<Self>(self).m_elems;
	}

	template<typename T>
	template<typename Self>
	decltype(auto) Vec<T>::begin(this Self&& self) {
		return std::forward<Self>(self).m_elems;
	}

	template<typename T>
	template<typename Self>
	decltype(auto) Vec<T>::end(this Self&& self) {
		return std::forward<Self>(self).m_elems + self.m_count;
	}

	template<typename T>
	template<typename Self>
	decltype(auto) Vec<T>::front(this Self&& self) {
		return std::forward<Self>(self).m_elems[0];
	}

	template<typename T>
	template<typename Self>
	decltype(auto) Vec<T>::back(this Self&& self) {
		return std::forward<Self>(self).m_elems[self.m_count - 1];
	}

	template<typename T>
	template<typename... Args>
	void Vec<T>::push(Args&&... args) {
		if(m_count == m_capacity) {
			if(m_capacity == 0) {
				m_capacity = 1;
			}
			else {
				m_capacity *= 2;
			}

			m_elems = static_cast<T*>(realloc(m_elems, m_capacity * sizeof(T)));
		}

		new (m_elems + m_count) T(std::forward<Args>(args)...);
		m_count++;
	}

	template<typename T>
	void Vec<T>::pop() {
		if constexpr(!std::is_trivially_destructible_v<T>) {
			m_elems[m_count - 1].~T();
		}

		m_count--;
	}

	template<typename T>
	template<typename... Args>
	void Vec<T>::insert(u64 index, Args&&... args) {
		if(m_count == m_capacity) {
			if(m_capacity == 0) {
				m_capacity = 1;
			}
			else {
				m_capacity *= 2;
			}

			m_elems = static_cast<T*>(realloc(m_elems, m_capacity * sizeof(T)));
		}

		memmove(m_elems + index + 1, m_elems + index, (m_count - index) * sizeof(T));
		new (m_elems + index) T(std::forward<Args>(args)...);
		m_count++;
	}

	template<typename T>
	void Vec<T>::remove(u64 index, u64 count) {
		if constexpr(!std::is_trivially_destructible_v<T>) {
			for(u64  i = index; i < index + count; i++) {
				m_elems[i].~T();
			}
		}

		m_count -= count;
		memmove(m_elems + index, m_elems + index + count, (m_count - index) * sizeof(T));
	}

	template<typename T>
	void Vec<T>::setCount(u64 newCount) {
		if constexpr(!std::is_trivially_destructible_v<T>) {
			for(u64 i = newCount; i < m_count; i++) {
				m_elems[i].~T();
			}
		}

		if(newCount > m_capacity) {
			m_capacity = newCount;
			m_elems = static_cast<T*>(realloc(m_elems, m_capacity * sizeof(T)));
		}

		if constexpr(!std::is_trivially_destructible_v<T>) {
			for(u64 i = m_count; i < newCount; i++) {
				new (m_elems + i) T();
			}
		}

		m_count = newCount;
	}

	template<typename T>
	void Vec<T>::clear() {
		setCount(0);
	}

	template<typename T>
	template<typename Self>
	decltype(auto) Vec<T>::operator[](this Self&& self, u64 index) {
		return std::forward<Self>(self).m_elems[index];
	}

	template<typename T>
	Vec<T>::~Vec() {
		if constexpr(!std::is_trivially_destructible_v<T>) {
			for(u64 i = 0; i < m_count; i++) {
				m_elems[i].~T();
			}
		}

		free(m_elems);
	}

	template<typename T>
	Vec<T>::Vec(const Vec<T>& src) {
		m_count = src.m_count;
		m_capacity = src.m_capacity;
		m_elems = static_cast<T*>(malloc(m_capacity));

		if constexpr(std::is_trivially_copy_constructible_v<T>) {
			memcpy(m_elems, src.m_elems, size());
		}
		else {
			for(u64 i = 0; i < m_count; i++) {
				new (m_elems + i) T(src.m_elems[i]);
			}
		}
	}

	template<typename T>
	Vec<T>& Vec<T>::operator=(const Vec<T>& src) {
		this->~Vec();
		new (this) Vec<T>(src);
	}

	template<typename T>
	Vec<T>::Vec(Vec<T>&& src) {
		memcpy(this, &src, sizeof(Vec<T>));
		memset(&src, 0, sizeof(Vec<T>));
	}

	template<typename T>
	Vec<T>& Vec<T>::operator=(Vec<T>&& src) {
		this->~Vec();
		new (this) Vec<T>(std::move(src));
	}
}