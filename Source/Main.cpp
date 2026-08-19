#include <SDL3/SDL.h>

#define _USE_MATH_DEFINES 1

#include <chrono>
#include <iostream>

#include "Rasterizer/Renderer.hpp"

#include "Rasterizer/Shapes/Cube.hpp"
#include "Rasterizer/Shapes/Triangle.hpp"
#include "Rasterizer/Shapes/Rectangle.hpp"

using namespace Rasterizer;

int main()
{
	SDL_Init(SDL_INIT_VIDEO);

	uint32_t Width = 800;
	uint32_t Height = 600;

	SDL_Window* Window = SDL_CreateWindow("CPU Rasterizer", Width, Height, SDL_WINDOW_RESIZABLE);
	SDL_Surface* DrawSurface = nullptr;

	float MouseX = 0;
	float MouseY = 0;

	using Clock = std::chrono::high_resolution_clock;
	auto LastFrameStart = Clock::now();

	float TotalTime = 0;

	bool bRunning = true;
	while (bRunning)
	{
		// Handle SDL Events
		for (SDL_Event Event; SDL_PollEvent(&Event);)
		{
			switch (Event.type)
			{
				case SDL_EVENT_WINDOW_RESIZED:
					if (nullptr != DrawSurface)
					{
						SDL_DestroySurface(DrawSurface);
						DrawSurface = nullptr;
					}
					Width = Event.window.data1;
					Height = Event.window.data2;
					break;

				case SDL_EVENT_QUIT:
					bRunning = false;
					break;

				case SDL_EVENT_MOUSE_MOTION:
					MouseX = Event.motion.x;
					MouseY = Event.motion.y;
					break;

				case SDL_EVENT_KEY_DOWN:
					if (SDLK_ESCAPE == Event.key.key)
					{
						bRunning = false;
					}
					break;
			}
		}

		auto Now = Clock::now();
		float DeltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(Now - LastFrameStart).count();
		LastFrameStart = Now;
		TotalTime += DeltaTime;
		std::cout << DeltaTime << " -- " << TotalTime << std::endl;

		// If we're still running after handling SDL events, render the frame
		if (bRunning)
		{
			if (nullptr == DrawSurface)
			{
				DrawSurface = SDL_CreateSurface(Width, Height, SDL_PIXELFORMAT_RGBA32);
				SDL_SetSurfaceBlendMode(DrawSurface, SDL_BLENDMODE_NONE);
			}

			FRenderTarget RenderTarget
			{
				// FImageView
				{
					(FColor4ub*)DrawSurface->pixels,
					Width,
					Height
				},
				// FViewport
				{
					0,
					RenderTarget.ColorBuffer.Width,
					0,
					RenderTarget.ColorBuffer.Height,
				}
			};

			Rasterizer::Clear(RenderTarget, { 0.9f, 0.9f, 0.9f, 1.f });

			FMatrix4x4f Model = FMatrix4x4f::Translate({0.f, 0.f, -4.f})
									* FMatrix4x4f::RotateZX(TotalTime)
									* FMatrix4x4f::RotateXY(TotalTime * 1.61f);

			FMatrix4x4f Projection = FMatrix4x4f::Perspective(4.f, 10.f, M_PI / 3.f, Width * 1.f / Height);

			FDrawCommand DrawCommand
			{
				// Mesh
				{
					//Rasterizer::Triangle
					Rasterizer::Cube
					//Rasterizer::Rectangle
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

			Rasterizer::Draw(RenderTarget, DrawCommand);

			SDL_Rect Rect
			{
				0, 0,			// Position
				Width, Height	// Dimensions
			};

			SDL_BlitSurface(DrawSurface, &Rect, SDL_GetWindowSurface(Window), &Rect);
			SDL_UpdateWindowSurface(Window);
		}
	}
}