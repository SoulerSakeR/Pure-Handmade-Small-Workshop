#pragma once
#include "Core/Core/Camera.h"
#include <Core/Core/Scene.h>
#include <Core/Render/renderwidget.h>
#include "Core/Render/RenderSetting.h"

class IScriptBehaviour;

class SceneMgr
{
	friend class GameEngine;
	friend class GameProject;
	friend class GameObject;
	friend class Camera;
	friend class Renderwidget;
public:
	static SceneMgr& get_instance();

	Camera* get_main_camera();
	void set_main_camera(Camera& camera);

	Scene* get_current_scene();
	bool hasCurrentScene();

	GameObject* findGameObjectByName(const std::string name);
	std::vector<GameObject*> findGameObjectsByTag(const std::string tag);

	void registerRenderer(Renderer* renderer);
	void unregisterRenderer(Renderer* renderer);

	RenderSetting* get_render_setting() const;

	void addScene(const std::string& relativePath);
	Scene* loadScene(int index);
	Scene* loadScene(const std::string& name);

	void exitScene();

	void tagToGameObjects_add(const std::string& tag, GameObject* gameObject);
	void tagToGameObjects_remove(const std::string& tag, GameObject* gameObject);

	void nameToGameObjects_add(const std::string& name, GameObject* gameObject);
	void nameToGameObjects_remove(const std::string& name, GameObject* gameObject);

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
	std::unordered_map<std::string, std::vector<GameObject*>> nameToGameObjects;
	std::unordered_map<std::string, std::vector<GameObject*>> tagToGameObjects;
	std::vector<IScriptBehaviour*> script_behaviours;
	RenderSetting* render_setting = nullptr;

	friend class IScriptBehaviour;
	friend class GameLoop;
};

