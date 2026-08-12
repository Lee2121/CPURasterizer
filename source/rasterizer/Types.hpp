#pragma once

#include <cstdint>
#include "util/math.hpp"

namespace Rasterizer
{
	struct alignas(4) FColor4UB
	{
		uint8_t r = 0;
		uint8_t g = 0;
		uint8_t b = 0;
		uint8_t a = 0;
	};

	struct FVector4f
	{
		float x = 0;
		float y = 0;
		float z = 0;
		float w = 0;
	};

	inline FColor4UB Vector4fToColor4UB(const FVector4f& c)
	{
		FColor4UB Result;
		Result.r = static_cast<uint8_t>(Math::Max(0.f, Math::Min(255.f, c.x * 255.f)));
		Result.g = static_cast<uint8_t>(Math::Max(0.f, Math::Min(255.f, c.y * 255.f)));
		Result.b = static_cast<uint8_t>(Math::Max(0.f, Math::Min(255.f, c.z * 255.f)));
		Result.a = static_cast<uint8_t>(Math::Max(0.f, Math::Min(255.f, c.w * 255.f)));
		return Result;
	}
}