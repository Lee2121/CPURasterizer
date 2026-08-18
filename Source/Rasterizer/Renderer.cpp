#pragma once

#include "Renderer.hpp"
#include "Color.hpp"
#include "Util/Memory.hpp"
#include "Util/Validation.hpp"

namespace Rasterizer
{
	namespace
	{
		struct FVertex
		{
			FVector4f Position;
			FVector4f Color;
		};

		// Clipping a single triangle by both the near and far plane could create at most 4 triangles. 4 Tris * 3 vertices each = 12 total vertices
#define MAX_CLIPPED_TRIANGLE_VERTICES 12

		static const FVector4f ClippingPlanes[2] =
		{
			{0.f, 0.f,  1.f, 1.f}, // Z > -W  <=>   Z + W > 0 (Far Clipping Plane)
			{0.f, 0.f, -1.f, 1.f}, // Z <  W  <=> - Z + W > 0 (Near Clipping Plane)
		};

		struct FTriangleClipper
		{
			FTriangleClipper(FVertex InitialVert0, FVertex InitialVert1, FVertex InitialVert2)
			{
				ClippedVertices[0] = InitialVert0;
				ClippedVertices[1] = InitialVert1;
				ClippedVertices[2] = InitialVert2;
			}

			// The output
			FVertex ClippedVertices[MAX_CLIPPED_TRIANGLE_VERTICES];
			FVertex* ClippedVerticesStartItr = nullptr;
			FVertex* ClippedVerticesEndItr = nullptr;

			//  Flags which vertices should be clipped
			enum class EClippedVertices : uint8_t
			{
				None = 0b000,
				Vertex0 = 0b001,
				Vertex1 = 0b010,
				Vertex2 = 0b100,
				Vertex0and1 = 0b011,
				Vertex0and2 = 0b101,
				Vertex1and2 = 0b110,
				AllVertices = 0b111
			};

			FVertex CreateVertexAlongClippedEdge(const FVertex& EdgeStart, const FVertex& EdgeEnd, const float& EdgeStartDot, const float& EdgeEndDot)
			{
				float Alpha = EdgeStartDot / (EdgeStartDot - EdgeEndDot);

				FVertex NewVert;
				NewVert.Position = (1 - Alpha) * EdgeStart.Position + Alpha * EdgeEnd.Position;
				NewVert.Color = (1 - Alpha) * EdgeStart.Color + Alpha * EdgeEnd.Color;

				return NewVert;
			}

			void Clip()
			{
				ClippedVerticesStartItr = ClippedVertices;
				ClippedVerticesEndItr = ClippedVerticesStartItr + 3;

				FVertex Result[MAX_CLIPPED_TRIANGLE_VERTICES];

				// Iterate through each clipping function
				for (const FVector4f& ClippingPlane : ClippingPlanes)
				{
					// Tracks the vertex we are currently evaluating for clipping
					FVertex* CurrentResultItr = Result;

					// Iterate through every triangle
					for (FVertex* Triangle = ClippedVerticesStartItr; Triangle != ClippedVerticesEndItr; Triangle += 3)
					{
						float VertexDotProds[3] =
						{
							Triangle[0].Position.Dot(ClippingPlane),
							Triangle[1].Position.Dot(ClippingPlane),
							Triangle[2].Position.Dot(ClippingPlane),
						};

						uint8_t NumClippedVertices = 0;
						uint8_t ClippedVertexIndices[3];
						for(int i = 0; i < 3; ++i)
						{
							if (VertexDotProds[i] < 0.f)
							{
								ClippedVertexIndices[NumClippedVertices] = i;
								NumClippedVertices++;
							}
						}

						switch (NumClippedVertices)
						{

						// No clipping needed. All vertices can be used as is
						case 0:

							*CurrentResultItr++ = Triangle[0];
							*CurrentResultItr++ = Triangle[1];
							*CurrentResultItr++ = Triangle[2];

							break;

						// Single vertex is clipped, resulting in two new tris
						case 1:
						{
							uint8_t ClippedIndex = ClippedVertexIndices[0];

							uint8_t PrevIndex = (ClippedIndex - 1) >= 0 ? (ClippedIndex - 1) : 2;
							uint8_t NextIndex = (ClippedIndex + 1) % 3;

							// Create new vertices
							FVertex NewVertForward = CreateVertexAlongClippedEdge(Triangle[ClippedIndex], Triangle[NextIndex], VertexDotProds[ClippedIndex], VertexDotProds[NextIndex]);
							FVertex NewVertBackwards = CreateVertexAlongClippedEdge(Triangle[ClippedIndex], Triangle[PrevIndex], VertexDotProds[ClippedIndex], VertexDotProds[PrevIndex]);

							// First new tri
							*CurrentResultItr++ = Triangle[PrevIndex];
							*CurrentResultItr++ = NewVertForward;
							*CurrentResultItr++ = Triangle[NextIndex];

							// Second new tri
							*CurrentResultItr++ = NewVertForward;
							*CurrentResultItr++ = Triangle[PrevIndex];
							*CurrentResultItr++ = NewVertBackwards;

							break;
						}
						// Two vertices are clipped
						case 2:
						{

							uint8_t PossibleVertexIndices[3] = { 0, 1, 2 };
							uint8_t UnchangedVertexIndex;
							for (int i = 0; i < 3; ++i)
							{
								if (PossibleVertexIndices[i] != ClippedVertexIndices[0] && PossibleVertexIndices[i] != ClippedVertexIndices[1])
								{
									UnchangedVertexIndex = i;
									break;
								}
							}

							*CurrentResultItr++ = CreateVertexAlongClippedEdge(Triangle[ClippedVertexIndices[0]], Triangle[UnchangedVertexIndex], VertexDotProds[ClippedVertexIndices[0]], VertexDotProds[UnchangedVertexIndex]);
							*CurrentResultItr++ = Triangle[UnchangedVertexIndex];
							*CurrentResultItr++ = CreateVertexAlongClippedEdge(Triangle[ClippedVertexIndices[1]], Triangle[UnchangedVertexIndex], VertexDotProds[ClippedVertexIndices[1]], VertexDotProds[UnchangedVertexIndex]);
							
							break;

						}
						// All vertices are clipped. Do nothing, causing CurrentResultItr to remain at the head, effectively culling the triangle
						case 3:
							break;

						default:
							ENSURE(false, "Unhandled count of clipped tris!");
							break;
						}
					}
	
					// Copy all vertices between 
					ClippedVerticesEndItr = std::copy(Result, CurrentResultItr, ClippedVerticesStartItr); //Memory::Copy(Result, CurrentResultItr, ClippedVerticesStartItr);
				}
			}
		};

		// Receive an input triangle
		// Check if any vertex is closer then near clip or far clip (> 1 or < -1)
		// If it is, find where the line crosses the clipping plane, and dynamically create a new triangle based on that new point
	}

	void Clear(const FRenderTarget& RenderTarget, const FVector4f& Color)
	{
		FColor4ub* Pixels = RenderTarget.ColorBuffer.Pixels;
		uint32_t Size = RenderTarget.ColorBuffer.Width * RenderTarget.ColorBuffer.Height;
		for (uint32_t i = 0; i < Size; i++)
		{
			Pixels[i] = FColor4ub::FromVector4F(Color);
		}
	}

	void Draw(const FRenderTarget& RenderTarget, const FDrawCommand& Command)
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

		auto IsLeftOrTopEdge = [](const FVector4f& Start, const FVector4f& End) -> bool
			{
				bool bIsLeft = End.Y - Start.Y > 0;
				bool bIsTop = End.Y - Start.Y == 0 && End.X - Start.X <= 0;
				return bIsLeft || bIsTop;
			};

		// Loop through groups of three vertices that make up a triangle without overshooting
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
			UnclippedV1.Color = Command.Mesh.Colors[VertexIndex0];  
			UnclippedV2.Color = Command.Mesh.Colors[VertexIndex0];

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

				if (bIsCounterClockwise)
				{
					Memory::Swap(v1, v2);
					Determinant012 = -Determinant012;
				}

				// Clamp to within screen/pixel coords, in case the Viewport is larger than the screen
				int32_t xMin = Math::Max<int32_t>(0, RenderTarget.Viewport.xMin);
				int32_t yMin = Math::Max<int32_t>(0, RenderTarget.Viewport.yMin);
				int32_t xMax = Math::Min<int32_t>(RenderTarget.ColorBuffer.Width, RenderTarget.Viewport.xMax) - 1;
				int32_t yMax = Math::Min<int32_t>(RenderTarget.ColorBuffer.Height, RenderTarget.Viewport.yMax) - 1;

				// Clamp to within AABB of the mesh
				xMin = Math::Min({ Math::Floor(v0.Position.X),	Math::Floor(v1.Position.X),	Math::Floor(v2.Position.X)	});
				yMin = Math::Min({ Math::Floor(v0.Position.Y),	Math::Floor(v1.Position.Y),	Math::Floor(v2.Position.Y)	});
				xMax = Math::Max({ Math::Ceil(v0.Position.X),	Math::Ceil(v1.Position.X),	Math::Ceil(v2.Position.X)	});
				yMax = Math::Max({ Math::Ceil(v0.Position.Y),	Math::Ceil(v1.Position.Y),	Math::Ceil(v2.Position.Y)	});

				// Clamp to color buffer bounds
				xMin = Math::Max<int32_t>(xMin, 0);
				yMin = Math::Max<int32_t>(yMin, 0);
				xMax = Math::Min<int32_t>(xMax, RenderTarget.ColorBuffer.Width);
				yMax = Math::Min<int32_t>(yMax, RenderTarget.ColorBuffer.Height);

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

						//// Fill rule to fix multiple tris with overlapping edges. Uses the "top left" fill rule - https://kristoffer-dyrkorn.github.io/triangle-rasterizer/4
						//Determinant01ToPoint = b01IsTopOrLeftEdge ? Determinant01ToPoint - 1.f : Determinant01ToPoint;
						//Determinant12ToPoint = b12IsTopOrLeftEdge ? Determinant12ToPoint - 1.f : Determinant12ToPoint;
						//Determinant20ToPoint = b20IsTopOrLeftEdge ? Determinant20ToPoint - 1.f : Determinant20ToPoint;

						if (Determinant01ToPoint >= 0.f && Determinant12ToPoint >= 0.f && Determinant20ToPoint >= 0.f)
						{
							float Color0Alpha = Determinant12ToPoint / Determinant012;
							float Color1Alpha = Determinant20ToPoint / Determinant012;
							float Color2Alpha = Determinant01ToPoint / Determinant012;

							RenderTarget.ColorBuffer.GetPixelAtPos(x, y) = FColor4ub::FromVector4F(v0.Color * Color0Alpha + v1.Color * Color1Alpha + v2.Color * Color2Alpha);
						}
					}
				}
			}
		}
	}
}