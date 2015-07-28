#include <iostream>
#include <string>
#include <chrono>

#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include "base/system.h"
#include "base/TexturePool.h"
#include "base/Unit.h"

const int SCREEN_FPS = 60;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

Mix_Music *gMusic = nullptr;

Mix_Chunk *gScratch = nullptr;
Mix_Chunk *gHigh = nullptr;
Mix_Chunk *gMedium = nullptr;
Mix_Chunk *gLow = nullptr;

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

 const auto * keys = SDL_GetKeyboardState(NULL);

  while(!done) {
    while(SDL_PollEvent(&e)) {
      if(e.type == SDL_QUIT) {
        done = true;
      }    
    }

    SDL_PumpEvents();

    if (keys[SDL_SCANCODE_UP]) {
      unit.add_speed(0, -200 * delta.count());
    }
    if (keys[SDL_SCANCODE_DOWN]) {
      unit.add_speed(0, 200 * delta.count());
    }
    if (keys[SDL_SCANCODE_LEFT]) {
      unit.add_speed(-200 * delta.count(), 0);
    }
    if (keys[SDL_SCANCODE_RIGHT]) {
      unit.add_speed(200 * delta.count(), 0);
    }
    if (keys[SDL_SCANCODE_W]) {
      unit.add_accel(0, -150 * delta.count());
    }
    if (keys[SDL_SCANCODE_S]) {
      unit.add_accel(0, 150 * delta.count());
    }
    if (keys[SDL_SCANCODE_A]) {
      unit.add_accel(-150 * delta.count(), 0);
    }
    if (keys[SDL_SCANCODE_D]) {
      unit.add_accel(150 * delta.count(), 0);
    }
    if (keys[SDL_SCANCODE_SPACE]) {
      unit.inv_speed(true, true);
    }

    while ((std::chrono::high_resolution_clock::now() - t2).count() < 0.0167f) {}

    delta = std::chrono::high_resolution_clock::now() - t2;
    t2 = std::chrono::high_resolution_clock::now();

    //Accel decay
    if (unit.get_accel().x < 0) {
      unit.add_accel(50 * delta.count(), 0);
    } else if ((unit.get_accel().x > 0)) {
      unit.add_accel(-50 * delta.count(), 0);
    }

    if (unit.get_accel().y < 0) {
      unit.add_accel(0 , 50 * delta.count());
    } else if ((unit.get_accel().y > 0)) {
      unit.add_accel(0, -50 * delta.count());
    }

    //Bounds on acceleration
    if (unit.get_accel().x < -400) {
      unit.set_accel(-400, unit.get_accel().y);
    } else if (unit.get_accel().x > 400) {
      unit.set_accel(400, unit.get_accel().y);
    }

    if (unit.get_accel().y < -400) {
      unit.set_accel(unit.get_accel().x, -400);
    } else if (unit.get_accel().y > 400) {
      unit.set_accel(unit.get_accel().x, 400);
    }

    unit.add_speed(unit.get_accel().x * delta.count(), unit.get_accel().y * delta.count());

    //Speed decay
    if (unit.get_speed().x < 0) {
      unit.add_speed(70 * delta.count(), 0);
    } else if ((unit.get_speed().x > 0)) {
      unit.add_speed(-70 * delta.count(), 0);
    }

    if (unit.get_speed().y < 0) {
      unit.add_speed(0, 70 * delta.count());
    } else if ((unit.get_speed().y > 0)) {
      unit.add_speed(0, -70 * delta.count());
    }

    //Bounds on speed
    if (unit.get_speed().x < -400) {
      unit.set_speed(-400, unit.get_speed().y);
    } else if (unit.get_speed().x > 400) {
      unit.set_speed(400, unit.get_speed().y);
    }

    if (unit.get_speed().y < -400) {
      unit.set_speed(unit.get_speed().x, -400);
    } else if (unit.get_speed().y > 400) {
      unit.set_speed(unit.get_speed().x, 400);
    }

    unit.add_pos(unit.get_speed().x * delta.count(), unit.get_speed().y * delta.count());

    //Bounds on position
    if (unit.get_pos().x < 0) {
      unit.set_pos(0, unit.get_pos().y);
      unit.inv_accel(true, false);
      unit.set_speed(-unit.get_speed().x / 4, unit.get_speed().y);
    } else if (unit.get_pos().x > 1280-32) {
      unit.set_pos(1280 - 32, unit.get_pos().y);
      unit.inv_accel(true, false);
      unit.set_speed(-unit.get_speed().x / 4, unit.get_speed().y);
    }

    if (unit.get_pos().y < 0) {
      unit.set_pos(unit.get_pos().x, 0);
      unit.inv_accel(false, true);
      unit.set_speed(unit.get_speed().x, -unit.get_speed().y / 4);
    } else if (unit.get_pos().y > 720-32) {
      unit.set_pos(unit.get_pos().x, 720-32);
      unit.inv_accel(false, true);
      unit.set_speed(unit.get_speed().x, -unit.get_speed().y / 4);
    }

    if (std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count() >= 1000) {
      std::cout << "DELTA: " << delta.count() << std::endl;
      std::cout << "FPS: " << fps << std::endl;
      std::cout << "POS (X,Y) " << "(" << unit.get_pos().x << "," << unit.get_pos().y << ")" << std::endl;
      std::cout << "SPEED (X,Y) " << "(" << unit.get_speed().x << "," << unit.get_speed().y << ")" << std::endl;
      std::cout << "ACCEL (X,Y) " << "(" << unit.get_accel().x << "," << unit.get_accel().y << ")\n" << std::endl;

      fps = 0;
      t1 = t2;
    }

    //Render unit and background
    SDL_RenderClear(base_system.get_renderer());
    SDL_RenderCopy(base_system.get_renderer(), tp.get("bg.png"), nullptr, nullptr);
    SDL_RenderCopy(base_system.get_renderer(), unit.get_texture(), nullptr, unit.get_rect());
    SDL_RenderPresent(base_system.get_renderer());

    ++fps;
  }

  std::cout << "hi";
  return 0;
}