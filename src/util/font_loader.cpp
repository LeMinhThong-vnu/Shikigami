#include "font_loader.h"

TTF_Font* loadFont(const char* filename, int font_size) {
    return TTF_OpenFont(filename, font_size);
}

SDL_Texture *toTexture(SDL_Surface *surface, int destroySurface)
{
    SDL_Texture *texture;

    texture = SDL_CreateTextureFromSurface(renderer, surface);

    if (destroySurface)
    {
        SDL_FreeSurface(surface);
    }

    return texture;
}

SDL_Texture* getTextTexture(TTF_Font* font, const char* text, SDL_Color color) {
    SDL_Surface *surface;

    surface = TTF_RenderUTF8_Blended(font, text, color);

    return toTexture(surface, 1);
}

SDL_Texture* getTextTextureWrapped(TTF_Font* font, const char* text, SDL_Color color, int wrap_length) {
    SDL_Surface *surface;

    surface = TTF_RenderUTF8_Blended_Wrapped(font, text, color, wrap_length);

    return toTexture(surface, 1);
}