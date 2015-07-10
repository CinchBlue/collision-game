#ifndef TEXTUREPOOL_H
#define TEXTUREPOOL_H

#include <fstream>
#include <string>
#include <vector>

#include "system.h"

class TexturePool {
public:
  //path is the directory we want to be loading from.
  TexturePool(SDL_Renderer* renderer) :
    renderer(renderer),
    path("") {}

  TexturePool(SDL_Renderer* renderer, std::string path) :
    renderer(renderer),
    path(path) {}

  void load_texture(std::string name); 
  void unload_texture(std::string name);
  SDL_Texture* get(std::string name);

  void set_path(std::string path) {this->path = path;}
private:
  SDL_Renderer* renderer;
  std::vector<std::string> names;
  std::vector<SDL_Texture*> textures;
  std::string path;
};

#endif //TEXTUREPOOL_H