#include <SDL3/SDL.h>

int main()
{
	SDL_Init(SDL_INIT_VIDEO);

	int width = 800;
	int height = 600;

	SDL_Window* Window = SDL_CreateWindow("CPU Rasterizer", width, height, SDL_WINDOW_RESIZABLE);
	SDL_Surface* DrawSurface = nullptr;

	int MouseX = 0;
	int MouseY = 0;

	bool bRunning = true;
	while (bRunning)
	{
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

		if (bRunning)
		{
			DrawSurface = SDL_CreateSurface(width, height, SDL_PIXELFORMAT_ABGR32);
			SDL_SetSurfaceBlendMode(DrawSurface, SDL_BLENDMODE_NONE);

			SDL_Rect Rect;
			Rect.x = 0;
			Rect.y = 0;
			Rect.w = width;
			Rect.h = height;

			uint32_t* Pixels = (uint32_t*)DrawSurface->pixels;
			for (int i = 0; i < width * height; i++)
			{
				Pixels[i] = 0xffffdfdf;
			}

			SDL_BlitSurface(DrawSurface, &Rect, SDL_GetWindowSurface(Window), &Rect);
			SDL_UpdateWindowSurface(Window);
		}
	}
}