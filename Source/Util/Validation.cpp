#pragma once

#include "Validation.hpp"

namespace ValidationInternal
{
	void Internal_Ensure(const char* Text, const char* File, int Line, const char* Msg)
	{
		FTrackedEnsure NewEnsure = FTrackedEnsure(File, Line);
		if (FTrackedEnsureList::GetInstance().AddTrackedEnsure(NewEnsure))
		{
			std::cerr << "Ensure Failed: " << File << " [" << Line << "] " << Text;
			if (nullptr != Msg)
			{
				std::cerr << " -- " << Msg;
			}
			std::cerr << std::endl;

			__debugbreak();
		}
	}
}