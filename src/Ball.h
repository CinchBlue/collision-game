#ifndef BALL_H
#define BALL_H

#include "base/TexturePool.h"
#include "base/Unit.h"

class Ball : public Unit {
public:
  Ball(int x, int y, SDL_Texture* texture) {
    set_rect({x, y, 16, 16});
    set_texture(texture);
    set_pos(x, y);
    set_speed(0, 0);
  }

  virtual void update(float delta) {
    speed.x += accel.x * delta;
    speed.y += accel.y * delta;

    if (get_speed().x > 700) {
      set_speed(700, get_speed().y);
    } else if (get_speed().x < -700) {
      set_speed(-700, get_speed().y);
    }

    if (get_speed().y > 700) {
      set_speed(get_speed().x, 700);
    } else if (get_speed().y < -700) {
      set_speed(get_speed().x, -700);
    }

    pos.x += speed.x * delta;
    pos.y += speed.y * delta;
    rect.x = pos.x;
    rect.y = pos.y;

    if (get_pos().y < 0) {
      set_pos(get_pos().x, 0);
      inv_accel(false, true);
      inv_speed(false, true);
    } else if (get_pos().y > 720-16) {
      set_pos(get_pos().x, 720-16);
      inv_accel(false, true);
      inv_speed(false, true);
    }

    if (get_pos().x < 0) {
      set_pos(0, get_pos().y);
    } else if (get_pos().x > 1280-16) {
      set_pos(1280-16, get_pos().y);
    }
  }
};

#endif //BALL_H