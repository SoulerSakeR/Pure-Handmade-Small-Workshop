#include "GameEngine.h"


GameEngine *GameEngine::getInstance()
{
	if(instance==nullptr)
		instance = new GameEngine();
    return instance;
}

GameProject *GameEngine::creatGameProject(string name, string path)
{
	GameProject* game = nullptr;
	game = new GameProject(name, path);
	game->openScene(0);
}

bool GameEngine::saveGameProject()
{
	if(gameProject!=nullptr&&gameProject->currentScene!=nullptr)
	{
		string scene = gameProject->currentScene->serialize();
		// TODO: 写入场景到文件
		return true;
	}
    return false;
}
