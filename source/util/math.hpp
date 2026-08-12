#pragma once

namespace Math
{
	template <typename T>
	inline T min(T x, T y)
	{
		return (x <= y) ? x : y;
	}

	template <typename T>
	inline T max(T x, T y)
	{
		return (x >= y) ? x : y;
	}
}