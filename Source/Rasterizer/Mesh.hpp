#pragma once

#include "Types.hpp"
#include "Attribute.hpp"

namespace Rasterizer
{
	struct FMesh
	{
		TAttribute<FVector3f> Positions = {};
		TAttribute<FVector4f> Colors = {};
		uint32_t const* Indices = nullptr;
		uint32_t Count = 0; // If Indices is valid, contains the number indices in the array. If Indices is not valid, count is the number of vertices.
	};
}