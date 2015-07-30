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

void Unit::update(float delta) {
  if (get_speed().x > 0) {
    set_speed(0, get_speed().y);
  } else if (get_speed().x < -0) {
    set_speed(-0, get_speed().y);
  }

  if (get_speed().y > 450) {
    set_speed(get_speed().x, 450);
  } else if (get_speed().y < -450) {
    set_speed(get_speed().x, -450);
  }

  pos.x += speed.x * delta;
  pos.y += speed.y * delta;
  rect.x = pos.x;
  rect.y = pos.y;

  if (get_pos().y < 0) {
    set_pos(get_pos().x, 0);
  } else if (get_pos().y > 720-64) {
    set_pos(get_pos().x, 720-64);
  }
}

vec3 swept_collision(Unit a, Unit b, float delta) {
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