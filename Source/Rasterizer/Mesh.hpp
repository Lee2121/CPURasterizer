#pragma once

#include "Types.hpp"

namespace Rasterizer
{
	struct FMesh
	{
		const FVector3f* Positions = nullptr;
		uint32_t VertexCount = 0;
		FVector4f Color = {1.f, 1.f , 1.f, 1.f};
	};
}