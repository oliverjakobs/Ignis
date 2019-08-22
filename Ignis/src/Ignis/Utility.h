#pragma once 

#include <iostream>
#include <sstream>

#define BIT(x) (1 << x)

#define SAFE_DELETE(ptr) if( (ptr) != nullptr ) delete (ptr); (ptr) = nullptr;

#ifdef _DEBUG
	#define DEBUG_MESSAGE(s) std::cout << s << std::endl
#else
	#define DEBUG_MESSAGE(s) 
#endif