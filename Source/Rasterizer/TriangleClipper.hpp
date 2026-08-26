#pragma once

#include "Util/Validation.hpp"

namespace Rasterizer
{
	struct FVertex
	{
		FVector4f Position;
		FVector4f Color;
	};

	// Clipping a single Triangle by both the near and far plane could create at most 4 Triangles. 4 Tris * 3 vertices each = 12 total vertices
#define MAX_CLIPPED_TRIANGLE_VERTICES 12

	static const FVector4f ClippingPlanes[2] =
	{
		{0.f, 0.f,  1.f, 1.f}, // Near Clipping Plane
		{0.f, 0.f, -1.f, 1.f}, // Far Clipping Plane
	};

	inline uint32_t GetNextVertexIndex(const uint32_t& CurrIndex)
	{
		uint32_t Out = CurrIndex;
		Out += 1;
		return Out % 3;
	}

	inline uint32_t GetPreviousVertexIndex(const uint32_t& CurrIndex)
	{
		int8_t Out = CurrIndex;
		Out -= 1;
		return Out >= 0 ? Out : 2;
	}

	struct FTriangleClipper
	{
		FTriangleClipper(FVertex InitialVert0, FVertex InitialVert1, FVertex InitialVert2)
		{
			ClippedVertices[0] = InitialVert0;
			ClippedVertices[1] = InitialVert1;
			ClippedVertices[2] = InitialVert2;

			ClippedVerticesStartItr = ClippedVertices;
			ClippedVerticesEndItr = ClippedVerticesStartItr + 3;
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
			NewVert.Position = (1.f - Alpha) * EdgeStart.Position + Alpha * EdgeEnd.Position;
			NewVert.Color = (1.f - Alpha) * EdgeStart.Color + Alpha * EdgeEnd.Color;

			return NewVert;
		}

		void Clip()
		{
			FVertex Result[MAX_CLIPPED_TRIANGLE_VERTICES];

			// Iterate through each clipping function
			for (const FVector4f& ClippingPlane : ClippingPlanes)
			{
				// Tracks the vertex we are currently evaluating for clipping
				FVertex* CurrentResultItr = Result;

				// Iterate through every Triangle
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
					for (int i = 0; i < 3; ++i)
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

						// Single vertex is clipped, CurrentResultItring in two new tris
					case 1:
					{

						uint8_t ClippedIndex = ClippedVertexIndices[0];

						uint8_t PrevIndex = GetPreviousVertexIndex(ClippedIndex);
						uint8_t NextIndex = GetNextVertexIndex(ClippedIndex);

						// Create new vertices
						FVertex NewVertForward = CreateVertexAlongClippedEdge(Triangle[ClippedIndex], Triangle[NextIndex], VertexDotProds[ClippedIndex], VertexDotProds[NextIndex]);
						FVertex NewVertBackwards = CreateVertexAlongClippedEdge(Triangle[ClippedIndex], Triangle[PrevIndex], VertexDotProds[ClippedIndex], VertexDotProds[PrevIndex]);

						// First new tri
						*CurrentResultItr++ = Triangle[PrevIndex];
						*CurrentResultItr++ = NewVertBackwards;
						*CurrentResultItr++ = Triangle[NextIndex];

						// Second new tri
						*CurrentResultItr++ = NewVertForward;
						*CurrentResultItr++ = Triangle[NextIndex];
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

						uint8_t PrevIndex = GetPreviousVertexIndex(UnchangedVertexIndex);
						uint8_t NextIndex = GetNextVertexIndex(UnchangedVertexIndex);

						*CurrentResultItr++ = CreateVertexAlongClippedEdge(Triangle[UnchangedVertexIndex], Triangle[PrevIndex], VertexDotProds[UnchangedVertexIndex], VertexDotProds[PrevIndex]);
						*CurrentResultItr++ = Triangle[UnchangedVertexIndex];
						*CurrentResultItr++ = CreateVertexAlongClippedEdge(Triangle[UnchangedVertexIndex], Triangle[NextIndex], VertexDotProds[UnchangedVertexIndex], VertexDotProds[NextIndex]);

						break;

					}
					// All vertices are clipped. Do nothing, causing CurrentResultItr to remain at the head, effectively culling the Triangle
					case 3:
						break;

					default:
						ENSURE(false, "Unhandled count of clipped tris!");
						break;
					}
				}
				// Copy all vertices between 
				ClippedVerticesEndItr = std::copy(Result, CurrentResultItr, ClippedVerticesStartItr); //Memory::Copy(CurrentResultItr, CurrentResultItr, ClippedVerticesStartItr);
			}
		}
	};
}