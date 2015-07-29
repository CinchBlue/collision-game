#ifndef PADDLE_H
#define PADDLE_H

#include "base/TexturePool.h"
#include "base/Unit.h"

class Paddle : public Unit {
public:
  Paddle(int x, int y, SDL_Texture* texture) {
    set_rect({x, y, 16, 64});
    set_texture(texture);
    set_pos(x, y);
    set_speed(0, 0);
  }
};

#endif //PADDLE_H