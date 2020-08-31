#include "MyFramework.h"

#include <ctime>

#include "settings.h"

const float SIN_COS_45 = 0.71f;


MyFramework::MyFramework(int windowWidth, int windowHeight, bool fullscreen, int mapWidth, int mapHeight, int nEnemies, int nAmo) {
	srand(time(NULL));

	// GameSettings
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;
	this->fullscreen = fullscreen;
	this->mapWidth = mapWidth;
	this->mapHeight = mapHeight;
	this->nEnemies = nEnemies;
	this->nAmo = nAmo;

	// Game mode
	this->lastTick = 0;
	this->shot = false;
}

void MyFramework::PreInit(int& width, int& height, bool& fullscreen) {
	width = this->windowWidth;
	height = this->windowHeight;
	fullscreen = this->fullscreen;
}

bool MyFramework::Init() {
	// load sprites
	this->avatarSprite = createSprite(settings::AVATAR_PATH);
	this->reticleSprite = createSprite(settings::RETICLE_PATH);
	this->enemySprite = createSprite(settings::ENEMY_PATH);
	this->borderSprite = createSprite(settings::BORDER_PATH);
	this->bulletSprite = createSprite(settings::BULLET_PATH);

	// set vars for reticle
	int w, h;
	getSpriteSize(this->reticleSprite, w, h);
	this->mouseDX = w / 2;
	this->mouseDY = h / 2;

	// Init
	newGame();

	return true;
}

void MyFramework::Close() {
	
}

bool MyFramework::Tick() {

	unsigned int tick = getTickCount();
	// std::cout << tick << std::endl;
	int dTick = int(tick - this->lastTick);
	this->lastTick = tick;

	std::pair<int, int> delta = this->avatar.move(dTick, this->mapWidth, this->mapHeight);
	this->windowX += delta.first;
	this->windowY += delta.second;

	this->enemies.moveToAvatar(dTick, this->avatar);

	if (this->shot) {
		this->shot = false;
		int fromX = this->avatar.getX() + this->avatar.getW() / 2,
			fromY = this->avatar.getY() + this->avatar.getH() / 2,
			toX = this->mouseX,
			toY = this->mouseY;
		toMapXY(toX, toY);
		bullets.newBullet(tick, fromX, fromY, toX, toY);
	}

	// this->bullets.checkCollision(this->enemies);
	this->bullets.move(dTick, mapWidth, mapHeight);
	this->bullets.checkCollision(this->enemies);

	if (this->enemies.hasCollisionWith(this->avatar.getX(), this->avatar.getY(), this->avatar.getW(), this->avatar.getH()) != -1) {
		//std::cout << "Restart!" << std::endl;
		newGame();
	}

	// ------------------ Drawing ------------------
	drawTestBackground();
	drawBorder();

	this->enemies.draw([this](int x, int y) {
		drawSpriteByMapXY(this->enemySprite, x, y);
	});

	drawSpriteByMapXY(this->avatarSprite, this->avatar.getX(), this->avatar.getY());

	this->bullets.draw([this](int x, int y) {
		drawSpriteByMapXY(this->bulletSprite, x, y);
	});

	drawSprite(this->reticleSprite, this->mouseX - this->mouseDX, this->mouseY - this->mouseDY);

	return false;
}

void MyFramework::onMouseMove(int x, int y, int xrelative, int yrelative) {
	// std::cout << "onMouseMove x=" << x << ", y=" << y << ", xrelative=" << xrelative << ", yrelative=" << yrelative << std::endl;
	this->mouseX = x;
	this->mouseY = y;
}

void MyFramework::onMouseButtonClick(FRMouseButton button, bool isReleased) {
	if (isReleased && button == FRMouseButton::LEFT) {
		this->shot = true;
	}

	/*switch (button) {
	case FRMouseButton::RIGHT:
		std::cout << "RIGHT " << isReleased << std::endl;
		break;
	case FRMouseButton::LEFT:
		std::cout << "LEFT " << isReleased << std::endl;
		break;
	case FRMouseButton::MIDDLE:
		std::cout << "MIDDLE " << isReleased << std::endl;
		break;
	case FRMouseButton::COUNT:
		std::cout << "COUNT" << isReleased << std::endl;
		break;
	default:
		std::cout << "Unknown " << isReleased << std::endl;
		break;
	}*/
}

void MyFramework::onKeyPressed(FRKey k) {
	switch (k) {
	case FRKey::RIGHT:
		this->avatar.setMoveR(true);
		// std::cout << "RIGHT" << std::endl;
		break;
	case FRKey::LEFT:
		this->avatar.setMoveL(true);
		// std::cout << "LEFT" << std::endl;
		break;
	case FRKey::DOWN:
		this->avatar.setMoveD(true);
		// std::cout << "DOWN" << std::endl;
		break;
	case FRKey::UP:
		this->avatar.setMoveU(true);
		// std::cout << "UP" << std::endl;
		break;
	case FRKey::COUNT:
		std::cout << "COUNT" << std::endl;
		break;
	default:
		std::cout << "Unknown" << std::endl;
		break;
	}
}

void MyFramework::onKeyReleased(FRKey k) {
	switch (k) {
	case FRKey::RIGHT:
		this->avatar.setMoveR(false);
		// std::cout << "Release RIGHT" << std::endl;
		break;
	case FRKey::LEFT:
		this->avatar.setMoveL(false);
		// std::cout << "Release LEFT" << std::endl;
		break;
	case FRKey::DOWN:
		this->avatar.setMoveD(false);
		// std::cout << "Release DOWN" << std::endl;
		break;
	case FRKey::UP:
		this->avatar.setMoveU(false);
		// std::cout << "Release UP" << std::endl;
		break;
	case FRKey::COUNT:
		std::cout << "Release COUNT" << std::endl;
		break;
	default:
		std::cout << "Release Unknown" << std::endl;
		break;
	}
}

void MyFramework::newGame() {
	int w, h;

	this->windowX = int(this->mapWidth - this->windowWidth) / 2;
	this->windowY = int(this->mapHeight - this->windowHeight) / 2;

	getSpriteSize(this->avatarSprite, w, h);
	this->avatar.Init(this->mapWidth / 2, this->mapHeight / 2, w, h);

	getSpriteSize(this->enemySprite, w, h);
	this->enemies.Init(w, h, this->nEnemies, this->mapWidth, this->mapHeight, this->avatar);

	getSpriteSize(this->bulletSprite, w, h);
	this->bullets.Init(w, h, this->nAmo);
}

void MyFramework::drawBorder() {
	int w, h;
	getSpriteSize(this->borderSprite, w, h);

	int x = -w, y1 = -h, y2 = this->mapHeight;

	while (x <= this->mapWidth) {
		drawSpriteByMapXY(this->borderSprite, x, y1);
		drawSpriteByMapXY(this->borderSprite, x, y2);
		x += w;
	}
	
	int x1 = -w, x2 = x - w, y = 0;
	while (y + h < this->mapHeight) {
		drawSpriteByMapXY(this->borderSprite, x1, y);
		drawSpriteByMapXY(this->borderSprite, x2, y);
		y += h;
	}
}

void MyFramework::drawSpriteByMapXY(Sprite* sprite, int x, int y) {
	toWindowXY(x, y);
	drawSprite(sprite, x, y);
}

void MyFramework::toWindowXY(int& x, int& y) {
	x -= this->windowX;
	y -= this->windowY;
}

void MyFramework::toMapXY(int& x, int& y) {
	x += this->windowX;
	y += this->windowY;
}

int MyFramework::getDistance(int x1, int y1, int x2, int y2) {
	return int(round(sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2))));
}

void MyFramework::normalize(float &x, float &y) {
	float length = sqrt(x * x + y * y);
	x = x / length;
	y = y / length;
}

void MyFramework::deleteByIndex(std::list<int>& list_, int index) {
	auto it = list_.begin();
	advance(it, index);
	list_.erase(it);
}

void MyFramework::Avatar::Init(int x, int y, int w, int h) {
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;

	this->moveL = false, this->moveR = false, this->moveU = false, this->moveD = false;
}

std::pair<int, int> MyFramework::Avatar::move(int dTick, int mapWidth, int mapHeight) {
	std::pair<int, int> delta = std::make_pair(0, 0);

	float f_dX = 0, f_dY = 0;

	bool moveX = this->moveL || this->moveR;
	bool moveY = this->moveU || this->moveD;

	if (!(moveX || moveY)) return delta; // no movements

	if (moveX) {
		f_dX = settings::AVATAR_SPEED * dTick;
		if (this->moveL) {
			f_dX *= -1;
		}
		if (moveY) {
			f_dX *= SIN_COS_45;
		}
	}
	if (moveY) {
		f_dY = settings::AVATAR_SPEED * dTick;
		if (this->moveU) {
			f_dY *= -1;
		}
		if (moveX) {
			f_dY *= SIN_COS_45;
		}
	}

	int dX = int(round(f_dX)),
		dY = int(round(f_dY));

	if (this->x + dX > 0 && this->x + dX + w < mapWidth) {
		this->x += dX;
		delta.first = dX;
	}
	if (this->y + dY > 0 && this->y + dY + h < mapHeight) {
		this->y += dY;
		delta.second = dY;
	}
	return delta;
}

int MyFramework::Avatar::getX() const {
	return this->x;
}

int MyFramework::Avatar::getY() const {
	return this->y;
}

int MyFramework::Avatar::getW() const {
	return this->w;
}

int MyFramework::Avatar::getH() const {
	return this->h;
}

void MyFramework::Avatar::setMoveL(bool value) {
	this->moveL = value;
}

void MyFramework::Avatar::setMoveR(bool value) {
	this->moveR = value;
}

void MyFramework::Avatar::setMoveU(bool value) {
	this->moveU = value;
}

void MyFramework::Avatar::setMoveD(bool value) {
	this->moveD = value;
}

void MyFramework::Enemies::Init(int w, int h, int count, int mapWidth, int mapHeight, const Avatar &avatar) {
	this->w = w;
	this->h = h;

	this->alive.clear();
	this->xs.clear();
	this->ys.clear();

	for (int i = 0; i < count; ++i) {
		spawmNewEnemy(mapWidth, mapHeight, avatar);
	}
}

int MyFramework::Enemies::hasCollisionWith(int x, int y, int w, int h, int exclude) {
	for (int i = 0; i < this->alive.size(); ++i) {
		if (i == exclude || !this->alive[i]) continue;

		if (x + w < this->xs[i])      continue;
		if (this->xs[i] + this->w < x) continue;
		if (y + h < this->ys[i])      continue;
		if (this->ys[i] + this->h < y) continue;

		return i;
	}

	return -1; // no collisions
}

void MyFramework::Enemies::moveToAvatar(int dTick, const Avatar &avatar) {
	int targetX = avatar.getX() + avatar.getW() / 2,
		targetY = avatar.getY() + avatar.getH() / 2;

	float directionX, directionY;
	int newX, newY;
	for (int i = 0; i < this->alive.size(); ++i) {
		if (!this->alive[i]) continue;

		directionX = float(targetX - this->xs[i]);
		directionY = float(targetY - this->ys[i]);
		normalize(directionX, directionY);

		newX = this->xs[i] + int(round(directionX * dTick * settings::ENEMY_SPEED));
		newY = this->ys[i] + int(round(directionY * dTick * settings::ENEMY_SPEED));

		if (hasCollisionWith(newX, newY, w, h, i) == -1) {
			this->xs[i] = newX;
			this->ys[i] = newY;
		}
	}
}

void MyFramework::Enemies::kill(int i) {
	this->alive[i] = false;
}

void MyFramework::Enemies::draw(std::function<void(int x, int y)> fDraw) {
	for (int i = 0; i < this->alive.size(); i++) {
		if (this->alive[i]) {
			fDraw(this->xs[i], this->ys[i]);
		}
	}
}

void MyFramework::Enemies::spawmNewEnemy(int mapWidth, int mapHeight, const Avatar &avatar) {
	int x, y;
	int maxX = mapWidth - this->w,
		maxY = mapHeight - this->h;
	while (true) {
		x = rand() % maxX;
		y = rand() % maxY;

		if (getDistance(x + w, y + h, avatar.getX() + avatar.getW(), avatar.getY() + avatar.getH()) < settings::MIN_ENEMY_SPAWN_RANGE) {
			continue;
		}
		if (hasCollisionWith(x, y, w, h) != -1) {
			continue;
		}

		break;
	}
	this->alive.push_back(true);
	this->xs.push_back(x);
	this->ys.push_back(y);
}

void MyFramework::Bullets::Init(int w, int h, int nAmmo) {
	this->w = w;
	this->h = h;
	this->nAmmo = nAmmo;
	this->leftAmmo = nAmmo;

	this->xs.clear();
	this->ys.clear();
	this->directionXs.clear();
	this->directionYs.clear();

	this->reloadEndTime = 0;
}

void MyFramework::Bullets::newBullet(unsigned int tick, int fromX, int fromY, int toX, int toY) {
	if (this->leftAmmo == 0 && this->reloadEndTime <= tick) {
		this->leftAmmo = this->nAmmo;
	}

	if (this->leftAmmo > 0) {
		--this->leftAmmo;

		if (this->leftAmmo == 0) {
			this->reloadEndTime = tick + settings::RELOADING_TIME;
		}

		float directionX = float(toX - fromX),
			directionY = float(toY - fromY);
		normalize(directionX, directionY);

		this->xs.push_back(fromX);
		this->ys.push_back(fromY);
		this->directionXs.push_back(directionX);
		this->directionYs.push_back(directionY);
	}
}

void MyFramework::Bullets::move(int dTick, int mapWidth, int mapHeight) {
	float delta = dTick * settings::BULLET_SPEED;

	auto x = this->xs.begin(),
		 y = this->ys.begin();

	auto directionX = this->directionXs.begin(),
		 directionY = this->directionYs.begin();

	while (x != xs.end()) { // && y != ys.end()
		*x += int(round(*directionX * delta));
		*y += int(round(*directionY * delta));

		if (*x < 0 || *x > mapWidth || *y < 0 || *y > mapHeight) {
			destroy(x, y, directionX, directionY);
		} else {
			++x; ++y;
			++directionX;
			++directionY;
		}
	}
}

void MyFramework::Bullets::checkCollision(Enemies &enemies) {
	auto x = this->xs.begin(),
		y = this->ys.begin();

	auto directionX = this->directionXs.begin(),
		directionY = this->directionYs.begin();

	while (x != xs.end()) {
		int enemy = enemies.hasCollisionWith(*x, *y, this->w, this->h);

		if (enemy != -1) {
			// std::cout << "Destroy! " << *x << ' ' << *y << ' ' << w << ' ' << h << std::endl;
			destroy(x, y, directionX, directionY);
			enemies.kill(enemy);
		} else {
			++x; ++y;
			++directionX;
			++directionY;
		}
	}
}

void MyFramework::Bullets::draw(std::function<void(int, int)> fDraw) {
	auto x = this->xs.begin(),
		 y = this->ys.begin();

	while (x != xs.end()) { // && y != ys.end()
		fDraw(*x, *y);
		++x;
		++y;
	}
}

void MyFramework::Bullets::destroy(std::list<int>::iterator &x, std::list<int>::iterator &y,
			std::list<float>::iterator &directionX, std::list<float>::iterator &directionY) {
	x = this->xs.erase(x);
	y = this->ys.erase(y);
	directionX = this->directionXs.erase(directionX);
	directionY = this->directionYs.erase(directionY);
}
