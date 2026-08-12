#pragma once

namespace Math
{
	template <typename T>
	constexpr T Min(const T& a, const T& b)
	{
		return (b < a) ? b : a;
	}

	template <typename T>
	constexpr T Max(const T& a, const T& b)
	{
		return (b > a) ? b : a;
	}
}