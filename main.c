#include <SDL2/SDL.h>
#include <stdbool.h>

#define WIN_WIDTH 1920
#define WIN_HEIGHT 1080

int main()
{
  if (SDL_Init(SDL_INIT_VIDEO)) {
    fprintf(stderr, "Error SDL init");
    return 1;
  }

  SDL_Window* window = SDL_CreateWindow("Procedural Texture",
                                        SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        WIN_WIDTH,
                                        WIN_HEIGHT,
                                        SDL_WINDOW_SHOWN);

  if (!window) {
    fprintf(stderr, "Could not create SDL window %s", SDL_GetError());
    return 1;
  }

  SDL_Renderer* ren = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  if (!ren) {
    fprintf(stderr, "Cound not creat Render %s", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }

  bool quit = false;
  while (!quit) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT: {
        quit = true;
      } break;
      }
    }

    SDL_RenderClear(ren);
    SDL_RenderPresent(ren);
  }

  // Destroy
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
