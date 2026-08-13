#pragma once

#include "Renderer.hpp"
#include "Util/Memory.hpp"
#include "Util/Validation.hpp"

namespace Rasterizer
{
	void Clear(const FImageView& ColorBuffer, const FVector4f& Color)
	{
		FColor4ub* Pixels = ColorBuffer.Pixels;
		uint32_t Size = ColorBuffer.Width * ColorBuffer.Height;
		for (uint32_t i = 0; i < Size; i++)
		{
			Pixels[i] = FColor4ub::FromVector4F(Color);
		}
	}

	void Draw(const FImageView& ColorBuffer, const FDrawCommand& Command)
	{
		auto ShouldTriDraw = [](bool bTriIsClockwise, const FDrawCommand& Command) -> bool
			{
				switch (Command.CullMode)
				{
				case ECullMode::None:				return true;
				case ECullMode::Clockwise:			return !bTriIsClockwise;
				case ECullMode::CounterClockWise:	return bTriIsClockwise;
				default:
					ENSURE(false, "Missing logic for cullmode");
					return true;
				}
			};

		// Loop through groups of three vertices that make up a triangle without overshooting
		for (uint32_t VertexIndex = 0; VertexIndex + 2 < Command.Mesh.VertexCount; VertexIndex += 3)
		{
			FVector4f v0 = Command.Mesh.Positions[VertexIndex + 0].AsVector4f(1.f);
			FVector4f v1 = Command.Mesh.Positions[VertexIndex + 1].AsVector4f(1.f);
			FVector4f v2 = Command.Mesh.Positions[VertexIndex + 2].AsVector4f(1.f);

			float Determinant012 = Determinant2D(v1 - v0, v2 - v0);
			const bool bIsCounterClockwise = Determinant012 < 0.f;

			if (!ShouldTriDraw(bIsCounterClockwise, Command))
			{
				continue;
			}

			if (bIsCounterClockwise)
			{
				Memory::Swap(v1, v2);
				Determinant012 = -Determinant012;
			}

			// Calc AABB of the mesh
			int32_t xMin = Math::Min({ Math::Floor(v0.X),	Math::Floor(v1.X),	Math::Floor(v2.X)	});
			int32_t yMin = Math::Min({ Math::Floor(v0.Y),	Math::Floor(v1.Y),	Math::Floor(v2.Y)	});
			int32_t xMax = Math::Max({ Math::Ceil(v0.X),	Math::Ceil(v1.X),	Math::Ceil(v2.X)	});
			int32_t yMax = Math::Max({ Math::Ceil(v0.Y),	Math::Ceil(v1.Y),	Math::Ceil(v2.Y)	});

			// Clamp to color buffer bounds
			xMin = Math::Max<int32_t>(xMin, 0);
			yMin = Math::Max<int32_t>(yMin, 0);
			xMax = Math::Min<int32_t>(xMax, ColorBuffer.Width);
			yMax = Math::Min<int32_t>(yMax, ColorBuffer.Height);

			for (int32_t y = yMin; y < yMax; y++)
			{
				for (int32_t x = xMin; x < xMax; x++)
				{
					FVector4f Point {x + 0.5f, y + 0.5f, 0.f, 0.f};

					float Determinant01ToPoint = Determinant2D(v1 - v0, Point - v0);
					float Determinant12ToPoint = Determinant2D(v2 - v1, Point - v1);
					float Determinant20ToPoint = Determinant2D(v0 - v2, Point - v2);

					if (Determinant01ToPoint >= 0.f && Determinant12ToPoint >= 0.f && Determinant20ToPoint >= 0.f)
					{
						ColorBuffer.GetPixelAtPos(x, y) = FColor4ub::FromVector4F(Command.Mesh.Color);
					}
				}
			}
		}
	}
}