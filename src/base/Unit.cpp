#include "Unit.h"

void Unit::set_pos(const float& x, const float& y) {
  pos.x = x;
  pos.y = y;
  rect.x = pos.x;
  rect.y = pos.y;
}

void Unit::set_speed(const float& x, const float& y) {
  speed.x = x;
  speed.y = y;
}

void Unit::set_accel(const float& x, const float& y) {
  accel.x = x;
  accel.y = y;
}

void Unit::add_pos(const float& x, const float& y) {
  pos.x += x;
  pos.y += y;
  rect.x = pos.x;
  rect.y = pos.y;
}

void Unit::add_speed(const float& x, const float& y) {
  speed.x += x;
  speed.y += y;
}

void Unit::add_accel(const float& x, const float& y) {
  accel.x += x;
  accel.y += y;
}

void Unit::inv_pos(bool x, bool y) {
  if (x)
    pos.x = -pos.x;
  if (y)
    pos.y = -pos.y;
}

void Unit::inv_speed(bool x, bool y) {
  if (x)
    speed.x = -speed.x;
  if (y)
    speed.y = -speed.y;
}

void Unit::inv_accel(bool x, bool y) {
  if (x)
    accel.x = -accel.x;
  if (y)
    accel.y = -accel.y;
}