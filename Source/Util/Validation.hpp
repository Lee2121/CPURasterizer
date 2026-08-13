#pragma once

#include <iostream>

#define _ENSURE_IMPL(Expr, EnsureFunc)	if(!!(Expr) || (EnsureFunc, false))
#define _ENSURE_END_IMPL				{} else

// Triggers an error if the given expression fails, then continues
#define ENSURE(Expr, ...)				_ENSURE_IMPL(Expr, ValidationInternal::Internal_Ensure(#Expr, __FILE__, __LINE__, ##__VA_ARGS__)) _ENSURE_END_IMPL

// Triggers an error if the given expression fails, and exits the calling function if that occurs.
#define ENSURE_EXIT(Expr, ...)			ENSURE(Expr, #__VA_ARGS__) return


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