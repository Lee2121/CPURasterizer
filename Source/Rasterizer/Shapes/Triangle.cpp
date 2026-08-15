#pragma once

#include "Triangle.hpp"

namespace Rasterizer
{
	static FVector3f TrianglePositions[] =
	{
		{-0.5f, -0.5f, 0.f},
		{-0.5f,  0.5f, 0.f},
		{ 0.5f, -0.5f, 0.f},
	};

	static FVector4f TriangleColors[] =
	{
		{1.f, 0.f, 0.f, 1.f},
		{0.f, 1.f, 0.f, 1.f},
		{0.f, 0.f, 1.f, 1.f},
	};

	static size_t TriangleIndices[] =
	{
		0, 1, 2,
	};

	const FMesh Triangle
	{
		// Positions
		{
			TrianglePositions
		},
		// Colors
		{
			TriangleColors
		},
		// Indices
		{
			TriangleIndices
		},
		// Count
		3
	};
}