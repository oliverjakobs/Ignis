#pragma once

#include <iterator>
#include <stdexcept>

namespace detail 
{
	template <typename Type>
	class range 
	{
	private:
		const Type m_start;
		const Type m_stop;
		const Type m_step;

	public:
		range(const Type& start, const Type& stop, const Type& step) : m_start(start), m_stop(stop), m_step(step)
		{
			if (m_step == 0)
				throw std::invalid_argument("Range step argument must not be zero");
			else if ((m_start > m_stop && m_step > 0) || (m_start < m_stop && m_step < 0))
				throw std::invalid_argument("Range arguments must result in termination");
		}

		class iterator 
		{
		private:
			Type m_value;
			const Type m_step;
			const Type m_boundary;
			const bool m_positive_step;

		public:
			iterator(Type value, Type step, Type boundary) 
				: m_value(value), m_step(step), m_boundary(boundary), m_positive_step(step > 0) {}

			iterator operator++() { m_value += m_step; return *this; }
			Type& operator*() { return m_value; }
			const Type* operator->() { return &m_value; }

			bool operator==(const iterator& rhs)
			{
				return m_positive_step ? (m_value >= rhs.m_value && m_value > m_boundary) : (m_value <= rhs.m_value && m_value < m_boundary);
			}
			bool operator!=(const iterator& rhs) 
			{
				return m_positive_step ? (m_value < rhs.m_value && m_value >= m_boundary) : (m_value > rhs.m_value && m_value <= m_boundary);
			}
		};

		iterator begin() const
		{
			return iterator(m_start, m_step, m_start);
		}

		iterator end() const 
		{
			return iterator(m_stop, m_step, m_start);
		}
	};
}


namespace obelisk
{
	template <typename T>
	detail::range<T> range(const T& stop)
	{
		return detail::range<T>(T{ 0 }, stop, T{ 1 });
	}

	template <typename T>
	detail::range<T> range(const T& start, const T& stop)
	{
		return detail::range<T>(start, stop, T{ 1 });
	}

	template <typename T>
	detail::range<T> range(const T& start, const T& stop, const T& step)
	{
		return detail::range<T>(start, stop, step);
	}
}