#ifndef UNIT_H
#define UNIT_H

#include "SDL2/SDL.h"
#include <cstdlib>
#include <limits>
#include <iostream>

class Unit;

struct vec2 {
  float x;
  float y;
};

struct vec3 {
  float x;
  float y;
  float z;
};

struct vec4 {
  float w;
  float x;
  float y;
  float z;
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

  inline const vec2& get_pos() {return pos;}
  inline const vec2& get_speed() {return speed;}
  inline const vec2& get_accel() {return accel;}

  virtual void update(float delta);
  
protected:
  vec2 pos;
  vec2 speed;
  vec2 accel;
  SDL_Rect rect;
  SDL_Texture* texture;
};

inline bool check_collision(SDL_Rect a, SDL_Rect b) {
 return ((a.x <= b.x + b.w) &&
         (a.y <= b.y + b.h) &&
         (a.x + a.w >= b.x) &&
         (a.y + a.h >= b.y));
}

vec3 swept_collision(Unit a, Unit b, float delta);

#endif //UNIT_H