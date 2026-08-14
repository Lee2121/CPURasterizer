#pragma once

#include "Types.hpp"

namespace Rasterizer
{
	struct FViewport
	{
		uint32_t xMin;
		uint32_t xMax;
		uint32_t yMin;
		uint32_t yMax;

		inline FVector4f ViewportToScreenCoords(const FVector4f& ViewportCoords) const
		{
			FVector4f ScreenCoords;
			ScreenCoords.X = xMin + (xMax - xMin) * (0.5f + 0.5f * ViewportCoords.X);
			ScreenCoords.Y = yMin + (yMax - yMin) * (0.5f - 0.5f * ViewportCoords.Y);
			return ScreenCoords;
		}
	};
}