#pragma once

#include "Game.h"

class LTexture {
public:
	LTexture();
	~LTexture();
	bool loadFromFile(string path);
	void free();
	void setColor(Uint8 res, Uint8 green, Uint8 blue);
	void setBlendMode(SDL_BlendMode blending);
	void setAlpha(Uint8 alpha);
	void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
	int getWidth();
	int getHeight();
private:
	SDL_Texture* mTexture;
	int mWidth, mHeight;
};