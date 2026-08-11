#include <SDL3/SDL.h>

int main()
{
	SDL_Init(SDL_INIT_VIDEO);

	int width = 800;
	int height = 600;

	SDL_Window* Window = SDL_CreateWindow("CPU Rasterizer", width, height, SDL_WINDOW_RESIZABLE);

	int MouseX = 0;
	int MoustY = 0;

	bool bRunning = true;
	while (bRunning)
	{
		for (SDL_Event Event; SDL_PollEvent(&Event);)
		{
			switch (Event.type)
			{
				width = Event.window.data1;
				height = Event.window.data2;
				break;
			}
		}
	}
}