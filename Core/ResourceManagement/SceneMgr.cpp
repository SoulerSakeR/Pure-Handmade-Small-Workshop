#include "SceneMgr.h"

SceneMgr* SceneMgr::instance = nullptr;

SceneMgr::SceneMgr()
{
	main_camera = nullptr;
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

void SceneMgr::set_main_camera(Camera& camera)
{
	if (main_camera != nullptr)
	{
		main_camera->set_is_main_camera(false);
		main_camera = nullptr;
	}
	main_camera = &camera;
	camera.set_is_main_camera(true);
	main_camera = &camera;
}
