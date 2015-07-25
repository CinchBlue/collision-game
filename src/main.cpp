#include <iostream>
#include <string>
#include <chrono>

#include "SDL2/SDL.h"
#include "base/system.h"
#include "base/TexturePool.h"
#include "base/Unit.h"

const int SCREEN_FPS = 60;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

int main(int argc, char** argv) {
  BaseSystem base_system("SDL_Test!", 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  TexturePool tp(base_system.get_renderer(), "img");
  tp.load_texture("bg.png");
  tp.load_texture("square.png");

  Unit unit;
  unit.set_rect({1280/2 - 32, 720/2 - 32, 32, 32});
  unit.set_texture(tp.get("square.png"));
  unit.set_pos(1280/2 - 32, 720/2 - 32);
  unit.set_speed(0, 0);

  SDL_Event e;
  bool done = false;

  auto t1 = std::chrono::high_resolution_clock::now();
  auto t2 = std::chrono::high_resolution_clock::now();
  std::chrono::duration<float> delta = t2-t1;

  int fps = 0;

  while(!done) {
    while(SDL_PollEvent(&e)) {
      if(e.type == SDL_QUIT) {
        done = true;
      }
  
      const Uint8* keystates = SDL_GetKeyboardState(NULL);
      if (keystates[SDL_SCANCODE_UP]) {
        unit.add_speed(0, -10);
      }
      if (keystates[SDL_SCANCODE_DOWN]) {
        unit.add_speed(0, 10);
      }
      if (keystates[SDL_SCANCODE_LEFT]) {
        unit.add_speed(-10, 0);
      }
      if (keystates[SDL_SCANCODE_RIGHT]) {
        unit.add_speed(10, 0);
      }
      if (!(keystates[SDL_SCANCODE_UP] &&
            !keystates[SDL_SCANCODE_DOWN] &&
            !keystates[SDL_SCANCODE_LEFT] &&
            !keystates[SDL_SCANCODE_RIGHT])
         ) {
        //unit.set_speed(0, 0);
      }
    }    

    if (unit.get_speed().x < -200) {
      unit.set_speed(-200, unit.get_speed().y);
    } else if (unit.get_speed().x > 200) {
      unit.set_speed(200, unit.get_speed().y);
    }

    if (unit.get_speed().y < -200) {
      unit.set_speed(unit.get_speed().x, -200);
    } else if (unit.get_speed().y > 200) {
      unit.set_speed(unit.get_speed().x, 200);
    }

    while ((std::chrono::high_resolution_clock::now() - t2).count() < 0.0166666f) {}

    delta = std::chrono::high_resolution_clock::now() - t2;
    t2 = std::chrono::high_resolution_clock::now();

    if (std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count() >= 1000) {
      std::cout << "DELTA: " << delta.count() << std::endl;
      std::cout << "FPS: " << fps << std::endl;
      std::cout << "POS (X,Y) " << "(" << unit.get_pos().x << "," << unit.get_pos().y << ")" << std::endl;
      std::cout << "SPEED (X,Y) " << "(" << unit.get_speed().x << "," << unit.get_speed().y << ")" << std::endl;
      std::cout << "ACCEL (X,Y) " << "(" << unit.get_accel().x << "," << unit.get_accel().y << ")\n" << std::endl;

      auto rect = *unit.get_rect();

      std::cout << "X, Y, W , H: "
      <<  rect.x << ", "
      <<  rect.y << ", "
      <<  rect.w << ", "
      <<  rect.h << std::endl;

      fps = 0;
      t1 = t2;
    }

    unit.add_pos(unit.get_speed().x * delta.count(), 0);
    unit.add_pos(0, unit.get_speed().y * delta.count());

    if (unit.get_pos().x < 0) {
      unit.set_pos(0, unit.get_pos().y);
    } else if (unit.get_pos().x > 1280) {
      unit.set_pos(1280/2 - 32, unit.get_pos().y);
    }

    if (unit.get_pos().y < 0) {
      unit.set_pos(unit.get_pos().x, 0);
    } else if (unit.get_pos().y > 720) {
      unit.set_pos(unit.get_pos().x, 720-32);
    }

    SDL_RenderClear(base_system.get_renderer());
    SDL_RenderCopy(base_system.get_renderer(), tp.get("bg.png"), nullptr, nullptr);
    SDL_RenderCopy(base_system.get_renderer(), unit.get_texture(), nullptr, unit.get_rect());
    SDL_RenderPresent(base_system.get_renderer());

    ++fps;
  }

  std::cout << "hi";
  return 0;
}