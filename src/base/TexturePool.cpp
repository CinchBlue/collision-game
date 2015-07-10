#include "TexturePool.h"

void TexturePool::load_texture(std::string name) {
  SDL_Surface* surface = nullptr;
  SDL_Texture* texture = nullptr;

  if ((surface = IMG_Load(name.c_str())) == nullptr) {
    surface =  IMG_Load((path + '/' + name).c_str());
  }

  std::cout << name << std::endl;

  if (surface == nullptr) {
    std::string str("Could not load texture file: ");
    str += name;
    throw std::runtime_error(str.c_str());
  } else {
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == nullptr) {
      std::string str("Could not convert surface to texture: ");
      str += name;
      throw std::runtime_error(str.c_str());
    } else {
      names.push_back(name);
      textures.push_back(texture);
    }
  }

}

void TexturePool::unload_texture(std::string name) {
  for(size_t i = 0; i < names.size(); ++i) {
    if ((names[i]) == name) {
      std::swap(names[i], names.back());
      names.pop_back();
      std::swap(textures[i], textures.back());
      textures.pop_back();    
    }
  }
}

SDL_Texture* TexturePool::get(std::string name) {
  for(size_t i = 0; i < names.size(); ++i) {
    if ((names[i]) == name) {
      return textures[i];
    }
  }
  return nullptr;
}