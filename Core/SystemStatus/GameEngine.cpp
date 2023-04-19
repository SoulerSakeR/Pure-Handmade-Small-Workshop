#include <filesystem>

#include "GameEngine.h"
#include "../Core/Scene.h"
#include "../Core/Debug.h"
#include "../FileIO/IO.h"
#include "Core/Render/renderwidget.h"
#include <Core/ResourceManagement/SceneMgr.h>
#include <Core/Utils/PHPath.h>
#include <Core/Core/Image.h>
#include <Core/UI/ComponentsDockWidget.h>

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

GameObject* GameEngine::getSelectedGameObject()
{
	if (ComponentsDockWidget::get_instance() != nullptr)
		return ComponentsDockWidget::get_instance()->get_selected_gameobject();
	return nullptr;
}

void GameEngine::onPropertyChange(Property* property)
{
	if (ComponentsDockWidget::get_instance() != nullptr)
		ComponentsDockWidget::get_instance()->onPropertyChanged(property);
}

void GameEngine::renderLoop()
{

}

Scene* const  GameEngine::getCurrentScene()
{
	return SceneMgr::get_instance().get_current_scene();
}

GameProject* GameEngine::getCurrentGameProject()
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
		new_path =  new_path.combinePath(name);
	}		
	GameProject* game = new GameProject(name, new_path.getNewPath());
	gameProject = game;
	game->openScene(gameProject->creatNewScene());
	return *game;
}
Vector2D GameEngine::get_resolution()
{
	auto rect = RenderWidget::getGameWidget().rect();
	return Vector2D(rect.width(), rect.height());
}
void GameEngine::refreshHierarchy()
{
	window->refreshHierachy();
}
bool GameEngine::needToRefeshUI(GameObject* gameobj)
{
	if (getSelectedGameObject() !=gameobj)
		return false;
	if(RenderWidget::getGameWidget().frameCount%5==0)
		return true;
	return false;
}
#ifdef TEST
bool GameEngine::openGameProjectTest(const std::string& project, const std::string** scenes)
{
	delete gameProject;
	stringstream ss(project);
	GameProject* gp = new GameProject("", "", false);
	gp->deserializeTest(ss,scenes);	
	gameProject = gp;
	gameProject->open(0);
	return true;
}
#endif // TEST

RenderWindow* GameEngine::getWindow()
{
	return window;
}

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
GameObject& GameEngine::addGameObject(const string& name, GameObject* const parent,Component::ComponentType type,InsertMode insertMode)
{
	Debug::log("addGameObject:" + name);
	GameObject* gameObject = new GameObject(name);
    if(parent == nullptr)
		SceneMgr::get_instance().get_current_scene()->insertGameObject(*gameObject);
	else
		SceneMgr::get_instance().get_current_scene()->insertGameObject(*gameObject,parent,insertMode);
	switch (type)
	{
	case Component::UNKNOWN:
		break;
	case Component::TRANSFORM:
		break;
	case Component::IMAGE:
		gameObject->addComponent<Image>()->set_imgPath("\\Resources\\0028.png");
		break;
	case Component::CAMERA:
		gameObject->addComponent<Camera>();
		break;
	case Component::SCRIPT:
		break;
	default:
		break;
	}
	if (inEditor)
	{
		hierarchy->addGameObject(gameObject, parent, insertMode);
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
	return nullptr;
}
