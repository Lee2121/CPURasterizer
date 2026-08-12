#pragma once

#include "Mesh.hpp"
#include "Settings.hpp"

namespace Rasterizer
{
	struct FDrawCommand
	{
		FMesh Mesh;
		ECullMode CullMode = ECullMode::None;
	};
}