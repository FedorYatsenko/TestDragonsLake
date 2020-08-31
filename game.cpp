#include "MyFramework.h"


int main(int argc, char *argv[]) {
	int windowX = 800, windowY = 600,
		mapX = 1000, mapY = 1000,
		nEnemies = 10, nAmmo = 3;

	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-window") == 0 && i + 2 < argc) {
			windowX = strtoul(argv[++i], NULL, 10);
			windowY = strtoul(argv[++i], NULL, 10);
			// std::cout << "-window " << windowX << 'x' << windowY << std::endl;
		} else if (strcmp(argv[i], "-map") == 0 && i + 2 < argc) {
			mapX = strtoul(argv[++i], NULL, 10);
			mapY = strtoul(argv[++i], NULL, 10);
			// std::cout << "-map " << mapX << 'x' << mapY << std::endl;
		} else if (strcmp(argv[i], "-num_enemies") == 0 && i + 1 < argc) {
			nEnemies = strtoul(argv[++i], NULL, 10);
			// std::cout << "-num_enemies " << nEnemies << std::endl;
		} else if (strcmp(argv[i], "-num_ammo") == 0 && i + 1 < argc) {
			nAmmo = strtoul(argv[++i], NULL, 10);
			// std::cout << "-num_ammo " << nAmo << std::endl;
		} else {
			std::cerr << "Unknown arg " << argv[i] << std::endl;
		}
	}

	std::cout << "windowX=" << windowX << ", windowY=" << windowY << ", mapX=" << mapX << ", mapY=" << mapY << ", nEnemies=" << nEnemies << ", nAmmo=" << nAmmo << std::endl;
	return run(new MyFramework(windowX, windowY, false, mapX, mapY, nEnemies, nAmmo));
}


