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

	// Copies all of the values found in the array between First and Last into the Destination array.
	template <typename Iterator>
	Iterator Copy(Iterator First, Iterator Last, Iterator Destination)
	{
		for (; First != Last; (void*)++First, (void*)++Destination)
		{
			Destination = First;
		}
		return Destination;
	}
}