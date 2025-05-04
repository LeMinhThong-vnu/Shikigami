#ifndef DRAW_H
#define DRAW_H

#include <SDL.h>
#include <SDL_image.h>
#include <map>
#include <string>

extern std::map<std::string, SDL_Texture*> texture_assets;

SDL_Texture* load_texture(const char* filename);

SDL_Texture* loadNewTexture(const char* filename);

void drawTexture(SDL_Texture* texture, double x, double y);

SDL_Point getTextureSize(SDL_Texture* texture);

#endif