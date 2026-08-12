#pragma once

namespace Math
{
	template <typename T>
	inline constexpr T Min(const T& A, const T& B)
	{
		return (B < A) ? B : A;
	}

	template <typename T>
	inline constexpr T Max(const T& A, const T& B)
	{
		return (B > A) ? B : A;
	}

	template <typename T, size_t N>
	inline constexpr T Min(const T(&Vals)[N])
	{
		T OutMin = Vals[0];
		for (size_t i = 1; i < N; i++)
		{
			if (Vals[i] < OutMin)
			{
				OutMin = Vals[i];
			}
		}
		return OutMin;
	}

	template <typename T, size_t N>
	inline constexpr T Max(const T(&Vals)[N])
	{
		T OutMin = Vals[0];
		for (size_t i = 1; i < N; i++)
		{
			if (Vals[i] > OutMin)
			{
				OutMin = Vals[i];
			}
		}
		return OutMin;
	}

	inline constexpr int Floor(const float& A)
	{
		int AsInt = (int)A;
		if (A < 0 && A != AsInt)
		{
			return AsInt - 1;
		}
		return AsInt;
	}

	inline constexpr int Ceil(const float& A)
	{
		int AsInt = (int)A;
		if (A > 0 && A != AsInt)
		{
			return AsInt + 1;
		}
		return AsInt;
	}
}