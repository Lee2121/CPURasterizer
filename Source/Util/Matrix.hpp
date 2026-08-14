#pragma once

#include "Vector.hpp"

struct FMatrix4x4f
{
	float Values[16];

	static FMatrix4x4f Identity()
	{
		return FMatrix4x4f{
			1.f, 0.f, 0.f, 0.f,
			0.f, 1.f, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			0.f, 0.f, 0.f, 1.f
		};
	}

	inline float& operator[](size_t Index)
	{
		return Values[Index];
	}

	inline const float& operator[](size_t Index) const
	{
		return Values[Index];
	}
};

inline FVector4f operator* (const FMatrix4x4f& M, const FVector4f& V)
{
	FVector4f Result{ 0.f, 0.f, 0.f, 0.f };

	for (size_t i = 0; i < 4; ++i)
	{
		float Sum = 0.f;
		size_t RowOffset = i * 4;
		for (size_t j = 0; j < 4; ++j)
		{
			Sum += M[RowOffset + j] * V[j];
		}
		Result[i] = Sum;
	}

	return Result;
}