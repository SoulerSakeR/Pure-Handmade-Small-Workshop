#pragma once
#include "Core/Core/Camera.h"
class SceneMgr
{
public:
	static SceneMgr& get_instance();

	Camera* get_main_camera();
	void set_main_camera(Camera& camera);

private:
	SceneMgr();
	SceneMgr(const SceneMgr&) = delete;
	SceneMgr& operator=(const SceneMgr&) = delete;

	static SceneMgr* instance;
	Camera* main_camera;
};

