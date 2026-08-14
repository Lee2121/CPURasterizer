#include <SDL3/SDL.h>

#include <chrono>
#include <iostream>

#include "Rasterizer/Renderer.hpp"

using namespace Rasterizer;

int main()
{
	SDL_Init(SDL_INIT_VIDEO);

	int width = 800;
	int height = 600;

	SDL_Window* Window = SDL_CreateWindow("CPU Rasterizer", width, height, SDL_WINDOW_RESIZABLE);
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
					width = Event.window.data1;
					height = Event.window.data2;
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
			DrawSurface = SDL_CreateSurface(width, height, SDL_PIXELFORMAT_RGBA32);
			SDL_SetSurfaceBlendMode(DrawSurface, SDL_BLENDMODE_NONE);

			FImageView ColorBuffer;
			ColorBuffer.Height = height;
			ColorBuffer.Width = width;
			ColorBuffer.Pixels = (FColor4ub*)DrawSurface->pixels;
			
			Rasterizer::Clear(ColorBuffer, { 0.f, 1.f, 1.f, 1.f });

			FMesh TriangleMesh;
			FVector3f Vertices[] =
			{
				{0.f, 0.f, 0.f},
				{100.f, 0.f, 0.f},
				{0.f, 100.f, 0.f},
			};

			FVector4f Colors[] =
			{
				{1.f, 0.f, 0.f, 1.f},
				{0.f, 1.f, 0.f, 1.f},
				{0.f, 0.f, 1.f, 1.f},
			};

			TriangleMesh.Positions = {Vertices};
			TriangleMesh.Colors = {Colors};
			TriangleMesh.VertexCount = 3;

			FDrawCommand DrawCommand;
			DrawCommand.CullMode = ECullMode::None;

			for (int TriIndex = 0; TriIndex < 100; ++TriIndex)
			{
				FMatrix4x4f TransformMatrix =
				{
					1.f, 0.f, 0.f, MouseX + 100.f * (TriIndex % 10),
					0.f, 1.f, 0.f, MouseY + 100.f * (TriIndex/ 10),
					0.f, 0.f, 1.f, 0.f,
					0.f, 0.f, 0.f, 1.f,
				};

				DrawCommand.Mesh = TriangleMesh;
				DrawCommand.Transform = TransformMatrix;

				Rasterizer::Draw(ColorBuffer, DrawCommand);
			}

			SDL_Rect Rect;
			Rect.x = 0;
			Rect.y = 0;
			Rect.w = width;
			Rect.h = height;

			SDL_BlitSurface(DrawSurface, &Rect, SDL_GetWindowSurface(Window), &Rect);
			SDL_UpdateWindowSurface(Window);
		}
	}
}