#pragma once

#include <cstdint>
#include "Util/Math.hpp"

namespace Rasterizer
{
	struct FVector4f
	{
		float X = 0;
		float Y = 0;
		float Z = 0;
		float W = 0;
	};

	inline constexpr FVector4f operator- (const FVector4f& v0, const FVector4f& v1)
	{
		return {v0.X - v1.X, v0.Y - v1.Y, v0.Z - v1.Z, v0.W - v1.W};
	}

	inline constexpr float Determinant2D(const FVector4f& v0, const FVector4f& v1)
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

		constexpr inline FVector4f AsVector4f(const float w = 0.f) const
		{
			return { X, Y, Z, w };
		}
	};

	inline constexpr FVector3f operator- (const FVector3f& v0, const FVector3f& v1)
	{
		return { v0.X - v1.X, v0.Y - v1.Y, v0.Z - v1.Z };
	}
}