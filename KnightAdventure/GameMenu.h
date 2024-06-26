#pragma once

#include "Game.h"
#include "LTexture.h"

const int menuItemNum = 3;

class GameMenu {
public:
	GameMenu();
	~GameMenu() {};
	enum buttonType {
		PLAY_TEXT = 0,
		HELP_TEXT = 1,
		ABOUT_TEXT = 2
	};
	void loadMenu();
	void render(SDL_Event& e, Mix_Chunk buttonSound);
	bool checkMouseEvent(SDL_Event& e, SDL_Rect a);
	bool getPlayState();
	bool getHelpState();
	bool getAboutState();
	void setPlayState(bool state);
	void setHelpState(bool state);
	void setAboutState(bool state);
private:
	LTexture menuTexture;
	LTexture text[menuItemNum];
	LTexture mainText;
	bool playState;
	bool helpState;
	bool aboutState;
	int mWidth, mHeight;
	LTexture menuItem[menuItemNum];
	int posChange[menuItemNum];
	SDL_Rect menuItemPos;
	SDL_Rect spriteClips[2];
};