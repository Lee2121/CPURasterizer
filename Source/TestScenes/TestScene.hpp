#pragma once

#include "Rasterizer/Shapes/Cube.hpp"
#include "Rasterizer/Shapes/Triangle.hpp"
#include "Rasterizer/Shapes/Rectangle.hpp"

#include "Rasterizer/DrawCommand.hpp"

using namespace Rasterizer;

namespace TestScenes
{
	class UTestSceneBase
	{

	public:

		virtual FDrawCommand GenerateDrawCommand(float TotalTime, uint32_t Width, uint32_t Height) { return FDrawCommand(); };
	};

	class UTestScene_Cube : public UTestSceneBase
	{
	public:

		virtual FDrawCommand GenerateDrawCommand(float TotalTime, uint32_t Width, uint32_t Height) override
		{
			FMatrix4x4f Model = FMatrix4x4f::Translate({ 0.f, 0.f, -4.f })
				* FMatrix4x4f::RotateZX(TotalTime)
				* FMatrix4x4f::RotateXY(TotalTime * 1.61f);

			FMatrix4x4f Projection = FMatrix4x4f::Perspective(4.f, 10.f, M_PI / 3.f, Width * 1.f / Height);

			return
			{
				// Mesh
				{
					Rasterizer::Cube
				},
				// CullMode
				{
					ECullMode::CounterClockWise
				},
				// Transform
				{
					Projection * Model
				},
			};
		}
	};

	class UTestScene_Rectangle : public UTestSceneBase
	{
	public:

		virtual FDrawCommand GenerateDrawCommand(float TotalTime, uint32_t Width, uint32_t Height) override
		{
			FMatrix4x4f Model = FMatrix4x4f::Translate({ 0.f, 0.f, -2.f })
				* FMatrix4x4f::RotateYZ(TotalTime);

			FMatrix4x4f Projection = FMatrix4x4f::Perspective(0.01f, 10.f, M_PI / 3.f, Width * 1.f / Height);

			return
			{
				// Mesh
				{
					Rasterizer::Rectangle
				},
				// CullMode
				{
					ECullMode::None
				},
				// Transform
				{
					Projection * Model
				},
			};
		}
	};
}