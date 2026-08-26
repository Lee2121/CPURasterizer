#pragma once

#include "Util/Vector.hpp"
#include "DrawCommand.hpp"
#include "RenderTarget.hpp"

namespace Rasterizer
{
	void Clear(FRenderTarget& RenderTarget, const FVector4f& Color, const uint32_t& Depth);
	void Draw(const FRenderTarget& RenderTarget, const FDrawCommand& Command);	
}