#include <iostream>
#include <string>
#include <chrono>
#include <cstdlib>
#include <ctime>

#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include "base/system.h"
#include "base/TexturePool.h"
#include "base/Unit.h"
#include "Paddle.h"
#include "Ball.h"

/*
Mix_Music *gMusic = nullptr;

Mix_Chunk *gScratch = nullptr;
Mix_Chunk *gHigh = nullptr;
Mix_Chunk *gMedium = nullptr;
Mix_Chunk *gLow = nullptr;
*/
class Font {
public:
  Font(const char* path, size_t size = 12) {
    open_font(path, size);
  }

  ~Font() {
    TTF_CloseFont(font);
  }

 void open_font(const char* path, size_t size = 12) {
    font = TTF_OpenFont(path, size);
    if (font == nullptr) {
      std::cout << "Failed to load font from " << path << "; " << TTF_GetError() << std::endl;
      throw std::runtime_error(TTF_GetError());
    } else {
      return;
    }
  }

  TTF_Font* get_font() {return font;}

private:
  TTF_Font* font;
};  

class Text {
private:
  struct Dim2 {
    int x;
    int y;
  };
public:
  Text(SDL_Renderer* renderer, Font& font, SDL_Color color) :
    renderer(renderer),
    font(font),
    color(color) { }

  void render_text(std::string str) {
    SDL_Surface* text_surface = TTF_RenderText_Blended(font.get_font(), str.c_str(), color);
    if (text_surface == nullptr) {
      std::cout << "Unable to render text surface; text: " << str << "; " << TTF_GetError() << std::endl;
      SDL_FreeSurface(text_surface);
      throw std::runtime_error(TTF_GetError());
    } else {
      texture = SDL_CreateTextureFromSurface(renderer, text_surface);
      if (texture == nullptr) {
        std::cout << "Unable to create texture from text surface; text: " << str << "; " << SDL_GetError() << std::endl;
        SDL_FreeSurface(text_surface);
        throw std::runtime_error(SDL_GetError()); 
      } else {
        dim.x = text_surface->w;
        dim.y = text_surface->h;
        SDL_FreeSurface(text_surface);
      }
    }
  }

  SDL_Texture* get_texture() {return texture;}
  Dim2 get_dim() {return dim;}

private:
  Dim2 dim;
  std::string str;
  SDL_Renderer* renderer;
  Font& font;
  SDL_Color color;
  SDL_Texture* texture;
};

int check_ball(Ball& ball) {
  if (ball.get_pos().x < 8) {
    return -1;
  } else if (ball.get_pos().x > 1280 - 32 + 8) {
    return 1;
  }
  return 0;
}

int main(int argc, char** argv) {
  int p1score = 0;
  int p2score = 0;
  std::string score;

  std::srand(std::time(NULL));

  BaseSystem base_system("SDL_Test!", 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  TexturePool tp(base_system.get_renderer(), "img");
  tp.load_texture("bg.png");
  tp.load_texture("square.png");
  tp.load_texture("paddle.png");
  tp.load_texture("background.png");
  tp.load_texture("ball.png");

  Font dejavu_sans_mono("font/DejaVu_Sans/DejaVuSansMono.ttf", 42);
  Text text(base_system.get_renderer(), dejavu_sans_mono, {128, 128, 0, 128});
  SDL_Texture* text_texture;

  SDL_SetRenderDrawBlendMode(base_system.get_renderer(), SDL_BLENDMODE_BLEND);

  Paddle p2(32, 720/2 - 32, tp.get("paddle.png"));
  Paddle p1(1280 - 48, 720/2 - 32, tp.get("paddle.png"));
  Ball ball(1280/2 - 8, 720/2 - 8, tp.get("ball.png"));

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
      p1.set_speed(0, -20000 * delta.count());
    }
    if (keys[SDL_SCANCODE_DOWN]) {
      p1.set_speed(0, 20000 * delta.count());
    }
    if (keys[SDL_SCANCODE_LEFT]) {
      p1.set_speed(-20000 * delta.count(), 0);
    }
    if (keys[SDL_SCANCODE_RIGHT]) {
      p1.set_speed(20000 * delta.count(), 0);
    }

    if (!keys[SDL_SCANCODE_UP] &&
        !keys[SDL_SCANCODE_DOWN] &&
        !keys[SDL_SCANCODE_LEFT] &&
        !keys[SDL_SCANCODE_RIGHT]) {
      p1.set_speed(0, 0);
    }

    if (keys[SDL_SCANCODE_W]) {
      p2.set_speed(0, -20000 * delta.count());
    }
    if (keys[SDL_SCANCODE_S]) {
      p2.set_speed(0, 20000 * delta.count());
    }
    if (keys[SDL_SCANCODE_A]) {
      p2.set_speed(-20000 * delta.count(), 0);
    }
    if (keys[SDL_SCANCODE_D]) {
      p2.set_speed(20000 * delta.count(), 0);
    }

    if (!keys[SDL_SCANCODE_W] &&
        !keys[SDL_SCANCODE_S] &&
        !keys[SDL_SCANCODE_A] &&
        !keys[SDL_SCANCODE_D]) {
      p2.set_speed(0, 0);
    }

    if (keys[SDL_SCANCODE_SPACE]) {
      ball.set_accel(std::rand() % 300 - 150, std::rand() % 300 - 150);
    }

    if (keys[SDL_SCANCODE_KP_ENTER]) {
      ball.set_accel(std::rand() % 300 - 150, std::rand() % 300 - 150);
    }

    if (keys[SDL_SCANCODE_ESCAPE]) {
      ball.set_pos(1280/2 - 8, 720/2 - 8);
      ball.set_speed(0,0);
      ball.set_accel(0,0);
    }

    p1.update(delta.count());
    p2.update(delta.count());

    vec3 collision_info;
    float remaining_time = delta.count();
    
    collision_info = swept_collision(static_cast<Unit>(ball), p1, delta.count());
    if (collision_info.z < delta.count()/50) {
      remaining_time = 1.0f - collision_info.z;
      ball.update(collision_info.z * delta.count());
      ball.inv_accel(true, false);
      ball.inv_speed(true, false);
      std::cout << "P1\n";
      std::cout << "RIGHT COLLISION TIME: " << collision_info.z << std::endl;
    } else {
      collision_info = swept_collision(static_cast<Unit>(ball), p2, delta.count());
      if (collision_info.z < delta.count()/50) {
        remaining_time = 1.0f - collision_info.z;
        ball.update(collision_info.z * delta.count());
        ball.inv_accel(true, false);
        ball.inv_speed(true, false);
        std::cout << "P2\n";
        std::cout << "LEFT COLLISION TIME: " << collision_info.z << std::endl;
      } else {
        ball.update(delta.count());
      }
    }
    
    if(check_ball(ball) > 0) {
      //P2 SCORES
      ++p2score;
      ball.set_pos(1280/2 - 8, 720/2 - 8);
      ball.set_speed(0,0);
      ball.set_accel(0,0);
    } else if (check_ball(ball) < 0) {
      //P1 SCORES
      ++p1score;
      ball.set_pos(1280/2 - 8, 720/2 - 8);
      ball.set_speed(0,0);
      ball.set_accel(0,0);
    }

    score = std::to_string(p2score).append(std::string(" | ")).append(std::to_string(p1score));
    text.render_text(score);
    text_texture = text.get_texture();
    SDL_SetTextureBlendMode(text_texture, SDL_BLENDMODE_BLEND);

    while ((std::chrono::high_resolution_clock::now() - t2).count() < 0.016666f) {}

    delta = std::chrono::high_resolution_clock::now() - t2;
    t2 = std::chrono::high_resolution_clock::now();

    if (std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count() >= 1000) {
      std::cout << "DELTA: " << delta.count() << std::endl;
      std::cout << "FPS: " << fps << std::endl;
      std::cout << "========== BALL==========" << std::endl;
      std::cout << "POS (X,Y) " << "(" << ball.get_pos().x << "," << ball.get_pos().y << ")" << std::endl;
      std::cout << "SPEED (X,Y) " << "(" << ball.get_speed().x << "," << ball.get_speed().y << ")" << std::endl;
      std::cout << "ACCEL (X,Y) " << "(" << ball.get_accel().x << "," << ball.get_accel().y << ")\n" << std::endl;

      fps = 0;
      t1 = t2;
    }

    SDL_Rect text_rect = {1280/2 - text.get_dim().x/2, 720/2 - text.get_dim().y/2, text.get_dim().x, text.get_dim().y};

    //Render unit and background
    SDL_RenderClear(base_system.get_renderer());
    SDL_RenderCopy(base_system.get_renderer(), tp.get("background.png"), nullptr, nullptr);
    SDL_RenderCopy(base_system.get_renderer(), text_texture, nullptr, &text_rect);
    SDL_RenderCopy(base_system.get_renderer(), p1.get_texture(), nullptr, p1.get_rect());
    SDL_RenderCopy(base_system.get_renderer(), p2.get_texture(), nullptr, p2.get_rect());
    SDL_RenderCopy(base_system.get_renderer(), ball.get_texture(), nullptr, ball.get_rect());
    
    SDL_RenderPresent(base_system.get_renderer());

    ++fps;
  }

  std::cout << "hi";
  return 0;
}