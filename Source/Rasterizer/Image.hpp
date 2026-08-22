#pragma once

#include "ImageView.hpp"

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
		static FImage Allocate(uint32_t Width, uint32_t Height)
		{
			return FImage
			{
				.Pixels = new TPixel[Width * Height],
			};
		}

		void Deallocate()
		{
			ENSURE_EXIT(IsValid());
			delete Pixels;
			Width = 0;
			Height = 0;
		}

		template <typename TPixel>
		FImageView<TPixel> AsImageView()
		{
			ENSURE_EXIT(IsValid()) FImageView<TPixel>{};
			return FImageView<TPixel>
			{
				.Pixels = Pixels,
				.Width = Width,
				.Height = Height
			};
		}
	};
}
