#pragma once

// foo: burn in hell stl
#include <initializer_list>
#include <ranges>
#include <optional>

#pragma once

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
static_assert(sizeof(b8) == 1, "b8 must alias an 8-bit boolean type.");

using b32 = u32;

using byte = u8;

namespace pl {
	template <typename T>
	class View {
		public:

			u64 size() const {
				return m_count * sizeof(T);
			}

			u64 count() const {
				return m_count;
			}

			bool empty() const {
				return m_count == 0;
			}

			bool contains(const T& element) const {
				for(u64 i = 0; i < m_count; i++) {
					if(m_elems[i] == element) {
						return true;
					}
				}

				return false;
			}

			T* begin() const {
				return m_elems;
			}

			T* end() const {
				return m_elems + m_count;
			}

			T* data() const {
				return m_elems;
			}

			T& front() const {
				return m_elems[0];
			}
			T& back() const {
				return m_elems[m_count - 1];
			}

			T& operator[](u64 index) const {
				return m_elems[index];
			}

			View(std::remove_cvref_t<T> arr[]) : m_elems(arr), m_count(sizeof(arr) / sizeof(T)) {}
			View(std::remove_cvref_t<T>* data, u64 count) : m_elems(data), m_count(count) {}
			View(std::initializer_list<std::remove_cvref_t<T>>&& list) : m_elems(std::data(list)), m_count(list.end() - list.begin()) {}

			// foo: should decouple this from std::ranges maybe
			template <std::ranges::contiguous_range R>
			requires std::same_as<std::remove_cvref_t<std::ranges::range_value_t<R>>, std::remove_cvref_t<T>>
			View(R&& range) : m_elems(std::data(range)), m_count(range.end() - range.begin()) {}

			View() = default;

		private:
			T* m_elems = nullptr;
			u64 m_count = 0;
	};
}