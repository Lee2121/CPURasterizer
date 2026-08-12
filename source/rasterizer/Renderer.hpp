#pragma once

#include "Types.hpp"
#include "ImageView.hpp"

namespace Rasterizer
{
	void Clear(const FImageView& ColorBuffer, const FVector4f& Color);
}