#pragma once

#include "Util/Vector.hpp"
#include "DrawCommand.hpp"
#include "RenderTarget.hpp"

namespace Rasterizer
{
	void Clear(const FRenderTarget& RenderTarget, const FVector4f& Color);
	void Draw(const FRenderTarget& RenderTarget, const FDrawCommand& Command);	
}