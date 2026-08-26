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

		int NumDrawCommands = 5;

		virtual void GenerateDrawCommands(FDrawCommand* CommandsBuffer, float TotalTime, uint32_t Width, uint32_t Height) override
		{
			for (int i = 0; i < 5; ++i)
			{
				FMatrix4x4f View = FMatrix4x4f::Translate({ 0.f, 0.f, -4.f });

				FMatrix4x4f Model = FMatrix4x4f::RotateZX(TotalTime)
									* FMatrix4x4f::RotateXY(TotalTime * 1.61f);

				FMatrix4x4f Location = FMatrix4x4f::Translate({float(i - 2), 0.f, 0.f});

				FMatrix4x4f Projection = FMatrix4x4f::Perspective(0.01f, 10.f, float(M_PI) / 3.f, Width * 1.f / Height);

				CommandsBuffer[i] =
				{
					.Mesh = Rasterizer::Cube,
					.CullMode = ECullMode::Clockwise,
					.Transform = Projection * View * Location * Model,
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