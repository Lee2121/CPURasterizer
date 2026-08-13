#pragma once

#include <iostream>

#include "Array.hpp"

#define _ENSURE_IMPL(Expr, EnsureFunc)	if(!!(Expr) || (EnsureFunc, false))
#define _ENSURE_END_IMPL				{} else

// Triggers an error if the given expression fails, then continues
#define ENSURE(Expr, ...)				_ENSURE_IMPL(Expr, ValidationInternal::Internal_Ensure(#Expr, __FILE__, __LINE__, ##__VA_ARGS__)) _ENSURE_END_IMPL

// Triggers an error if the given expression fails, and exits the calling function if that occurs.
#define ENSURE_EXIT(Expr, ...)			ENSURE(Expr, #__VA_ARGS__) return

namespace ValidationInternal
{
	#define MAX_TRACKED_ENSURES			10

	struct FTrackedEnsure
	{
		FTrackedEnsure() {};
		FTrackedEnsure(const char* InFile, const int& InLine) 
			: File(InFile), Line(InLine) 
		{
		};

		const char* File = nullptr;
		int Line = -1;

		bool operator==(const FTrackedEnsure& Other) const
		{
			return (Line == Other.Line && File == Other.File);
		}
	};

	struct FTrackedEnsureList
	{

	public:

		static FTrackedEnsureList& GetInstance()
		{
			static FTrackedEnsureList Instance;
			return Instance;
		}

	private:
		FTrackedEnsureList() {};
		~FTrackedEnsureList() = default;

		TFixedArray<FTrackedEnsure, MAX_TRACKED_ENSURES> TrackedEnsures;
		int NumTrackedEnsures = 0;

	public:

		bool AddTrackedEnsure(const FTrackedEnsure& Ensure)
		{
			bool bNewEnsure = !TrackedEnsures.Contains(Ensure);
			if (bNewEnsure)
			{
				TrackedEnsures[NumTrackedEnsures] = Ensure;
				NumTrackedEnsures = (NumTrackedEnsures + 1) % MAX_TRACKED_ENSURES; // Just boot out the last tracked ensure if we hit the limit
			}
			return bNewEnsure;
		}
	};

	void Internal_Ensure(const char* Text, const char* File, int Line, const char* Msg = nullptr)
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