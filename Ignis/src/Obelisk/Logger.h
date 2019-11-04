#pragma once

#include "Singelton.h"

#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif 

namespace obelisk
{
	/*
	specifier	Output
	d or i		Signed decimal integer
	u			Unsigned decimal integer
	o			Unsigned octal
	x			Unsigned hexadecimal integer, lowercase 7fa
	X			Unsigned hexadecimal integer, uppercase 7FA
	f			Decimal floating point, lowercase
	F			Decimal floating point, uppercase
	e			Scientific notation(mantissa / exponent), lowercase 3.9265e+2
	E			Scientific notation(mantissa / exponent), uppercase 3.9265E+2
	g			Use the shortest representation: % e or % f
	G			Use the shortest representation: % E or % F
	a			Hexadecimal floating point, lowercase - 0xc.90fep-2
	A			Hexadecimal floating point, uppercase - 0XC.90FEP-2
	c			Character
	s			String of characters
	p			Pointer address
	%			A % followed by another % character will write a single % to the stream.
	*/

	// a string formatter using std::snprintf
	template<typename... argv> std::string format(const char* format, argv ...args)
	{
		const size_t str_size = std::snprintf(nullptr, 0, format, args...);

		std::string output;
		output.resize(str_size + 1);

		std::snprintf(&output[0], str_size + 1, format, args...);
		return std::move(output);
	}

#ifdef _WIN32
	inline std::ostream& colored_stream(std::ostream& s, unsigned int color)
	{
		HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hStdout, color);
		return s;
	}

	inline std::ostream& red(std::ostream& s)
	{
		return colored_stream(s, FOREGROUND_RED | FOREGROUND_INTENSITY);
	}

	inline std::ostream& green(std::ostream& s)
	{
		return colored_stream(s, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	}

	inline std::ostream& blue(std::ostream& s)
	{
		return colored_stream(s, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	}

	inline std::ostream& yellow(std::ostream& s)
	{
		return colored_stream(s, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
	}

	inline std::ostream& white(std::ostream& s)
	{
		return colored_stream(s, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	}

	inline std::ostream& red_bg(std::ostream& s)
	{
		return colored_stream(s, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | BACKGROUND_RED);
	}
#else
#error Colored Stream only supported for windows
#endif

	class Logger : private Singleton<Logger>
	{
	private:
		std::ostream& m_target = std::cout;

	public:
		template<typename... argv>
		inline static void Trace(const char* fmt, const argv& ...args)
		{
			Get().m_target << white << "[TRACE] " << format(fmt, args...) << "\n";
		}

		template<typename... argv>
		inline static void Info(const char* fmt, const argv& ...args)
		{
			Get().m_target << green << "[INFO] " << white << format(fmt, args...) << "\n";
		}

		template<typename... argv>
		inline static void Warn(const char* fmt, const argv& ...args)
		{
			Get().m_target << yellow << "[WARN] " << white << format(fmt, args...) << "\n";
		}

		template<typename... argv>
		inline static void Error(const char* fmt, const argv& ...args)
		{
			Get().m_target << red << "[ERROR] " << white << format(fmt, args...) << "\n";
		}

		template<typename... argv>
		inline static void Critical(const char* fmt, const argv& ...args)
		{
			Get().m_target << red_bg << "[CRITICAL]" << white << " " << format(fmt, args...) << "\n";
		}
	};
}