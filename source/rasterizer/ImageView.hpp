#pragma once

#include "Types.hpp"

#include <cstdint>

namespace Rasterizer
{
	struct FImageView
	{
		FColor4UB* Pixels = nullptr;
		uint32_t width = 0;
		uint32_t height = 0;
	};
}