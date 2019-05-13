#pragma once
#include "iterator.h"

namespace blt
{

	// Represents a 2D array of type T, Row major ordering
	// 0 1 2
	// 3 4 5
	// 6 7 8
	template<typename T, uint32_t Cols, uint32_t Rows>
	class BLT_API array2d
	{
	public:
		using ref = T&;
		using ptr = T*;
		using const_ptr = const ptr;

		using array_it = blt::iterator<T>;

	private:
		T m_Data[Cols * Rows];

	public:
		ptr data() const { return m_Data; }
		constexpr uint32_t size() const { return Cols * Rows; }
		constexpr uint32_t length() const { return Cols * Rows; }

		ref operator*() const { return *m_Data; }
		const_ptr operator->() const { return m_Data; }
		ptr operator->() { return m_Data; }
		ref operator[](uint32_t index) const { return m_Data[index]; }

		array_it begin() const { return array_it(m_Data); }
		array_it end() const { return array_it(m_Data + Cols * Rows); }

		ref get(uint32_t x, uint32_t y) const { return m_Data[x + y * Cols]; }

	};

}