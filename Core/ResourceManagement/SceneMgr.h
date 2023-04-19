#pragma once
#include "Core/Core/Camera.h"
#include <Core/Core/Scene.h>

class SceneMgr
{
	friend class GameEngine;
	friend class GameProject;
	friend class Camera;
public:
	static SceneMgr& get_instance();

	Camera* get_main_camera();
	Scene* get_current_scene();
	void addScene(const std::string& relativePath);
	void set_main_camera(Camera& camera);
	Scene* loadScene(int index);
	Scene* loadScene(const std::string& name);

private:
	SceneMgr();
	SceneMgr(const SceneMgr&) = delete;
	SceneMgr& operator=(const SceneMgr&) = delete;

	static SceneMgr* instance;
	Camera* main_camera;
	Scene* current_scene;
	std::vector<std::string> scenes;
	std::vector<Camera*> cameras;
};

