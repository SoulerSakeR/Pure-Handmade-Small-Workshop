#include <filesystem>

#include "GameEngine.h"
#include "../Core/Scene.h"
#include "../Core/Debug.h"
#include "../FileIO/IO.h"
#include "Core/Render/renderwidget.h"

using namespace std;

GameEngine* GameEngine::instance = nullptr;

GameEngine::GameEngine()
{
	initialize();
}

/// @brief get singleton instance, if it is not exists, create one 
/// @return the pointer of instance
GameEngine& GameEngine::get_instance()
{
	if(instance==nullptr)
		instance = new GameEngine();
    return *instance;
}

/// @brief initialize game engine and subsystem.
/// @return value that indicates the process was completed or not.
bool GameEngine::initialize()
{
	Debug::log("Engine initializing");
	gameProject = nullptr;
	std::filesystem::path current_path = std::filesystem::current_path();
	rootPath = current_path.string();
	Debug::log("Engine initialized");
    return true;
}

void GameEngine::renderLoop()
{

}

Scene* const  GameEngine::getCurrentScene()
{
	if (gameProject != nullptr && gameProject->currentScene != nullptr)
		return gameProject->currentScene;
	return nullptr;
}

GameProject* const GameEngine::getCurrentGameProject()
{
	return gameProject;
}

/// @brief creat game project with default scene.
/// @param name project name
/// @param path project absolute path
/// @return the pointer of the project created by name and path
GameProject& GameEngine::creatGameProject(const string& name,const string& path)
{
	// TODO: 创建前检查当前是否已保存项目，如否，弹出窗口询问是否保存当前项目
	// fix path if it contains unnecessary directory
	string new_path = path;
	auto index = new_path.find_last_of('\\');
	if (index == new_path.length() - 1)
		index = new_path.erase(index).find_last_of('\\');
	if (path.substr(index + 1) != name)
	{
		new_path.append("\\").append(name);
	}		
	GameProject* game = new GameProject(name, new_path);
	gameProject = game;
	game->openScene(0);
	game->save();
	return *game;
}
Vector2D GameEngine::get_resolution()
{
	auto rect = RenderWidget::getInstance().rect();
	return Vector2D(rect.width(), rect.height());
}
#ifdef TEST
bool GameEngine::openGameProjectTest(const std::string& project, const std::string** scenes)
{
	delete gameProject;
	stringstream ss(project);
	GameProject* gp = new GameProject("", "", false);
	gp->deserializeTest(ss,scenes);	
	gameProject = gp;
	gameProject->openScene(0);
	return true;
}
#endif // TEST

/// @brief open exist gameProject
/// @param path the obsolute path of .gameProject file
/// @return value that indicates whether the process was completedf
bool GameEngine::openGameProject(const string& path)
{
	const string& gameProject = IO::readText(path);
	stringstream ss(gameProject);
	GameProject* gp = new GameProject("","",false);
	gp->deserialize(ss);
	this->gameProject = gp;
	gp->openScene(0);
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
GameObject& GameEngine::addGameObject(const string& name, GameObject* const parent)
{
    if(parent == nullptr)
	{
		GameObject* gameObject = new GameObject(name);
		GameEngine::get_instance().gameProject->currentScene->insertGameObject(*gameObject);
		return *gameObject;
	}
	else
	{
		GameObject* gameObject = new GameObject(name);
		GameEngine::get_instance().gameProject->currentScene->insertGameObject(*gameObject,parent);
		return *gameObject;
	}
}

void GameEngine::deleteGameObject(GameObject* obj)
{
	getCurrentScene()->removeGameObject(obj);
}

const std::string& GameEngine::getRootPath()
{
	return rootPath;
}
