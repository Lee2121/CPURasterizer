#pragma once

namespace Memory
{
	template <typename T>
	constexpr T&& Move(T& A) noexcept
	{
		return static_cast<T&&>(A);
	}

	template <typename T>
	constexpr void Swap(T& A, T& B)
	{
		T Temp = Move(A);
		A = Move(B);
		B = Move(Temp);
	}
}