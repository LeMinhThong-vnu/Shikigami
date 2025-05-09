#ifndef FONT_LOADER_H
#define FONT_LOADER_H

#include "../global.h"
#include <SDL_ttf.h>

TTF_Font* loadFont(const char* filename, int font_size);

SDL_Texture *toTexture(SDL_Surface *surface, int destroySurface);

SDL_Texture* getTextTexture(TTF_Font* font, const char* text, SDL_Color color);

SDL_Texture* getTextTextureWrapped(TTF_Font* font, const char* text, SDL_Color color, int wrap_length);

void print_text_border(int x, int y, TTF_Font* font, const char* text, SDL_Color color_text, SDL_Color color_border, double ox, double oy);

#endif