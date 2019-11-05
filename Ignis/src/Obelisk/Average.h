#pragma once

#include <algorithm>

namespace obelisk
{
	template<typename Type>
	class Average
	{
	private:
		Type m_average;
		Type m_max;
		Type m_min;

		Type m_runningAverage;
		unsigned int m_count;

	public:
		Average()
		{
			m_max = Type();
			m_min = Type();
			m_average = Type();

			m_runningAverage = Type();
			m_count = 0;
		}

		void Set(Type value);

		Type GetAverage() const;
		Type GetMax() const;
		Type GetMin() const;
	};

	template<typename Type>
	inline void Average<Type>::Set(Type value)
	{
		m_max = std::max(m_max, value);
		m_min = std::min((m_min <= 0.0f) ? value : m_min, value);

		m_runningAverage += value;
		m_average = m_runningAverage / ++m_count;
	}

	template<typename Type>
	inline Type Average<Type>::GetAverage() const
	{
		return m_average;
	}

	template<typename Type>
	inline Type Average<Type>::GetMax() const
	{
		return m_max;
	}

	template<typename Type>
	inline Type Average<Type>::GetMin() const
	{
		return m_min;
	}
}