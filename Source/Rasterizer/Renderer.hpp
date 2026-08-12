#pragma once

#include "Types.hpp"
#include "ImageView.hpp"
#include "DrawCommand.hpp"

namespace Rasterizer
{
	void Clear(const FImageView& ColorBuffer, const FVector4f& Color);
	void Draw(const FImageView& ColorBuffer, const FDrawCommand& Command);
}