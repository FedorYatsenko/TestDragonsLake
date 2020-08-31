#pragma once

#include <vector>
#include <list>
#include <functional>

#include "Framework.h"


/* Test Framework realization */
class MyFramework : public Framework {

public:
	MyFramework(int windowWidth, int windowHeight, bool fullscreen,
				int mapWidth, int mapHeight,
				int nEnemies, int nAmo);

	virtual void PreInit(int& width, int& height, bool& fullscreen) override;
	virtual bool Init() override;
	virtual void Close() override;

	virtual bool Tick() override;

	virtual void onMouseMove(int x, int y, int xrelative, int yrelative) override;
	virtual void onMouseButtonClick(FRMouseButton button, bool isReleased) override;
	virtual void onKeyPressed(FRKey k) override;
	virtual void onKeyReleased(FRKey k) override;

private:

	class Avatar {
	public:
		void Init(int x, int y, int w, int h);
		std::pair<int, int> move(int dTick, int mapWidth, int mapHeight);
		int getX() const;
		int getY() const;
		int getW() const;
		int getH() const;
		void setMoveL(bool value);
		void setMoveR(bool value);
		void setMoveU(bool value);
		void setMoveD(bool value);
	private:
		int x, y, w, h;
		bool moveL, moveR, moveU, moveD;
	};

	class Enemies {
	public:
		void Init(int w, int h, int count, int mapWidth, int mapHeight, const Avatar &avatar);
		// int getW() const {return w; };
		// int getH() const {return h; };

		int hasCollisionWith(int x, int y, int w, int h, int exclude = -1);
		void moveToAvatar(int dTick, const Avatar &avatar);
		void kill(int i);
		void draw(std::function<void(int, int)> fDraw);
	private:
		int w, h;
		std::vector<bool> alive;
		std::vector<int> xs;
		std::vector<int> ys;

		void spawmNewEnemy(int mapWidth, int mapHeight, const Avatar& avatar);
	};

	class Bullets {
	public:
		void Init(int w, int h, int nAmmo);
		void draw(std::function<void(int, int)> fDraw);
		void newBullet(unsigned int tick, int fromX, int fromY, int toX, int toY);
		void move(int dTick, int mapWidth, int mapHeight);
		void checkCollision(Enemies &enemies);
		void destroy(std::list<int>::iterator &x, std::list<int>::iterator &y,
			std::list<float>::iterator &directionX, std::list<float>::iterator &directionY);
	private:
		int w, h, nAmmo, leftAmmo;
		std::list<int> xs;
		std::list<int> ys;
		std::list<float> directionXs;
		std::list<float> directionYs;
		unsigned int reloadEndTime;
	};


	// GameSettings
	int windowWidth, windowHeight, mapWidth, mapHeight;
	int nEnemies, nAmo;
	bool fullscreen;

	// Game mode
	unsigned int lastTick;
	int windowX, windowY;
	bool shot;
	void newGame();
	
	// Sprites
	Sprite *avatarSprite, *reticleSprite, *enemySprite, *borderSprite, *bulletSprite;

	Avatar avatar;
	Enemies enemies;
	Bullets bullets;

	// Reticle
	int mouseDX, mouseDY, mouseX, mouseY;

	// border
	void drawBorder();
	
	void drawSpriteByMapXY(Sprite*, int mapX, int mapY);
	void toWindowXY(int& x, int& y);
	void toMapXY(int& x, int& y);
	
	static int getDistance(int x1, int y1, int x2, int y2);
	static void normalize(float &x, float &y);
	static void deleteByIndex(std::list<int>& list_, int index);
};