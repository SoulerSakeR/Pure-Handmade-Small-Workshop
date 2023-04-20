#pragma once
#include "Core/Core/Camera.h"
#include <Core/Core/Scene.h>
#include <Core/Render/renderwidget.h>

class SceneMgr
{
	friend class GameEngine;
	friend class GameProject;
	friend class Camera;
	friend class Renderwidget;
public:
	static SceneMgr& get_instance();

	Camera* get_main_camera();
	Scene* get_current_scene();
	void addScene(const std::string& relativePath);
	void set_main_camera(Camera& camera);
	Scene* loadScene(int index);
	Scene* loadScene(const std::string& name);
	std::vector<Camera*> cameras;

private:
	SceneMgr();
	SceneMgr(const SceneMgr&) = delete;

	void clear();
	SceneMgr& operator=(const SceneMgr&) = delete;

	static SceneMgr* instance;
	Camera* main_camera;
	Scene* current_scene;
	std::vector<std::string> scenes;
	std::unordered_map<std::string,std::vector<GameObject*>> nameToGameObjects;
	std::unordered_map<std::string, std::vector<GameObject*>> tagToGameObjects;
};

