#ifndef KBL_CORE_H
#define KBL_CORE_H

#include <stdint.h>
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>


#define KBL_ASSERT(x, ...) { if (!(x)) { ASSERT_LOG(__VA_ARGS__); __debugbreak(); } }
//#define KBL_ERROR(...) { std::cout << __VA_ARGS__ << std::endl; exit(1); }

template <typename... Args>
void ASSERT_LOG(Args&& ... args)
{
	std::cout << "ASSERTION: ";
	(std::cout << ... << args);
	std::cout << std::endl;
	
}

template <typename... Args>
void KBL_ERROR(Args&& ... args)
{
	std::cout << "ERROR: ";
	(std::cout << ... << args);
	std::cout << std::endl;
	exit(1);
}

template <typename... Args>
void KBL_WARN(Args&& ... args)
{
	std::cout << "WARN: ";
	(std::cout << ... << args);
	std::cout << std::endl;
}

#endif