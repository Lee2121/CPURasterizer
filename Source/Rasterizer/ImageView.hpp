#pragma once

#include "Color.hpp"

namespace Rasterizer
{
	template <typename TPixel>
	struct FImageView
	{
		TPixel* Pixels = nullptr;
		uint32_t Width = 0;
		uint32_t Height = 0;

		bool IsValid() const { return nullptr != Pixels; }

		FColor4ub& GetPixelAtPos(uint32_t x, uint32_t y) const
		{
			return Pixels[x + y * Width];
		}
	};
}