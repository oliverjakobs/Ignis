#pragma once

#include <fstream>

namespace obelisk
{
	inline std::string ReadFile(const std::string& path)
	{
		std::ifstream ifs(path);

		if (ifs)
			return std::string(std::istreambuf_iterator<char>(ifs.rdbuf()), std::istreambuf_iterator<char>());

		return std::string();
	}
}