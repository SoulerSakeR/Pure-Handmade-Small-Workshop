#include "GameEngine.h"
#include "Core/Core/Test.h"
using namespace std;

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
#ifdef TEST
	string gameProject = "GameProject:Test\nE:/SourceCodes/Git/GroupProject/Pure-Handmade-Small-Workshop/debug/test\nScene:1\nE:/SourceCodes/Git/GroupProject/Pure-Handmade-Small-Workshop/debug/test/Scene/ExampleScene.scene\n"; // = readText(path);
	stringstream ss(gameProject);
#else // TEST
	string gameProject = file.readText(path);
	stringstream ss(gameProject);
#endif
	GameProject* gp = new GameProject("","",false);
	gp->deserialize(ss);
	return true;
}

/// @brief save game project to hard disk
/// @return value that indicates the process was completed or not
bool GameEngine::saveGameProject()
{
	if(gameProject!=nullptr)
	{		
		return gameProject->save();
	}
    return false;
}

/// @brief add new game object to scene or parent game object 
/// @param parent the pointer of parent game object or null for scene
/// @param name name of the new game object
/// @return the pointer of new game object
GameObject *GameEngine::addGameObject(string name, GameObject* parent)
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
