#pragma once

#include "Rasterizer/Shapes/Cube.hpp"
#include "Rasterizer/Shapes/Triangle.hpp"
#include "Rasterizer/Shapes/Rectangle.hpp"

#include "Rasterizer/DrawCommand.hpp"

#include <cstdint>

#define _USE_MATH_DEFINES 1
#include <math.h>

using namespace Rasterizer;

#define MAX_DRAW_COMMANDS 5

namespace TestScenes
{
	class UTestSceneBase
	{

	public:

		int NumDrawCommands = 0;

		virtual void GenerateDrawCommands(FDrawCommand* CommandsBuffer, float TotalTime, uint32_t Width, uint32_t Height) {};
	};

	class UTestScene_Cube : public UTestSceneBase
	{
	public:

		int NumDrawCommands = 2;

		virtual void GenerateDrawCommands(FDrawCommand* CommandsBuffer, float TotalTime, uint32_t Width, uint32_t Height) override
		{
			// Cube 1
			{
				FMatrix4x4f Model = FMatrix4x4f::Translate({ 0.f, 0.f, -4.f })
					* FMatrix4x4f::RotateZX(TotalTime)
					* FMatrix4x4f::RotateXY(TotalTime * 1.61f);

				FMatrix4x4f Projection = FMatrix4x4f::Perspective(0.01f, 10.f, float(M_PI) / 3.f, Width * 1.f / Height);

				CommandsBuffer[0] =
				{
					.Mesh = Rasterizer::Cube,
					.CullMode = ECullMode::None,
					.Transform = Projection * Model,
				};
			}
			
			// Cube 2
			{
				FMatrix4x4f Model = FMatrix4x4f::Translate({ 0.f, 0.f, -4.f })
					* FMatrix4x4f::Scale(.5f)
					* FMatrix4x4f::RotateZX(-TotalTime)
					* FMatrix4x4f::RotateXY(TotalTime * .5f);

				FMatrix4x4f Projection = FMatrix4x4f::Perspective(0.01f, 10.f, float(M_PI) / 3.f, Width * 1.f / Height);

				CommandsBuffer[1] =
				{
					.Mesh = Rasterizer::Cube,
					.CullMode = ECullMode::None,
					.Transform = Projection * Model,
				};
			}
		}
	};

	class UTestScene_Rectangle : public UTestSceneBase
	{
	public:

		int NumDrawCommands = 1;

		virtual void GenerateDrawCommands(FDrawCommand* CommandsBuffer, float TotalTime, uint32_t Width, uint32_t Height) override
		{
			FMatrix4x4f Model = FMatrix4x4f::Translate({ 0.f, 0.f, -2.f })
				* FMatrix4x4f::RotateYZ(TotalTime);

			FMatrix4x4f Projection = FMatrix4x4f::Perspective(0.01f, 10.f, float(M_PI) / 3.f, Width * 1.f / Height);

			CommandsBuffer[0] =
			{
				.Mesh = Rasterizer::Rectangle,
				.CullMode = ECullMode::None,
				.Transform = Projection * Model,
			};
		}
	};
}