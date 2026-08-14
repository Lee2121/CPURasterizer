#pragma once

#include "Mesh.hpp"
#include "Settings.hpp"
#include "Util/Matrix.hpp"

namespace Rasterizer
{
	struct FDrawCommand
	{
		FMesh Mesh;
		ECullMode CullMode = ECullMode::None;
		FMatrix4x4f Transform = FMatrix4x4f::Identity();
	};
}