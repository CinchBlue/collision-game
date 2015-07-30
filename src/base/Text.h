#ifndef TEXT_H
#define TEXT_H

#include "SDL2/SDL.h"

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
    color(color) { texture = nullptr; }

   ~Text() {
    SDL_DestroyTexture(texture);
   }

  void render_text(std::string str) {
    if (texture != nullptr) {
      SDL_DestroyTexture(texture);
    }

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

#endif //TEXT_H