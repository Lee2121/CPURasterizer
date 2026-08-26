#include <SDL3/SDL.h>

#include <chrono>
#include <iostream>

#include "Rasterizer/Renderer.hpp"
#include "Rasterizer/Image.hpp"
#include "TestScenes/TestScene.hpp"

using namespace Rasterizer;

int main()
{
	SDL_Init(SDL_INIT_VIDEO);

	uint32_t Width = 800;
	uint32_t Height = 600;

	FImage<uint32_t> DepthBuffer;

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
					if (DepthBuffer.IsValid())
					{
						DepthBuffer.Deallocate();
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

			if (!DepthBuffer.IsValid())
			{	
				DepthBuffer.Allocate<uint32_t>(Width, Height);
			}

			FRenderTarget RenderTarget
			{
				.FrameBuffer
				{
					.Color
					{
						.Pixels = (FColor4ub*)DrawSurface->pixels,
						.Width = Width,
						.Height = Height,
					},
					.Depth = DepthBuffer,
				},
				.Viewport
				{
					.xMin = 0,
					.xMax = Width,
					.yMin = 0,
					.yMax = Height,
				}
			};

			Rasterizer::Clear(RenderTarget, { 0.9f, 0.9f, 0.9f, 1.f }, UINT32_MAX);

			FDrawCommand DrawCommandsBuffer[MAX_DRAW_COMMANDS];

			TestScenes::UTestScene_Cube TestScene; 
			//TestScenes::UTestScene_Rectangle TestScene;
			
			TestScene.GenerateDrawCommands(DrawCommandsBuffer, TotalTime, Width, Height);

			for (int i = 0; i < TestScene.NumDrawCommands; ++i)
			{
				Rasterizer::Draw(RenderTarget, DrawCommandsBuffer[i]);
			}
			
			SDL_Rect Rect
			{
				0, 0,					// Position
				(int)Width, (int)Height	// Dimensions
			};

			SDL_BlitSurface(DrawSurface, &Rect, SDL_GetWindowSurface(Window), &Rect);
			SDL_UpdateWindowSurface(Window);
		}
	}
}