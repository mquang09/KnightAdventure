#include "MainObject.h"
#include "Map.h"

MainObject::MainObject() {
	mPlayerHitBoxX = 0;
	mPlayerHitBoxY = 0;
	mPlayerHitBoxH = 0;
	mPlayerHitBoxW = 0;
	mVelX = 0;
	mVelY = 0;
	mPosX = 0;
	mPosY = 0;
	frame = 0;
	mWidth = 0;
	mHeight = 0;
	mFixX = 0;
	mFixY = 0;
	playerStatus = -1;
	flip = SDL_FLIP_NONE;
	inputType.left_ = 0;
	inputType.right_ = 0;
	inputType.up_ = 0;
	inputType.down_ = 0;
	onGround = false;
	attackAnimationFinished = false;
}

bool MainObject::loadImage(string path) {
	bool success = mPlayerTexture.loadFromFile(path.c_str());
	if (success) {
		mWidth = mPlayerTexture.getWidth() / ANIMATION_FRAMES;
		mHeight = mPlayerTexture.getHeight();
	}
	return success;
}

void MainObject::setSpriteClips() {
	if (mWidth > 0 && mHeight > 0) {
		for (int i = 0; i < ANIMATION_FRAMES; i++) {
			mSpriteClipsRun[i].x = i * 64;
			mSpriteClipsRun[i].y = 0;
			mSpriteClipsRun[i].w = mWidth;
			mSpriteClipsRun[i].h = mHeight;

			mSpriteClipsAttack[i].x = i * 144;
			mSpriteClipsAttack[i].y = 0;
			mSpriteClipsAttack[i].w = 144;
			mSpriteClipsAttack[i].h = 128;
		}
	}
}

void MainObject::render() {
	if (mPosY > 640) {
		mPosX = 0;
		mPosY = 0;
	}
	int currentFrame = ANIMATION_FRAMES;
	SDL_Rect* currentClip = &mSpriteClipsRun[frame / 6];
	mFixX = 0;
	mFixY = 10;
	mPlayerTexture.loadFromFile("assets/characters/running.png");
	attackAnimationFinished = false;
	if (playerStatus == WALK_LEFT){
		mPlayerTexture.loadFromFile("assets/characters/running.png");
		flip = SDL_FLIP_HORIZONTAL;
		currentFrame = ANIMATION_FRAMES;
		currentClip = &mSpriteClipsRun[frame / 6];
		mPlayerHitBoxX = 0;
		mPlayerHitBoxY = 16;
		mPlayerHitBoxW = -32;
		mPlayerHitBoxH = -32;
	}
	else if (playerStatus == WALK_RIGHT){
		mPlayerTexture.loadFromFile("assets/characters/running.png");
		flip = SDL_FLIP_NONE;
		currentFrame = ANIMATION_FRAMES;
		currentClip = &mSpriteClipsRun[frame / 6];
		mFixX = 0;
		mFixY = 10;
		mPlayerHitBoxX = 32;
		mPlayerHitBoxY = 16;
		mPlayerHitBoxW = -32;
		mPlayerHitBoxH = -32;
	}
	if (playerStatus == PLAYER_ATTACK && onGround && mVelX == 0 && mVelY == 0 && inputType.up_ == 0) {
		mPlayerTexture.loadFromFile("assets/characters/attack.png");
		currentFrame = ATTACK_FRAMES;
		currentClip = &mSpriteClipsAttack[frame / 6];
		if (flip == SDL_FLIP_NONE) {
			mFixX = -64;
			mFixY = -16 + 10;

			mPlayerHitBoxX = 64;
			mPlayerHitBoxY = 32;
			mPlayerHitBoxW = -112;
			mPlayerHitBoxH = -64;

			mAttackHitBoxX = 48;
			mAttackHitBoxY = 0;
			mAttackHitBoxW = -55;
			mAttackHitBoxH = -48;
		}
		else {
			mFixX = -32;
			mFixY = -16 + 10;

			mPlayerHitBoxX = 48;
			mPlayerHitBoxY = 32;
			mPlayerHitBoxW = -112;
			mPlayerHitBoxH = -64;

			mAttackHitBoxX = 7;
			mAttackHitBoxY = 0;
			mAttackHitBoxW = -55;
			mAttackHitBoxH = -48;
		}
	}
	if (mVelX == 0 && mVelY == 0 && playerStatus != PLAYER_ATTACK) {
		mPlayerTexture.loadFromFile("assets/characters/idle.png");
		currentFrame = IDLE_FRAMES;
		currentClip = &mSpriteClipsRun[frame / 6];
		if (flip == SDL_FLIP_NONE) {
			mPlayerHitBoxX = 0;
			mPlayerHitBoxY = 16;
			mPlayerHitBoxW = -32;
			mPlayerHitBoxH = -32;
		}
		else {
			mPlayerHitBoxX = 32;
			mPlayerHitBoxY = 16;
			mPlayerHitBoxW = -32;
			mPlayerHitBoxH = -32;
		}
	}
	mPlayerTexture.render(mPosX - mapX + mFixX, mPosY + mFixY, currentClip, NULL, NULL, flip);
	SDL_Rect hitBox1 = { mPosX - mapX + mFixX + mAttackHitBoxX, mPosY + mFixY + mAttackHitBoxY, mSpriteClipsAttack[0].w + mAttackHitBoxW, mSpriteClipsAttack[0].h + mAttackHitBoxH};
	SDL_Rect hitBox2 = { mPosX - mapX + mFixX + mPlayerHitBoxX, mPosY + mFixY + mPlayerHitBoxY, currentClip->w + mPlayerHitBoxW, currentClip->h + mPlayerHitBoxH};
	playerHitBox = hitBox2;
	attackHitBox = hitBox1;
	SDL_SetRenderDrawColor(Game::gRenderer, 255, 0, 0, 255);
	//SDL_RenderDrawRect(Game::gRenderer, &playerHitBox);
	//SDL_RenderDrawRect(Game::gRenderer, &attackHitBox);
	frame++;
	if (frame / 6 >= currentFrame) frame = 0;
}

void MainObject::handleInput(SDL_Event& e) {
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
		switch (e.key.keysym.sym) {
		case SDLK_RIGHT:
		{
			inputType.right_ = 1;
			inputType.left_ = 0;
			playerStatus = WALK_RIGHT;
		}
		break;
		case SDLK_LEFT:
		{
			inputType.right_ = 0;
			inputType.left_ = 1;
			playerStatus = WALK_LEFT;
		}
		break;
		case SDLK_UP:
		{
			inputType.up_ = 1;
		}
		break;
		case SDLK_SPACE:
		{
			playerStatus = PLAYER_ATTACK;
		}
		}
	}
	else if (e.type == SDL_KEYUP && e.key.repeat == 0) {
		switch (e.key.keysym.sym) {
		case SDLK_RIGHT: inputType.right_ = 0; break;
		case SDLK_LEFT: inputType.left_ = 0; break;
		case SDLK_UP: inputType.up_ = 0; break;
		case SDLK_SPACE: playerStatus = PLAYER_IDLE; break;
		}
	}

}

void MainObject::move(Map& map_data) {
	mVelX = 0;
	mVelY += GRAVITY_SPEED;
	if (mVelY > MAX_GRAVITY_SPEED) {
		mVelY = MAX_GRAVITY_SPEED;
	}
	if (inputType.left_ == 1) {
		mVelX -= POS_VEL;
	}
	else if (inputType.right_ == 1) {
		mVelX += POS_VEL;
	}

	if (inputType.up_ == 1) {
		if (onGround && playerStatus != PLAYER_ATTACK){
			mVelY = -JUMP_VAL;
		}
		inputType.up_ = 0;
	}

	int x1 = 0;
	int x2 = 0;
	int y1 = 0;
	int y2 = 0;
	int height_min = 0;

	// Xu li va cham theo chieu ngang
	if (mHeight + mPlayerHitBoxW < TILE_SIZE) {
		height_min = mHeight + mPlayerHitBoxW;
	}
	else height_min = TILE_SIZE;
	x1 = (mPosX + mVelX) / TILE_SIZE;
	x2 = (mPosX + mVelX + mWidth - 1) / TILE_SIZE;

	//y1 va y2 la 2 o chua nhan vat

	y1 = (mPosY + 16) / TILE_SIZE + 1;
	y2 = (mPosY + 16 + height_min - 1) / TILE_SIZE + 1;
	onGround = false;

	if (x1 >= 0 && x2 < TOTAL_TILES_ROW && y1 >= 0 && y2 < TOTAL_TILES_COL) {
		//cout << y1 << " " << x2 << " va " << y2 << " " << x2 << endl;
		//Nhan vat di chuyen sang phai
		if (mVelX > 0) {
			if (map_data.map[y1][x2] != BLANK_TILE || map_data.map[y2][x2] != BLANK_TILE){
				mPosX = x2 * TILE_SIZE;
				mPosX -= mWidth + 1;
				mVelX = 0;
			}
		}
		//Nhan vat di chuyen sang trai
		else if (mVelX < 0) {
			if (map_data.map[y1][x1] != BLANK_TILE || map_data.map[y2][x1] != BLANK_TILE){
				mPosX = (x1 + 1) * TILE_SIZE;
				mVelX = 0;
			}
		}
	}

	//Xu li va cham theo chieu doc

	int width_min = 0;
	if (mWidth < TILE_SIZE) width_min = mWidth;
	else width_min = TILE_SIZE;
	x1 = (mPosX) / TILE_SIZE;
	x2 = (mPosX + mWidth) / TILE_SIZE;
	y1 = (mPosY + mVelY) / TILE_SIZE;
	y2 = (mPosY + mVelY + mHeight - 1) / TILE_SIZE;
	if (x1 >= 0 && x2 < TOTAL_TILES_ROW && y1 >= 0 && y2 < TOTAL_TILES_COL) {
		if (mVelY > 0) {
			if (map_data.map[y2][x1] != BLANK_TILE || map_data.map[y2][x2] != BLANK_TILE) {
				mPosY = y2 * TILE_SIZE;
				mPosY -= (mHeight - 1);
				mVelY = 0;
				onGround = true;
			}

		}
		else if (mVelY < 0) {
			if (map_data.map[y1][x1] != BLANK_TILE || map_data.map[y1][x2] != BLANK_TILE) {
				mPosY = (y1 + 1) * TILE_SIZE;
				mVelY = 0;
			}
		}
	}
	if (playerStatus == PLAYER_ATTACK && onGround) {
		mVelX = 0;
		mVelY = 0;
	}
	mPosX += mVelX;
	mPosY += mVelY;
	if (mPosX < 0) {
		mPosX = 0;
	}
	if ((mPosX + mWidth) > TOTAL_TILES_ROW * TILE_SIZE) {
		mPosX = TOTAL_TILES_ROW * TILE_SIZE - mWidth - 1;
	}
	centerEntityOnMap();
	
}

void MainObject::centerEntityOnMap() {
	if (mPosY < 640) {
		mapX = mPosX - (1280 / 2);
	}
	if (mapX < 0) {
		mapX = 0;
	}
	else if (mapX + 1280 >= TOTAL_TILES_ROW * TILE_SIZE) {
		mapX = TOTAL_TILES_ROW * TILE_SIZE - 1280;
	}
}

int MainObject::getMapX() {
	return mapX;
}

float MainObject::getPosX() {
	return mPosX;
}

float MainObject::getPosY() {
	return mPosY;
}

SDL_Rect MainObject::getPlayerHitbox() {
	return playerHitBox;
}

SDL_Rect MainObject::getPlayerAttackHitbox() {
	return attackHitBox;
}

int MainObject::getPlayerStatus() {
	return playerStatus;
}

int MainObject::getPlayerCurrentFrame() {
	return frame;
}


