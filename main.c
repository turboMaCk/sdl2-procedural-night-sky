#include <SDL2/SDL.h>
#include <stdbool.h>

#define WIN_WIDTH 1920
#define WIN_HEIGHT 1080

void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
  int bpp = surface->format->BytesPerPixel;
  /* Here p is the address to the pixel we want to set */
  Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

  switch(bpp) {
  case 1:
    *p = pixel;
    break;

  case 2:
    *(Uint16 *)p = pixel;
    break;

  case 3:
    if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
      p[0] = (pixel >> 16) & 0xff;
      p[1] = (pixel >> 8) & 0xff;
      p[2] = pixel & 0xff;
    } else {
      p[0] = pixel & 0xff;
      p[1] = (pixel >> 8) & 0xff;
      p[2] = (pixel >> 16) & 0xff;
    }
    break;

  case 4:
    *(Uint32 *)p = pixel;
    break;
  }
}

SDL_Surface* generate(int width, int height)
{
  SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA32);

  if (!surface) {
    fprintf(stderr, "SDL surface creating error %s", SDL_GetError());
    exit(1);
  }

  for (int i = 0; i < width; ++i) {
    if (i % 2 == 1) continue;
    for (int j = 0; j < height; ++j) {
      if (j % 2 == 1) continue;
      putpixel(surface, i, j, SDL_MapRGB(surface->format, 0xFF, 0xFF, 0xFF));
    }
  }

  return surface;
}

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
    fprintf(stderr, "Could not create SDL window %s\n", SDL_GetError());
    return 1;
  }

  SDL_Renderer* ren = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  SDL_Surface* generated_surface = generate(WIN_WIDTH, WIN_HEIGHT);
  SDL_Texture* texture = SDL_CreateTextureFromSurface(ren, generated_surface);

  if (!ren) {
    fprintf(stderr, "Cound not creat Render %s\n", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }

  SDL_RenderSetScale(ren, 2, 2);

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
    SDL_RenderCopy(ren, texture, NULL, NULL);
    SDL_RenderPresent(ren);
  }

  // Destroy
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
