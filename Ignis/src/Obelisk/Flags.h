#pragma once


namespace obelisk
{
	class Flags
	{
	private:
		unsigned int m_flags;

	public:
		inline void AddFlag(unsigned int flag)
		{
			m_flags |= flag;
		}

		inline void RemoveFlag(unsigned int flag)
		{
			m_flags &= ~flag;
		}

		inline bool HasFlag(unsigned int flag)
		{
			return ((m_flags & flag) == flag);
		}
	};
}