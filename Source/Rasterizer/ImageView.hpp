#pragma once

#include "Types.hpp"

#include <cstdint>

namespace Rasterizer
{
	struct FImageView
	{
		FColor4ub* Pixels = nullptr;
		uint32_t Width = 0;
		uint32_t Height = 0;

		FColor4ub& GetPixelAtPos(uint32_t x, uint32_t y) const
		{
			return Pixels[x + y * Width];
		}
	};
}