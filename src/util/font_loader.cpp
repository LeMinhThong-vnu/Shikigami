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

void print_text_border(int x, int y, TTF_Font* font, const char* text, SDL_Color color_text, SDL_Color color_border, double ox, double oy) {
    SDL_Texture* font_texture = getTextTexture(font, text, color_border);
    SDL_Rect pos;
    SDL_QueryTexture(font_texture, NULL, NULL, &pos.w, &pos.h);
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            pos.x = x - (pos.w * ox) + i;
            pos.y = y - (pos.h * oy) + j;
            SDL_RenderCopy(renderer, font_texture, NULL, &pos);
        }
    }
    font_texture = getTextTexture(font, text, color_text);
    pos.x = x - (pos.w * ox);
    pos.y = y - (pos.h * oy);
    SDL_RenderCopy(renderer, font_texture, NULL, &pos);
}