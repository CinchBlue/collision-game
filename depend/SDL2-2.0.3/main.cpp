#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>

#include "SDL2/SDL.h"

int main(int argc, char** argv) {
  int hi = argc;
  char** hello = argv;

  SDL_Init(SDL_INIT_EVERYTHING);

  SDL_Window* window = SDL_CreateWindow("HI",
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED,
                                        800,
                                        600,
                                        SDL_WINDOW_OPENGL);

  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  SDL_Surface* surface = SDL_LoadBMP("circle.bmp");
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  SDL_Event e;
  bool done = false;
  while(!done) {
    while (SDL_PollEvent(&e)) {
      if(e.type == SDL_QUIT) {
        done = true;
      }
    }

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  SDL_Quit();

  return 0;

}