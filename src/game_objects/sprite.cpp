#include "sprite.h"
#include <vector>
#include "../global.h"
#include <string>
#include <iostream>
#include <json.h>
#include <fstream>
#include "../util/draw.h"

Sprite::Sprite() {
	animations = new std::map<std::string, std::vector<Frame>>({});
};
Sprite::~Sprite() {
    free();
	delete animations;
};

void Sprite::update() {
	if ((*animations).size() > 0) {
		frame_sub++;
		if (frame_sub > (*animations)[anim_key][frame_cur].duration) {
			frame_sub = 0;
			frame_cur++;
		}
		if (frame_cur >= anim_length - 1) {
			frame_cur = 0;
			complete = true;
		}
	}
}

void Sprite::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
	//Modulate texture rgb
	SDL_SetTextureColorMod( texture, red, green, blue );
}
		
void Sprite::setAlpha( Uint8 alpha )
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod( texture, alpha );
}

bool Sprite::loadTexture(const char* path) {
    //Get rid of preexisting texture
	free();

	// Return success
	if (texture_assets.find(std::string(path)) != texture_assets.end()) {
		printf("Loading from already loaded asset... %s", path);
		texture = texture_assets.at(path);
	}
	else {
		texture = loadNewTexture(path);
	}
	
	if (texture != NULL) {
		// Get JSON
		std::ifstream file("assets/images/" + std::string(path) + ".json");
		Json::Value data;
		Json::Reader reader;
		reader.parse(file, data);

		// Reset
		frame_cur = 0;
		frame_sub = 0;

		// Get image dimensions
		w = data["meta"]["w"].asInt();
		h = data["meta"]["h"].asInt();
		cols = data["meta"]["cols"].asInt();
		rows = data["meta"]["rows"].asInt();

		// Get animations
		for (Json::Value::ArrayIndex i = 0; i != data["animations"].size(); i++) {
			Json::Value anim_data = data["animations"];
			std::string key = anim_data.getMemberNames()[i];
			std::vector<Frame> ind = {};
			for (Json::Value::ArrayIndex j = 0; j != anim_data[key].size(); j++) {
				Json::Value frame_data = anim_data[key][j];
				ind.push_back({ index: frame_data["index"].asInt(), duration: frame_data["duration"].asInt() });
			}
			animations->insert({key, ind});
		}
	}

	return texture != NULL;
}

void Sprite::free()
{
	//Free texture if it exists
	if( texture != NULL )
	{
		SDL_DestroyTexture( texture );
		texture = NULL;
		w = 0;
		h = 0;
	}
}

void Sprite::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip ) {
    //Set rendering space and render to screen
    
    int fx, fy;
	int fw, fh;
	fw = w * sx;
	fh = h * sy;
    fx = x - (fw * ox * sx);
    fy = y - (fh * oy * sy);

    SDL_Rect renderQuad = { fx, fy, fw, fh };

    //Set clip rendering dimensions
    // if( clip != NULL )
    // {
    //     renderQuad.w = clip->w;
    //     renderQuad.h = clip->h;
    // }

	Frame frame = (*animations)[anim_key][frame_cur];
	if (frame_cur >= anim_length - 1) {
		frame = (*animations)[anim_key][0];
	}
	int offset_x = frame.index / rows;
	int offset_y = (frame.index - offset_x * rows);
	SDL_Rect _clip = {
		w * offset_x,
		h * offset_y,
		w,
		h
	};

    //Render to screen
    SDL_RenderCopyEx( renderer, texture, &_clip, &renderQuad, angle, center, flip );
}