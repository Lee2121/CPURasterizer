#pragma once

#include "renderer.hpp"

namespace Rasterizer
{
	void Clear(const FImageView& ColorBuffer, const FVector4f& Color)
	{
		FColor4UB* Pixels = ColorBuffer.Pixels;
		float Size = ColorBuffer.width * ColorBuffer.height;
		for (int i = 0; i < Size; i++)
		{
			Pixels[i] = Vector4fToColor4UB(Color);
		}
	}
}