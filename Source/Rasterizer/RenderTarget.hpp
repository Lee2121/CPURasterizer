#pragma once

#include "FrameBuffer.hpp"
#include "Viewport.hpp"

namespace Rasterizer
{
	struct FRenderTarget
	{
		FFrameBuffer FrameBuffer;
		FViewport Viewport;
	};
}