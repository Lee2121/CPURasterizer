#pragma once

#include "Image.hpp"
#include "Color.hpp"

namespace Rasterizer
{
	struct FFrameBuffer
	{
		FImage<FColor4ub> Color;
		FImage<uint32_t> Depth;

		uint32_t GetWidth() const
		{
			if (Color.IsValid())
			{
				return Color.Width;
			}
			else
			{
				return Depth.Width;
			}
		}

		uint32_t GetHeight() const
		{
			if (Color.IsValid())
			{
				return Color.Height;
			}
			else
			{
				return Depth.Height;
			}
		}
	};
}