#pragma once

#include <cstdint>

#include "Util/Vector.hpp"
#include "Util/Math.hpp"

namespace Rasterizer
{
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
}