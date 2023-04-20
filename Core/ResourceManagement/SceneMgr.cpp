#include "SceneMgr.h"
#include <Core/SystemStatus/GameEngine.h>

using std::string;
using std::vector;
using std::unordered_map;

SceneMgr* SceneMgr::instance = nullptr;

SceneMgr::SceneMgr()
{
	main_camera = nullptr;
	current_scene = nullptr;
	scenes = vector<string>();
	cameras = vector<Camera*>();
	nameToGameObjects = unordered_map<string, vector<GameObject*>>();
	tagToGameObjects = unordered_map<string, vector<GameObject*>>();
}

void SceneMgr::clear()
{
	main_camera = nullptr;
	tagToGameObjects.clear();
	nameToGameObjects.clear();
	cameras.clear();
}

SceneMgr& SceneMgr::get_instance()
{
	if (instance == nullptr)
		instance = new SceneMgr();
	return *instance;
}

Camera* SceneMgr::get_main_camera()
{
	return main_camera;
}

Scene* SceneMgr::get_current_scene()
{
	return current_scene;
}

void SceneMgr::addScene(const std::string& relativePath)
{
	scenes.push_back(relativePath);
}


void SceneMgr::set_main_camera(Camera& camera)
{
	if (main_camera != nullptr)
	{
		main_camera->set_main_camera(false);
		main_camera = nullptr;
	}
	main_camera = &camera;
	camera.set_main_camera(true);
	main_camera = &camera;
}

Scene* SceneMgr::loadScene(int index)
{
	clear();
	auto scene = new Scene();
	current_scene = scene;
	Scene::loadFromPath(PHPath(GameEngine::get_instance().getGamePath()).combinePath(scenes[index]).getNewPath(),scene);	
	return scene;
}

Scene* SceneMgr::loadScene(const std::string& path)
{
	clear();
	PHPath p(path);
	auto scene = Scene::loadFromPath(p.getNewPath());
	current_scene = scene;
	return scene;
}
