#include <iostream>
#include <string>
#include <chrono>

#include "SDL2/SDL.h"
#include "base/system.h"
#include "base/TexturePool.h"

const int SCREEN_FPS = 60;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

int main(int argc, char** argv) {
  BaseSystem base_system("SDL_Test!", 1280, 720, SDL_WINDOW_OPENGL);
  TexturePool tp(base_system.get_renderer(), "img");
  tp.load_texture("bg.png");
  tp.load_texture("square.png");


  SDL_Rect rekt;
  rekt.w = 32;
  rekt.h = 32;
  rekt.x = 1280/2 - 32;
  rekt.y = 720/2 - 32;

  float speedx = 0;
  float speedy = 0;

  float accelx = 0;
  float accely = 0;

  SDL_Event e;
  bool done = false;

  auto t1 = std::chrono::high_resolution_clock::now();
  auto t2 = std::chrono::high_resolution_clock::now();
  std::chrono::duration<float> delta = t2-t1;

  float fx = 1280/2 - 32;
  float fy = 720/2 - 32;

  int fps = 0;

  while(!done) {
    while(SDL_PollEvent(&e)) {
      if(e.type == SDL_QUIT) {
        done = true;
      }
  
      const Uint8* keystates = SDL_GetKeyboardState(NULL);
      if (keystates[SDL_SCANCODE_UP]) {
        accely -= 10;
        speedy -= 5;
      }
      if (keystates[SDL_SCANCODE_DOWN]) {
        accely += 10;
        speedy += 5;
      }
      if (keystates[SDL_SCANCODE_LEFT]) {
        accelx -= 10;
        speedx -= 5;
      }
      if (keystates[SDL_SCANCODE_RIGHT]) {
        accelx += 10;
        speedx += 5;
      }
    }

    if (accelx < 0) {
      accelx += 0.5;
    } else if (accelx > 0) {
      accelx -= 0.5;
    }

    if (accely < 0) {
      accely += 0.5;
    } else if (accely > 0) {
      accely -= 0.5;
    }

    if (accelx > 50) {
      accelx = 50;
    } else if (accelx < -50) {
      accelx = -50;
    }

    if (accely > 50) {
      accely = 50;
    } else if (accely < -50) {
      accely = -50;
    }

    

    if (rekt.x < 0) {
      fx = 0;
      speedx = -speedx / 3;
    } else if (rekt.x > 1280 - 32) {
      fx = 1280 - 32;
      speedx = -speedx / 3;
    }

    if (rekt.y < 0) {
      fy = 0;
      speedy = -speedy / 3;
    } else if (rekt.y > 720 - 32) {
      fy = 720 - 32;
      speedy = -speedy / 3;
    }

    while ((std::chrono::high_resolution_clock::now() - t2).count() < 0.0166666f) {}

    delta = std::chrono::high_resolution_clock::now() - t2;
    t2 = std::chrono::high_resolution_clock::now();

    if (std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count() >= 1000) {
      std::cout << "DELTA: " << delta.count() << std::endl;
      std::cout << "FPS: " << fps << std::endl;
      std::cout << "FX: " << fx << " | FY: " << fy << std::endl;
      std::cout << "ACCEL (X,Y) " << "(" << accelx << "," << accely << ")\n" << std::endl;
      fps = 0;
      t1 = t2;
    }

    speedx += accelx * delta.count();
    speedy += accely * delta.count();

    if (speedx < 0) {
      speedx += 0.2;
    } else if (speedx > 0) {
      speedx -= 0.2;
    }

    if (speedy < 0) {
      speedy += 0.2;
    } else if (speedy > 0) {
      speedy -= 0.2;
    }

    if (speedx > 200) {
      speedx = 200;
    } else if (speedx < -200) {
      speedx = -200;
    }

    if (speedy > 200) {
      speedy = 200;
    } else if (speedy < -200) {
      speedy = -200;
    }



    fx += speedx * delta.count();
    fy += speedy * delta.count();

    rekt.x = fx;
    rekt.y = fy;

    SDL_RenderClear(base_system.get_renderer());
    SDL_RenderCopy(base_system.get_renderer(), tp.get("bg.png"), nullptr, nullptr);
    SDL_RenderCopy(base_system.get_renderer(), tp.get("square.png"), nullptr, &rekt);
    SDL_RenderPresent(base_system.get_renderer());

    ++fps;
  }

  std::cout << "hi";
  return 0;
}