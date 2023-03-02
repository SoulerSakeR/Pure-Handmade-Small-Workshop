#include "GameEngine.h"

GameEngine* GameEngine::instance = nullptr;

GameEngine::GameEngine()
{
	initialize();
}

/// @brief get singleton instance, if it is not exists, create one 
/// @return the pointer of instance
GameEngine *GameEngine::getInstance()
{
	if(instance==nullptr)
		instance = new GameEngine();
    return instance;
}

/// @brief initialize game engine and subsystem.
/// @return value that indicates the process was completed or not.
bool GameEngine::initialize()
{
    return true;
}

/// @brief creat game project with default scene.
/// @param name project name
/// @param path project absolute path
/// @return the pointer of the project created by name and path
GameProject* GameEngine::creatGameProject(string name, string path)
{
	// TODO: 创建前检查当前是否已保存项目，如否，弹出窗口询问是否保存当前项目
	GameProject* game = nullptr;
	game = new GameProject(name, path);
	gameProject = game;
	game->openScene(0);
	return game;
}

/// @brief open exist gameProject
/// @param path the obsolute path of .gameProject file
/// @return value that indicates the process was completed or not
bool GameEngine::openGameProject(string path)
{
	
    return false;
}

/// @brief save game project to hard disk
/// @return value that indicates the process was completed or not
bool GameEngine::saveGameProject()
{
	if(gameProject!=nullptr&&gameProject->currentScene!=nullptr)
	{
		string scene = gameProject->currentScene->serialize();		
		string path = gameProject->path + "/" + gameProject->currentScene->name + sceneExtensionName;
		// TODO: 写入场景到文件
		// bool value = file::write(path,scene);
		PHString gp = PHString("");
		gp.appendLine("GameProject:",gameProject->name);
		return true;
	}
    return false;
}

/// @brief add new game object to scene or parent game object 
/// @param parent the pointer of parent game object or null for scene
/// @param name name of the new game object
/// @return the pointer of new game object
GameObject *GameEngine::addGameObject(GameObject *parent=nullptr,string name = "GameObject")
{
    if(parent == nullptr)
	{
		GameObject* gameObject = new GameObject(name);
		gameObject->transform->parent = nullptr;
		GameEngine::getInstance()->gameProject->currentScene->addRootGameObject(gameObject);
		return gameObject;
	}
	else
	{
		GameObject* gameObject = new GameObject(name);
		gameObject->transform->parent = parent->transform;
		parent->transform->children.push_back(gameObject->transform);
		GameEngine::getInstance()->gameProject->currentScene->addGameObject(gameObject);
		return gameObject;
	}
}
