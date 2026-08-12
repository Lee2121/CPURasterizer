#pragma once

#include "types.hpp"
#include "image_view.hpp"

namespace Rasterizer
{
	void Clear(const FImageView& ColorBuffer, const FVector4f& Color);
}