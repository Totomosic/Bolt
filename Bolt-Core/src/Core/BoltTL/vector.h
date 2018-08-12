#pragma once
#include "iterator.h"

namespace blt
{

	template<typename _Ty>
	class vector
	{
	private:
		typedef _Ty value_type;
		typedef value_type* pointer;
		typedef const value_type* const_pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef vector<value_type> this_vector;

	public:
		typedef blt::iterator<value_type> iterator;
		typedef blt::iterator<const value_type> const_iterator;

	private:
		pointer m_Buffer;
		size_t m_Size;
		size_t m_Capacity;

	public:
		vector() : vector(1)
		{

		}

		vector(size_t capacity)
			: m_Buffer(nullptr), m_Size(0), m_Capacity(capacity)
		{
			realloc_buffer(capacity);
		}

		vector(std::initializer_list<value_type> il) : vector(il.size())
		{
			for (int i = 0; i < il.size(); i++)
			{
				push_back(*(il.begin() + i));
			}
		}

		vector(const this_vector& other) : vector(other.m_Capacity)
		{
			for (const auto& val : other)
			{
				push_back(val);
			}
		}

		vector(this_vector&& other)
		{
			m_Size = other.m_Size;
			m_Capacity = other.m_Capacity;
			m_Buffer = other.m_Buffer;
			other.m_Buffer = nullptr;
		}

		~vector()
		{
			if (m_Buffer != nullptr)
			{
				delete[] m_Buffer;
			}
		}

		this_vector& operator=(const this_vector& other)
		{
			clear();
			for (const auto& val : other)
			{
				push_back(val);
			}
			return *this;
		}

		this_vector& operator=(this_vector&& other)
		{
			pointer tempBuffer = other.m_Buffer;
			m_Size = other.m_Size;
			m_Capacity = other.m_Capacity;
			other.m_Buffer = m_Buffer;
			m_Buffer = tempBuffer;
			return *this;
		}

		inline size_t size() const { return m_Size; }
		inline size_t length() const { return size(); }
		inline size_t capacity() const { return m_Capacity; }
		inline const_pointer data() const { return buffer_ptr(); }
		inline bool empty() const { return size() == 0; }

		inline const_iterator begin() const { return const_iterator(buffer_ptr()); }
		inline iterator begin() { return iterator(buffer_ptr()); }
		inline const_iterator end() const { return const_iterator(buffer_ptr() + size()); }
		inline iterator end() { return iterator(buffer_ptr() + size()); }

		inline const_reference front() const { return buffer_ptr()[0]; }
		inline reference front() { return buffer_ptr()[0]; }
		inline const_reference back() const { return buffer_ptr()[size() - 1]; }
		inline reference back() { return buffer_ptr()[size() - 1]; }

		inline const_reference at(int index) const { return buffer_ptr()[index]; }
		inline reference at(int index) { return buffer_ptr()[index]; }
		inline const_reference operator[](int index) const { return at(index); }
		inline reference operator[](int index) { return at(index); }

		void reserve(size_t capacity)
		{
			test_size(capacity);
		}

		void clear()
		{
			m_Size = 0;
		}

		void push_back(value_type&& value)
		{
			test_size(size() + 1);
			buffer_ptr()[size()] = std::move(value);
			m_Size++;
		}

		void push_back(const_reference value)
		{
			test_size(size() + 1);
			buffer_ptr()[size()] = value;
			m_Size++;
		}

		void pop_back()
		{
			m_Size--;
		}

		void pop_front()
		{
			m_Size--;
			move_elements(1, -1);
		}

		void insert(size_t pos, const_reference value)
		{
			test_size(size() + 1);
			move_elements(pos, 1);
			buffer_ptr()[pos] = value;
			m_Size++;
		}

		void insert(iterator pos, const_reference value)
		{
			size_t position = (size_t)(pos.get() - begin().get());
			insert(position, value);
		}

		void erase(size_t pos, size_t len)
		{
			move_elements(pos + len, -((int)len));
			m_Size -= len;
		}

		void erase(size_t pos)
		{
			return erase(pos, 1);
		}

		void erase(iterator start, iterator finish)
		{
			return erase((size_t)(start.get() - begin().get()), (size_t)(finish.get() - start.get()));
		}

		void erase(iterator it)
		{
			return erase(it.get() - begin().get());
		}

	private:
		void move_elements(size_t pos, int amount)
		{
			if (std::is_trivially_copyable<value_type>::value)
			{
				move_elements_trivial(pos, amount);
			}
			else if (std::is_move_constructible<value_type>::value)
			{
				move_elements_move(pos, amount);
			}
			else
			{
				move_elements_copy(pos, amount);
			}
		}

		void move_elements_copy(size_t pos, int amount)
		{
			if (amount < 0)
			{
				for (size_t i = pos; i < size() - pos; i++)
				{
					buffer_ptr()[i + amount] = buffer_ptr()[i];
				}
			}
			else
			{
				for (size_t i = size() - 1; i >= pos; i--)
				{
					buffer_ptr()[i + amount] = buffer_ptr()[i];
				}
			}
		}

		void move_elements_move(size_t pos, int amount)
		{
			if (amount < 0)
			{
				for (size_t i = pos; i < size() - pos; i++)
				{
					buffer_ptr()[i + amount] = std::move(buffer_ptr()[i]);
				}
			}
			else
			{
				for (size_t i = size() - 1; i >= pos; i--)
				{
					buffer_ptr()[i + amount] = std::move(buffer_ptr()[i]);
				}
			}
		}

		void move_elements_trivial(size_t pos, int amount)
		{
			memmove(buffer_ptr() + pos + amount, buffer_ptr() + pos, (size() - pos) * sizeof(value_type));
		}

		void test_size(size_t required_size)
		{
			if (required_size > capacity())
			{
				realloc_buffer(required_size);
			}
		}

		void realloc_buffer(size_t new_capacity)
		{
			if (std::is_trivially_copyable<value_type>::value)
			{
				realloc_buffer_trivial(new_capacity);
			}
			else if (std::is_move_assignable<value_type>::value)
			{
				realloc_buffer_move(new_capacity);
			}
			else
			{
				realloc_buffer_copy(new_capacity);
			}
		}

		void realloc_buffer_copy(size_t new_capacity)
		{
			pointer newBuffer = new value_type[new_capacity];
			if (m_Buffer != nullptr)
			{
				for (size_t i = 0; i < size(); i++)
				{
					newBuffer[i] = buffer_ptr()[i];
				}
				delete[] m_Buffer;
			}
			m_Buffer = newBuffer;
			m_Capacity = new_capacity;
		}

		void realloc_buffer_move(size_t new_capacity)
		{
			pointer newBuffer = new value_type[new_capacity];
			if (m_Buffer != nullptr)
			{
				for (size_t i = 0; i < size(); i++)
				{
					newBuffer[i] = std::move(buffer_ptr()[i]);
				}
				delete[] m_Buffer;
			}
			m_Buffer = newBuffer;
			m_Capacity = new_capacity;
		}

		void realloc_buffer_trivial(size_t new_capacity)
		{
			pointer newBuffer = new value_type[new_capacity];
			if (m_Buffer != nullptr)
			{
				memcpy(newBuffer, buffer_ptr(), size() * sizeof(value_type));
				delete[] m_Buffer;
			}
			m_Buffer = newBuffer;
			m_Capacity = new_capacity;
		}

		pointer buffer_ptr() const
		{
			return m_Buffer;
		}

	};

}