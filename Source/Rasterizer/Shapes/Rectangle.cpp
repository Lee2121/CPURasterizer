#pragma once

#include "Rectangle.hpp"

namespace Rasterizer
{
	static FVector3f RectanglePositions[] =
	{
		{-1.f, -1.f, 0.f},
		{ 1.f, -1.f, 0.f},
		{-1.f,  1.f, 0.f},
		{ 1.f,  1.f, 0.f},
	};

	static FVector4f RectangleColors[] =
	{
		{0.f, 0.f, 0.f, 1.f},
		{1.f, 0.f, 0.f, 1.f},
		{0.f, 1.f, 0.f, 1.f},
		{1.f, 1.f, 0.f, 1.f},
	};

	static size_t RectangleIndices[] =
	{
		0, 1, 2,
		2, 1, 3,
	};

	const FMesh Rectangle
	{
		// Positions
		{
			RectanglePositions
		},
		// Colors
		{
			RectangleColors
		},
		// Indices
		{
			RectangleIndices
		},
		// Count
		6
	};
}