#pragma once

#include "Renderer.hpp"
#include "Color.hpp"
#include "TriangleClipper.hpp"

#include "Util/Memory.hpp"
#include "Util/Validation.hpp"

namespace Rasterizer
{
	void Clear(FRenderTarget& RenderTarget, const FVector4f& Color, const uint32_t& Depth)
	{
		RenderTarget.FrameBuffer.Color.Fill(FColor4ub::FromVector4F(Color));
		RenderTarget.FrameBuffer.Depth.Fill(Depth);
	}

	void Draw(const FRenderTarget& RenderTarget, const FDrawCommand& Command)
	{
		auto ShouldTriDraw = [](bool bTriIsClockwise, const FDrawCommand& Command) -> bool
			{
				switch (Command.CullMode)
				{
				case ECullMode::None:				return true;
				case ECullMode::Clockwise:			return bTriIsClockwise;
				case ECullMode::CounterClockWise:	return !bTriIsClockwise;
				default:
					ENSURE(false, "Missing logic for cullmode");
					return true;
				}
			};

		auto IsLeftOrTopEdge = [](const FVector4f& Start, const FVector4f& End) -> bool
			{
				bool bIsLeft = End.Y - Start.Y > 0;
				bool bIsTop = End.Y - Start.Y == 0 && End.X - Start.X <= 0;
				return bIsLeft || bIsTop;
			};

		auto EvaluateDepthTest = [](EDepthPassMode DepthMode, uint32_t CurrDepth, uint32_t PrevDepth) -> bool
			{
				switch (DepthMode)
				{
				case Rasterizer::EDepthPassMode::Never:			return false;
				case Rasterizer::EDepthPassMode::Always:		return true;
				case Rasterizer::EDepthPassMode::Less:			return CurrDepth < PrevDepth;
				case Rasterizer::EDepthPassMode::LessEqual:		return CurrDepth <= PrevDepth;
				case Rasterizer::EDepthPassMode::Greater:		return CurrDepth > PrevDepth;
				case Rasterizer::EDepthPassMode::GreaterEqual:	return CurrDepth >= PrevDepth;
				case Rasterizer::EDepthPassMode::Equal:			return CurrDepth == PrevDepth;
				case Rasterizer::EDepthPassMode::NotEqual:		return CurrDepth != PrevDepth;
				default:
					ENSURE_EXIT(false, "No logic defined for depth mode") false;
				}
			};

		// Loop through groups of three vertices that make up a Triangle without overshooting
		for (size_t VertexIndex = 0; VertexIndex + 2 < Command.Mesh.Count; VertexIndex += 3)
		{
			size_t VertexIndex0 = VertexIndex + 0;
			size_t VertexIndex1 = VertexIndex + 1;
			size_t VertexIndex2 = VertexIndex + 2;

			if (nullptr != Command.Mesh.Indices)
			{
				VertexIndex0 = Command.Mesh.Indices[VertexIndex0];
				VertexIndex1 = Command.Mesh.Indices[VertexIndex1];
				VertexIndex2 = Command.Mesh.Indices[VertexIndex2];
			}

			FVertex UnclippedV0, UnclippedV1, UnclippedV2;

			UnclippedV0.Position = Command.Transform * Command.Mesh.Positions[VertexIndex0].AsVector4f(1.f);
			UnclippedV1.Position = Command.Transform * Command.Mesh.Positions[VertexIndex1].AsVector4f(1.f);
			UnclippedV2.Position = Command.Transform * Command.Mesh.Positions[VertexIndex2].AsVector4f(1.f);
			
			UnclippedV0.Color = Command.Mesh.Colors[VertexIndex0];
			UnclippedV1.Color = Command.Mesh.Colors[VertexIndex1];
			UnclippedV2.Color = Command.Mesh.Colors[VertexIndex2];

			FTriangleClipper TriangleClipper = FTriangleClipper(UnclippedV0, UnclippedV1, UnclippedV2);
			TriangleClipper.Clip();

			for (FVertex* TrianglesBegin = TriangleClipper.ClippedVerticesStartItr; TrianglesBegin != TriangleClipper.ClippedVerticesEndItr; TrianglesBegin += 3)
			{
				FVertex v0 = TrianglesBegin[0];
				FVertex v1 = TrianglesBegin[1];
				FVertex v2 = TrianglesBegin[2];
			
				// Apply perspective divide transform to our vertices, dividing the vector 4 by its W value
				v0.Position = v0.Position.PerspectiveDivide();
				v1.Position = v1.Position.PerspectiveDivide();
				v2.Position = v2.Position.PerspectiveDivide();

				v0.Position = RenderTarget.Viewport.ViewportToScreenCoords(v0.Position);
				v1.Position = RenderTarget.Viewport.ViewportToScreenCoords(v1.Position);
				v2.Position = RenderTarget.Viewport.ViewportToScreenCoords(v2.Position);

				float Determinant012 = Determinant2D(v1.Position - v0.Position, v2.Position - v0.Position);
				const bool bIsCounterClockwise = Determinant012 < 0.f;

				if (!ShouldTriDraw(bIsCounterClockwise, Command))
				{
					continue;
				}

				// Ensure we're always drawing the Triangle clockwise, even if it originally came in as ccw and was determined to still be allowed to draw.
				if (bIsCounterClockwise)
				{
					Memory::Swap(v1, v2);
					Determinant012 = -Determinant012;
				}

				// Clamp to within screen/pixel coords, in case the Viewport is larger than the screen
				int32_t xMin = Math::Max<int32_t>(0, RenderTarget.Viewport.xMin);
				int32_t yMin = Math::Max<int32_t>(0, RenderTarget.Viewport.yMin);
				int32_t xMax = Math::Min<int32_t>(RenderTarget.FrameBuffer.GetWidth(), RenderTarget.Viewport.xMax) - 1;
				int32_t yMax = Math::Min<int32_t>(RenderTarget.FrameBuffer.GetHeight(), RenderTarget.Viewport.yMax) - 1;

				// Clamp to within AABB of the mesh
				xMin = Math::Min({ Math::Floor(v0.Position.X),	Math::Floor(v1.Position.X),	Math::Floor(v2.Position.X)	});
				yMin = Math::Min({ Math::Floor(v0.Position.Y),	Math::Floor(v1.Position.Y),	Math::Floor(v2.Position.Y)	});
				xMax = Math::Max({ Math::Ceil(v0.Position.X),	Math::Ceil(v1.Position.X),	Math::Ceil(v2.Position.X)	});
				yMax = Math::Max({ Math::Ceil(v0.Position.Y),	Math::Ceil(v1.Position.Y),	Math::Ceil(v2.Position.Y)	});

				// Clamp to color buffer bounds
				xMin = Math::Max<int32_t>(xMin, 0);
				yMin = Math::Max<int32_t>(yMin, 0);
				xMax = Math::Min<int32_t>(xMax, RenderTarget.FrameBuffer.GetWidth());
				yMax = Math::Min<int32_t>(yMax, RenderTarget.FrameBuffer.GetHeight());

				//// Pre-calc if any edges are top or left
				//const bool b01IsTopOrLeftEdge = IsLeftOrTopEdge(v0, v1);
				//const bool b12IsTopOrLeftEdge = IsLeftOrTopEdge(v1, v2);
				//const bool b20IsTopOrLeftEdge = IsLeftOrTopEdge(v2, v0);

				for (int32_t y = yMin; y < yMax; y++)
				{
					for (int32_t x = xMin; x < xMax; x++)
					{
						FVector4f Point {x + 0.5f, y + 0.5f, 0.f, 0.f};

						float Determinant01ToPoint = Determinant2D(v1.Position - v0.Position, Point - v0.Position);
						float Determinant12ToPoint = Determinant2D(v2.Position - v1.Position, Point - v1.Position);
						float Determinant20ToPoint = Determinant2D(v0.Position - v2.Position, Point - v2.Position);

						//// Fill rule to fix multiple tris with overlapping edges. Uses the "top left" fill rule - https://kristoffer-dyrkorn.github.io/Triangle-rasterizer/4
						//Determinant01ToPoint = b01IsTopOrLeftEdge ? Determinant01ToPoint - 1.f : Determinant01ToPoint;
						//Determinant12ToPoint = b12IsTopOrLeftEdge ? Determinant12ToPoint - 1.f : Determinant12ToPoint;
						//Determinant20ToPoint = b20IsTopOrLeftEdge ? Determinant20ToPoint - 1.f : Determinant20ToPoint;

						if (Determinant01ToPoint >= 0.f && Determinant12ToPoint >= 0.f && Determinant20ToPoint >= 0.f)
						{
							// Calculates how much each vertex contributes to the current pixel's attributes
							float V0Coefficient = Determinant12ToPoint / Determinant012 * v0.Position.W;
							float V1Coefficient = Determinant20ToPoint / Determinant012 * v1.Position.W;
							float V2Coefficient = Determinant01ToPoint / Determinant012 * v2.Position.W;

							// Normalize the interpolated coefficients
							float CoefficientMagnitude = V0Coefficient + V1Coefficient + V2Coefficient;
							V0Coefficient /= CoefficientMagnitude;
							V1Coefficient /= CoefficientMagnitude;
							V2Coefficient /= CoefficientMagnitude;

							// Determine if this pixel should draw based on the depth buffer
							if (RenderTarget.FrameBuffer.Depth.IsValid())
							{
								float CurrPixelZ = v0.Position.Z * V0Coefficient + v1.Position.Z * V1Coefficient + v2.Position.Z * V2Coefficient;

								// Convert from [-1, 1] to [0, UINT32_MAX]
								uint32_t CurrDepth = (0.5f + 0.5f * CurrPixelZ) * UINT32_MAX;

								uint32_t& OldDepth = RenderTarget.FrameBuffer.Depth.GetPixelAtPos(x, y);
								if (!EvaluateDepthTest(Command.DepthPassSettings.Mode, CurrDepth, OldDepth))
								{
									continue;
								}

								if (Command.DepthPassSettings.bWrite)
								{
									OldDepth = CurrDepth;
								}
							}

							FColor4ub Color = FColor4ub::FromVector4F(v0.Color * V0Coefficient + v1.Color * V1Coefficient + v2.Color * V2Coefficient);

							ENSURE_EXIT(RenderTarget.FrameBuffer.Color.IsValid());
							RenderTarget.FrameBuffer.Color.GetPixelAtPos(x, y) = Color;
						}
					}
				}
			}
		}
	}
}