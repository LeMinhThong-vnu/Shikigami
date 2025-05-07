#include "../util/draw.h"
#include "../global.h"
#include <json.h>
#include <fstream>
#include <string>

std::map<std::string, SDL_Texture*> texture_assets = {};

SDL_Texture* load_texture(const char* path) {
	SDL_Texture* texture = NULL;
	if (texture_assets.find(std::string(path)) != texture_assets.end()) {
		printf("Loading from already loaded asset... %s\n", path);
		texture = texture_assets.at(path);
	}
	else {
		texture = loadNewTexture(path);
	}
	return texture;
}

SDL_Texture* loadNewTexture(const char* path)
{
	SDL_Texture* newTexture = NULL;

	std::string _path = ("assets/images/" + std::string(path) + ".png");
	SDL_Surface* loadedSurface = IMG_Load( _path.c_str() );
	if( loadedSurface == NULL )
	{
		std::printf( "Unable to load image %s! SDL_image Error: %s\n", _path.c_str(), IMG_GetError() );
	}
	else
	{
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

        newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
		if( newTexture == NULL )
		{
			std::printf( "Unable to create texture from %s! SDL Error: %s\n", _path.c_str(), SDL_GetError() );
		}

		SDL_FreeSurface( loadedSurface );
	}
    std::printf("Loaded successfully: %s\n", _path.c_str());
    texture_assets.insert({ path, newTexture });
	return newTexture;
}

void drawTexture(SDL_Texture* texture, double x, double y)
{
    SDL_Rect dest;

    dest.x = x;
    dest.y = y;
    SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

    SDL_RenderCopy(renderer, texture, NULL, &dest);
}

SDL_Point getTextureSize(SDL_Texture* texture) {
    SDL_Point size;
    SDL_QueryTexture(texture, NULL, NULL, &size.x, &size.y);
    return size;
}