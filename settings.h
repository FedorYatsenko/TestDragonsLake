#pragma once

namespace settings {
	// Sprites
	const char* AVATAR_PATH = "data\\avatar.jpg";
	const char* RETICLE_PATH = "data\\reticle.png";
	const char* ENEMY_PATH = "data\\enemy.png";
	const char* BORDER_PATH = "data\\circle.tga";
	const char* BULLET_PATH = "data\\bullet.png";

	// Avatar
	const float AVATAR_SPEED = 0.3f; // pixels per millisecond

	// Enemies
	const int MIN_ENEMY_SPAWN_RANGE = 400; // pixels
	const float ENEMY_SPEED = 0.1f; // pixels per millisecond

	// Bullets
	const float BULLET_SPEED = 0.5f; // pixels per millisecond
	const unsigned int RELOADING_TIME = 1000; // millisecond
}