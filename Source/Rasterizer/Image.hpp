#pragma once

#include <cstdint>
#include <memory>

#include "Util/Validation.hpp"

namespace Rasterizer
{
	template <typename TPixel>
	struct FImage
	{
		TPixel* Pixels = nullptr;
		uint32_t Width = 0;
		uint32_t Height = 0;

		bool IsValid() const { return nullptr != Pixels; }
	
		template <typename TPixel>
		void Allocate(uint32_t InWidth, uint32_t InHeight)
		{
			ENSURE_EXIT(!IsValid(), "Attempting to allocate already allocated image.");

			Pixels = new TPixel[InWidth * InHeight];
			Width = InWidth;
			Height = InHeight;
		}

		void Deallocate()
		{
			ENSURE_EXIT(IsValid());
			delete[] Pixels;
			Pixels = nullptr;
			Width = 0;
			Height = 0;
		}

		TPixel& GetPixelAtPos(uint32_t x, uint32_t y) const
		{
			return Pixels[x + y * Width];
		}

		void Fill(TPixel FillValue)
		{
			ENSURE_EXIT(IsValid());
			for (uint32_t i = 0; i < Width * Height; ++i)
			{
				Pixels[i] = FillValue;
			}
		}
	};
}
