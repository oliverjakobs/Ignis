#pragma once 

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

#include "Debugger.h"
#include "Timer.h"

#define BIT(x) (1 << x)

#define SAFE_DELETE(ptr) if( (ptr) != nullptr ) delete (ptr); (ptr) = nullptr;

inline std::string ReadFile(const std::string& path)
{
	if (path.empty())
		return "";

	std::ifstream ifs(path);

	std::string content(std::istreambuf_iterator<char>(ifs.rdbuf()), std::istreambuf_iterator<char>());

	return content;
}