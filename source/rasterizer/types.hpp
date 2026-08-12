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
		Result.r = Math::max(0.f, Math::min(255.f, c.x * 255.f));
		Result.g = Math::max(0.f, Math::min(255.f, c.y * 255.f));
		Result.b = Math::max(0.f, Math::min(255.f, c.z * 255.f));
		Result.a = Math::max(0.f, Math::min(255.f, c.w * 255.f));
		return Result;
	}
}