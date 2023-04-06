#include "Camera.h"
#include "Core/SystemStatus/GameEngine.h"
#include <Core/ResourceManagement/SceneMgr.h>

bool Camera::get_is_main_camera()
{
	return is_main_camera;
}

void Camera::set_is_main_camera(bool value)
{
	if (value == is_main_camera)
		return;
	else if (value)
	{
		auto cam = SceneMgr::get_instance().get_main_camera();
		if(cam==nullptr)
			SceneMgr::get_instance().set_main_camera(*this);
		else if(cam != this)
		{
			SceneMgr::get_instance().set_main_camera(*this);
		}
		else
		{
			is_main_camera = true;
		}
	}
	else
	{
		is_main_camera = false;
	}
}

QMatrix4x4 Camera::CalculateProjectionMulViewMatrix()
{
	QMatrix4x4 result;
	auto resolution = GameEngine::get_instance().get_resolution();
	float ratio = resolution.y / resolution.x;
	result.ortho(-view_width / 2, view_width / 2, -view_width / 2 * ratio, view_width / 2 * ratio,-10.f,10.f);
	result.lookAt(gameObject->transform->getWorldPosition().toQVector3D(1.f), QVector3D(0.f, 0.f, -1.f), QVector3D(0.f, 1.f, 0.f));
	return result;
}

Camera::Camera(GameObject* gameObj,float viewWidth):Component(gameObj)
{
	componentType = CAMERA;
	view_width = viewWidth;
}

void Camera::serialize(PHString& str)
{
	str.appendLine(std::to_string((int)componentType));
	str.appendLine(std::to_string(view_width));
}

void Camera::deserialize(std::stringstream& ss)
{
	std::string s;
	std::getline(ss, s);
	view_width = std::stof(s);
}

void Camera::reset()
{
	view_width = default_view_width;
}
