#pragma once

#include <cstdint>
#include "Util/Math.hpp"

namespace Rasterizer
{
	struct FVector4f
	{
		FVector4f() {};
		FVector4f(float InX, float InY, float InZ, float InW) 
			: X(InX), Y(InY), Z(InZ), W(InW)
		{
		}

		float X = 0;
		float Y = 0;
		float Z = 0;
		float W = 0;

		inline float& operator[](size_t Index)
		{
			switch (Index)
			{
			case 0:	return X;
			case 1:	return Y;
			case 2:	return Z;
			case 3:	return W;
			default: throw;
			}
		}

		inline const float& operator[](size_t Index) const
		{
			switch (Index)
			{
			case 0:	return X;
			case 1:	return Y;
			case 2:	return Z;
			case 3:	return W;
			default: throw;
			}
		}
	};

	inline FVector4f operator- (const FVector4f& v0, const FVector4f& v1)
	{
		return {v0.X - v1.X, v0.Y - v1.Y, v0.Z - v1.Z, v0.W - v1.W};
	}

	inline FVector4f operator+ (const FVector4f& v0, const FVector4f& v1)
	{
		return { v0.X + v1.X, v0.Y + v1.Y, v0.Z + v1.Z, v0.W + v1.W };
	}

	inline FVector4f operator* (const FVector4f& v0, const float& f)
	{
		return { v0.X * f, v0.Y * f, v0.Z * f, v0.W * f };
	}

	inline float Determinant2D(const FVector4f& v0, const FVector4f& v1)
	{
		return v0.X * v1.Y - v0.Y * v1.X;
	}

	struct alignas(4) FColor4ub
	{
		uint8_t R = 0;
		uint8_t G = 0;
		uint8_t B = 0;
		uint8_t A = 0;

		static inline FColor4ub FromVector4F(const FVector4f& C)
		{
			FColor4ub Result;
			Result.R = static_cast<uint8_t>(Math::Max(0.f, Math::Min(255.f, C.X * 255.f)));
			Result.G = static_cast<uint8_t>(Math::Max(0.f, Math::Min(255.f, C.Y * 255.f)));
			Result.B = static_cast<uint8_t>(Math::Max(0.f, Math::Min(255.f, C.Z * 255.f)));
			Result.A = static_cast<uint8_t>(Math::Max(0.f, Math::Min(255.f, C.W * 255.f)));
			return Result;
		}
	};

	struct FVector3f
	{
		float X = 0;
		float Y = 0;
		float Z = 0;

		inline FVector4f AsVector4f(const float w = 0.f) const
		{
			return { X, Y, Z, w };
		}
	};

	inline FVector3f operator- (const FVector3f& v0, const FVector3f& v1)
	{
		return { v0.X - v1.X, v0.Y - v1.Y, v0.Z - v1.Z };
	}

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
		FVector4f Result{0.f, 0.f, 0.f, 0.f};

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
}