#include <filesystem>

#include "GameEngine.h"
#include "../Core/Scene.h"
#include "../Core/Debug.h"
#include "../FileIO/IO.h"
#include "Core/Render/renderwidget.h"
#include <Core/ResourceManagement/SceneMgr.h>
#include <Core/Utils/PHPath.h>
#include <Core/Core/Image.h>

using namespace std;

GameEngine* GameEngine::instance = nullptr;

GameEngine::GameEngine():pool(4)
{

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
bool GameEngine::initialize(RenderWindow* window)
{
	Debug::log("Engine initializing");
	srand((unsigned)time(NULL));
	gameProject = nullptr;
	this->window = window;
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
	PHPath new_path(path);
	if (new_path.getFileName() != name)
	{
		new_path.combinePath(name);
	}		
	GameProject* game = new GameProject(name, new_path.getNewPath());
	gameProject = game;
	gameProject->creatNewScene();
	game->openScene(0);
	game->save();
	return *game;
}
Vector2D GameEngine::get_resolution()
{
	auto rect = RenderWidget::getInstance().rect();
	return Vector2D(rect.width(), rect.height());
}
void GameEngine::refreshHierarchy()
{
	window->refreshHierachy();
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
GameObject& GameEngine::addGameObject(const string& name, GameObject* const parent,ComponentType type,InsertMode insertMode)
{
	Debug::log("addGameObject:" + name);
	GameObject* gameObject = new GameObject(name);
    if(parent == nullptr)
		GameEngine::get_instance().gameProject->currentScene->insertGameObject(*gameObject);		
	else
		GameEngine::get_instance().gameProject->currentScene->insertGameObject(*gameObject,parent,insertMode);
	switch (type)
	{
	case UNKNOWN:				
		break;
	case TRANSFORM:
		break;
	case IMAGE:
		gameObject->transform->translate(Vector2D((rand() / double(RAND_MAX) - 0.5) * 500, (rand() / double(RAND_MAX) - 0.5) * 500));
		gameObject->addComponent<Image>()->set_imgPath("\\Resources\\0028.png");
		gameObject->transform->localScale = Vector2D(1.f, 1.f);
		gameObject->transform->localRotation = (rand() / double(RAND_MAX) - 0.5) * 360;
		break;
	case CAMERA:
		break;
	case SCRIPT:
		break;
	default:
		break;
	}
	return *gameObject;
}

void GameEngine::deleteGameObject(GameObject* obj)
{
	getCurrentScene()->removeGameObject(obj);
}

const std::string& GameEngine::getRootPath()
{
	return rootPath;
}

std::string GameEngine::getGamePath()
{
	if (gameProject != nullptr)
		return gameProject->path.getNewPath();
	return "";
}
