#pragma once

#include "Vector.hpp"

struct FMatrix4x4f
{
	float Values[16];

	static FMatrix4x4f Identity();
	static FMatrix4x4f Zero();

	void ApplyScale(const FVector3f& S);
	void ApplyScale(const float& S);
	void ApplyTranslation(const FVector3f& T);
	void ApplyRotationXY(float Angle);
	void ApplyRotationYZ(float Angle);
	void ApplyRotationZX(float Angle);

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

inline FMatrix4x4f operator*(const FMatrix4x4f& M1, const FMatrix4x4f& M2)
{
	FMatrix4x4f Result = FMatrix4x4f::Zero();
	for (size_t i = 0; i < 4; ++i)
	{
		for (size_t j = 0; j < 4; ++j)
		{
			for (size_t k = 0; k < 4; ++k)
			{
				Result[4 * i + j] += M1[4 * i + k] * M2[4 * k + j];
			}
		}
	}
	return Result;
}