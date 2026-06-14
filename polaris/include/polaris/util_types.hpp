#pragma once

// foo: burn in hell stl
#include <initializer_list>
#include <ranges>
#include <optional>

using f64 = double;
static_assert(sizeof(f64) == 8, "f64 must alias a 64-bit floating point type.");

using f32 = float;
static_assert(sizeof(f32) == 4, "f32 must alias a 32-bit floating point type.");

using i64 = long long;
static_assert(sizeof(i64) == 8, "i64 must alias a 64-bit signed integer type.");

using i32 = int;
static_assert(sizeof(i32) == 4, "i32 must alias a 32-bit signed integer type.");

using i16 = short;
static_assert(sizeof(i16) == 2, "i16 must alias an 16-bit signed integer type.");

using i8 = char;
static_assert(sizeof(i8) == 1, "i8 must alias an 8-bit signed integer type.");

using u64 = unsigned long long;
static_assert(sizeof(u64) == 8, "u64 must alias a 64-bit unsigned integer type.");

using u32 = unsigned int;
static_assert(sizeof(u32) == 4, "u32 must alias a 32-bit unsigned integer type.");

using u16 = unsigned short;
static_assert(sizeof(u16) == 2, "u16 must alias an 16-bit unsigned integer type.");

using u8 = unsigned char;
static_assert(sizeof(u8) == 1, "u8 must alias an 8-bit unsigned integer type.");

using b8 = bool;
static_assert(sizeof(b8) == 1, "b8 must alias an 8-bit b8ean type.");

using b32 = u32;

using byte = u8;

namespace pl {
	template <typename T>
	class View {
		using ElemType = std::remove_const_t<T>;

		public:

			u64 size() const {
				return m_count * sizeof(T);
			}

			u64 count() const {
				return m_count;
			}

			b8 empty() const {
				return m_count == 0;
			}

			b8 contains(const ElemType& element) const {
				for(u64 i = 0; i < m_count; i++) {
					if(m_elems[i] == element) {
						return true;
					}
				}

				return false;
			}

			const ElemType* begin() const {
				return m_elems;
			}

			const ElemType* end() const {
				return m_elems + m_count;
			}

			const ElemType* data() const {
				return m_elems;
			}

			const ElemType& front() const {
				return m_elems[0];
			}

			const ElemType& back() const {
				return m_elems[m_count - 1];
			}

			const ElemType& operator[](u64 index) const {
				return m_elems[index];
			}

			View() = default;

			View(const ElemType& elem) : m_elems(&elem), m_count(1) {}

			template <u64 N>
			View(const ElemType(&arr)[N]) : m_elems(arr), m_count(N) {}

			View(const ElemType* data, u64 count) : m_elems(data), m_count(count) {}

			View(std::initializer_list<T> list) : m_elems(std::data(list)), m_count(list.size()) {}

			template <std::ranges::contiguous_range R> requires std::same_as<std::remove_const_t<std::ranges::range_value_t<R>>, ElemType>
			View(R&& range) : m_elems(range.data()), m_count(range.end() - range.begin()) {}

		private:
			const ElemType* m_elems = nullptr;
			u64 m_count = 0;
	};


	template <typename T>
	View(T& elem) -> View<std::remove_const_t<T>>;

	template <typename T>
	View(T&& elem) -> View<std::remove_const_t<T>>;

	template <typename T, u64 N>
	View(T(&arr)[N]) -> View<std::remove_const_t<T>>;

	template <typename T>
	View(T* data, u64 count) -> View<std::remove_const_t<T>>;

	template <typename T>
	View(std::initializer_list<T>) -> View<std::remove_const_t<T>>;

	template <std::ranges::contiguous_range R>
	View(R& range) -> View<std::remove_const_t<std::ranges::range_value_t<R>>>;

	template <std::ranges::contiguous_range R>
	View(R&& range) -> View<std::remove_const_t<std::ranges::range_value_t<R>>>;
}