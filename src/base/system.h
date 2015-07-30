#ifndef SYSTEM_H
#define SYSTEM_H

#include <exception>
#include <memory>
#include <iostream>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_mixer.h"



//WINDOW
class Window {
public:
  Window(const char* title, size_t w, size_t h) {
    //Create the window centered at 720p
    window = SDL_CreateWindow(title,
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              w,
                              h,
                              SDL_WINDOW_OPENGL);
    if (window == nullptr) {
      std::cerr << "Window creation error.\n";
      throw std::runtime_error(SDL_GetError());
    }
  }

  //Alternate constructor for custom flags
  Window(const char* title, size_t w, size_t h, Uint32 flags) {
    //Create the window centered at 720p
    window = SDL_CreateWindow(title,
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              w,
                              h,
                              flags);
    if (window == nullptr) {
      std::cerr << "Window creation error.\n";
      throw std::runtime_error(SDL_GetError());
    }
  }

  ~Window() {
    SDL_DestroyWindow(window);
  }

  SDL_Window* get() {
    return window;
  }

private:
  SDL_Window* window;
};


//RENDERER
class Renderer {
public:
  Renderer(SDL_Window* window) {
    renderer = SDL_CreateRenderer(window,
                                  -1,
                                  SDL_RENDERER_ACCELERATED |
                                  SDL_RENDERER_TARGETTEXTURE |
                                  SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
      std::cerr << "Renderer creation error.\n";
      throw std::runtime_error(SDL_GetError());
    } 
  }

  Renderer(const Renderer& other) = delete;
  Renderer& operator=(const Renderer& other) = delete;

  Renderer(Renderer&& other) {
    this->renderer = other.renderer;
    other.renderer = nullptr;
  }

  Renderer& operator=(Renderer&& other) noexcept {
    this->renderer = other.renderer;
    other.renderer = nullptr;
    return *this;
  }

  ~Renderer() {
    SDL_DestroyRenderer(renderer);
  }

  SDL_Renderer* get() {
    return renderer;
  }

  SDL_Renderer* renderer;
};

class BaseSystem {
public:
  BaseSystem(const char* title, size_t w, size_t h, Uint32 flags) {
      if(SDL_Init( SDL_INIT_EVERYTHING) < 0)
      {
        std::cout << "SDL initialization error.\n";
        throw std::runtime_error(SDL_GetError());
      }

      //Create the default renderer
      window = std::unique_ptr<Window>(new Window(title, w, h, flags)) ;
      renderer = std::unique_ptr<Renderer>(new Renderer(window.get()->get()));

      //SDL_image subsystem init
      int imgFlags = IMG_INIT_PNG;
      if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cerr << "Image loading subsystem initialization error.\n";
        throw std::runtime_error(IMG_GetError());
      }

      //SDL_ttf subsystem init
      if (TTF_Init() < 0) {
        std::cerr << "TTF subsystem initialization error.\n";
        throw std::runtime_error(TTF_GetError());
      }

      //SDL_mixer subsystem init
      int mixerFlags = MIX_INIT_MP3 | MIX_INIT_OGG;
      if ((Mix_Init(mixerFlags) & mixerFlags) != mixerFlags) {
        std::cerr << "Audio mixer subsystem initialization error.\n";
        throw std::runtime_error(Mix_GetError());
      }
  }

  ~BaseSystem() {
    IMG_Quit();
    TTF_Quit();
    Mix_Quit();
    SDL_Quit();
  }

  SDL_Window* get_window() const noexcept {
    return window.get()->get();
  }

  SDL_Renderer* get_renderer() const noexcept {
    return renderer.get()->get();
  }

private:
  std::unique_ptr<Window> window;
  std::unique_ptr<Renderer> renderer;
};

#endif //SYSTEM_H