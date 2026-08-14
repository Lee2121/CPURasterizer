#pragma once

#include "Types.hpp"
#include "Attribute.hpp"

namespace Rasterizer
{
	struct FMesh
	{
		TAttribute<FVector3f> Positions = {};
		TAttribute<FVector4f> Colors = {};
		uint32_t VertexCount = 0;
	};
}