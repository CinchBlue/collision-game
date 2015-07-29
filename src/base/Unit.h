#ifndef UNIT_H
#define UNIT_H

#include "SDL2/SDL.h"
#include <cstdlib>
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

vec3 swept_collision(Unit a, Unit b, float delta = 1.0f) {
  vec3 collision_info;
  //normalx, normaly, time as Z

  //Find the distance between objects on near and far sides
  float x_inv_entry, x_inv_exit, y_inv_entry, y_inv_exit;

  if (a.get_speed().x * delta > 0.0f) {
    x_inv_entry = b.get_pos().x - (a.get_pos().x + a.get_rect()->w);
    x_inv_exit = (b.get_pos().x + b.get_rect()->w) - a.get_pos().x; 
  } else {
    x_inv_entry = (b.get_pos().x + b.get_rect()->w) - a.get_pos().x; 
    x_inv_exit = b.get_pos().x - (a.get_pos().x + a.get_rect()->w);
  }

  if (a.get_speed().y * delta > 0.0f) {
    y_inv_entry = b.get_pos().y - (a.get_pos().y + a.get_rect()->h);
    y_inv_exit = (b.get_pos().y + b.get_rect()->h) - a.get_pos().y; 
  } else {
    y_inv_entry = (b.get_pos().y + b.get_rect()->h) - a.get_pos().y; 
    y_inv_exit = b.get_pos().y - (a.get_pos().y + a.get_rect()->h);
  }

  //Time of collision and time of leaving for each axis
  //The infinity statement is so we don't divide by 0.
  float x_entry, x_exit, y_entry, y_exit;

  if (a.get_speed().x * delta == 0.0f) {
    x_entry = -std::numeric_limits<float>::infinity();
    x_exit = std::numeric_limits<float>::infinity();
  } else {
    x_entry = x_inv_entry / a.get_speed().x * delta;
    x_exit = x_inv_exit / a.get_speed().x * delta;
  }

  if (a.get_speed().y * delta == 0.0f) {
    y_entry = -std::numeric_limits<float>::infinity();
    y_exit = std::numeric_limits<float>::infinity();
  } else {
    y_entry = y_inv_entry / a.get_speed().y * delta;
    y_exit = y_inv_exit / a.get_speed().y * delta;
  }

  //Find earliest/latest times of collision
  float entry_time = std::max(x_entry, y_entry);
  float exit_time = std::min(x_exit, y_exit);

  //No collision
  if ((entry_time > exit_time) || 
      (x_entry < 0.0f && y_entry < 0.0f) ||
      (x_entry > delta && y_entry > delta)) {
    collision_info.x = 0.0f;
    collision_info.y = 0.0f;
    collision_info.z = delta;
  } else {
    //We have a collision
    if (x_entry > y_entry) {
      if (x_inv_entry < 0.0f) {
        collision_info.x = delta;
        collision_info.y = 0.0f;
      } else {
        collision_info.x = -delta;
        collision_info.y = 0.0f;
      }
    } else {
      if (y_inv_entry < 0.0f) {
        collision_info.x = 0.0f;
        collision_info.y = delta;
      } else {
        collision_info.x = 0.0f;
        collision_info.y = -delta;
      }
    }
    collision_info.z = entry_time;
  }

  return collision_info;
}

#endif //UNIT_H