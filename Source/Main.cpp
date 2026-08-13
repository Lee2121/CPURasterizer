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

			FVector3f Tri1Vertices[] =
			{
				{700, 300.f, 0.f},
				{140.f, 40.f, 0.f},
				{80.f, 40.f, 0.f},
			};

			FMesh Triangle1Mesh;
			Triangle1Mesh.Color = {1.f, 0.f, 0.f, 1.f};
			Triangle1Mesh.Positions = Tri1Vertices;
			Triangle1Mesh.VertexCount = 3;

			FDrawCommand DrawTri1Command;
			DrawTri1Command.Mesh = Triangle1Mesh;
			DrawTri1Command.CullMode = ECullMode::CounterClockWise;

			FVector3f Tri2Vertices[] =
			{
				{700, 300.f, 0.f},
				{80.f, 40.f, 0.f},
				{50.f, 90.f, 0.f},
			};

			FMesh Triangle2Mesh;
			Triangle2Mesh.Color = { 0.f, 0.f, 1.f, 1.f };
			Triangle2Mesh.Positions = Tri2Vertices;
			Triangle2Mesh.VertexCount = 3;

			FDrawCommand DrawTri2Command;
			DrawTri2Command.Mesh = Triangle2Mesh;
			DrawTri2Command.CullMode = ECullMode::CounterClockWise;

			bool bUseFillRule = (Math::Floor(TotalTime) % 2 == 0);

			Rasterizer::Draw(ColorBuffer, DrawTri1Command);
			Rasterizer::Draw(ColorBuffer, DrawTri2Command);

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