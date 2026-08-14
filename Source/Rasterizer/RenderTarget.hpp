#pragma once

#include "ImageView.hpp"
#include "Viewport.hpp"

namespace Rasterizer
{
	struct FRenderTarget
	{
		FImageView ColorBuffer;
		FViewport Viewport;
	};
}