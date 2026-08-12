#pragma once

#include <iostream>

#define _ENSURE_IMPL(Expr, EnsureFunc)		if(!!(Expr) || (EnsureFunc, false))

#define ENSURE(Expr, ...)					_ENSURE_IMPL(Expr, ValidationInternal::Internal_Ensure(#Expr, __FILE__, __LINE__, ##__VA_ARGS__)) return

namespace ValidationInternal
{
	void Internal_Ensure(const char* Text, const char* File, int Line, const char* Msg = nullptr)
	{
		std::cerr << "Ensure Failed: " << File << " [" << Line << "] " << Text;
		if(nullptr != Msg)
		{
			std::cerr << " -- " << Msg;
		}
		std::cerr << std::endl;

		__debugbreak();
	}
}