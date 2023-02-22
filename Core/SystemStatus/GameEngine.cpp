#include "GameEngine.h"

GameProject* GameEngine::creatGameProject(string name, string path)
{
	GameProject* game = NULL;
	game = new GameProject(name, path);
	game->openScene(0);
}
