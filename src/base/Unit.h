#ifndef UNIT_H
#define UNIT_H

#include "SDL2/SDL.h"
#include <cstdlib>
#include <iostream>

struct vec2 {
  float x;
  float y;
};

class Unit {
public:
  Unit() :
    pos({0, 0}),
    speed({0, 0}),
    accel({0, 0}),
    texture(nullptr) {}
    
  //draw set/get
  void set_rect(const SDL_Rect& r) {rect = r;}
  void set_texture(SDL_Texture* t) {texture = t;}

  SDL_Rect* get_rect() {return &rect;}
  SDL_Texture* get_texture() {return texture;}

  //physics set/get
  void set_pos(const float& x, const float& y);
  void set_speed(const float& x, const float& y);
  void set_accel(const float& x, const float& y);

  void add_pos(const float& x, const float& y);
  void add_speed(const float& x, const float& y);
  void add_accel(const float& x, const float& y);

  void inv_pos(bool x, bool y);
  void inv_speed(bool x, bool y);
  void inv_accel(bool x, bool y);

  const vec2& get_pos() {return pos;}
  const vec2& get_speed() {return speed;}
  const vec2& get_accel() {return accel;}
  
private:
  vec2 pos;
  vec2 speed;
  vec2 accel;
  SDL_Rect rect;
  SDL_Texture* texture;
};

#endif //UNIT_H