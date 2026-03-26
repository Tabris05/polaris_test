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
		using ElemType = std::remove_reference_t<T>;

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

			bool contains(const ElemType& element) const {
				for(u64 i = 0; i < m_count; i++) {
					if(m_elems[i] == element) {
						return true;
					}
				}

				return false;
			}

			ElemType* begin() const {
				return m_elems;
			}

			ElemType* end() const {
				return m_elems + m_count;
			}

			ElemType* data() const {
				return m_elems;
			}

			ElemType& front() const {
				return m_elems[0];
			}

			ElemType& back() const {
				return m_elems[m_count - 1];
			}

			ElemType& operator[](u64 index) const {
				return m_elems[index];
			}

			View(ElemType* data, u64 count) : m_elems(data), m_count(count) {}

			View(const ElemType& elem) : m_elems(const_cast<ElemType*>(&elem)), m_count(1) {}

			View(std::initializer_list<std::remove_cvref_t<T>> list): m_elems(const_cast<ElemType*>(std::data(list))), m_count(list.size()) {}

			template <std::ranges::contiguous_range R> requires
				std::same_as<std::ranges::range_value_t<R>, std::remove_cvref_t<T>> &&
				std::convertible_to<decltype(std::ranges::data(std::declval<R&>())), ElemType*>
			View(R&& range) : m_elems(range.data()), m_count(range.end() - range.begin()) {}

			template <typename U> requires !std::same_as<U, T> && std::convertible_to<std::remove_reference_t<U>*, ElemType*>
			View(View<U> other) : m_elems(other.data()), m_count(other.count()) {}

			View() = default;


		private:
			ElemType* m_elems = nullptr;
			u64 m_count = 0;
	};
}