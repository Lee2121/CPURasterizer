#pragma once

#include "types.hpp"

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