#include "Camera.h"
#include "Core/SystemStatus/GameEngine.h"
#include <Core/ResourceManagement/SceneMgr.h>

bool Camera::is_main_camera()
{
	return is_main_camera_;
}

void Camera::set_main_camera(bool value,bool refreshUI)
{
	if (value == is_main_camera_)
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
			is_main_camera_ = true;
		}
	}
	else
	{
		is_main_camera_ = false;
	}
	if (refreshUI && GameEngine::get_instance().getSelectedGameObject() == gameObject)
	{
		GameEngine::get_instance().onPropertyChange(properties["is_main_camera"]);
	}
}

QMatrix4x4 Camera::CalculateProjectionMulViewMatrix()
{
	QMatrix4x4 result;
	auto resolution = GameEngine::get_instance().get_resolution();
	float ratio = resolution.y / resolution.x;
	result.ortho(-view_width / 2, view_width / 2, -view_width / 2 * ratio, view_width / 2 * ratio,-10.f,10.f);
	QMatrix4x4 matrix = QMatrix4x4();
	matrix.rotate(gameObject->transform->getWorldRotation(), 0.f, 0.f, 1.f);
	result.lookAt(gameObject->transform->getWorldPosition().toQVector3D(1.f), QVector3D(0.f, 0.f, -1.f),  matrix* QVector4D(0.f, 1.f, 0.f,1.f).toVector3D());
	return result;
}

Vector2D Camera::screenToWorld(Vector2D screenPos)
{
	Vector2D res = GameEngine::get_instance().get_resolution();
	auto ratio = view_width /res.x ;
	Vector2D leftBottom2point = screenPos * ratio;
	Vector2D cameraPos = gameObject->transform->getWorldPosition();
	Vector2D origin2leftBottom = cameraPos + Vector2D(-view_width / 2, -view_width / 2 * res.y / res.x);
	return origin2leftBottom + leftBottom2point;
}

Vector2D Camera::WorldToScreen(Vector2D worldPos)
{
	auto matrix = SceneMgr::get_instance().get_main_camera()->CalculateProjectionMulViewMatrix();
	matrix.translate(worldPos.toQVector3D());
	return Vector2D((matrix * QVector4D(0.f,0.f,0.f,1.f)).toVector3DAffine().toVector2D());
}

void Camera::set_property(Property* property, void* value)
{
	if (property->get_name() == "view_width")
	{
		set_view_width(*(float*)value, false);
	}
	else if (property->get_name() == "is_main_camera")
	{
		set_main_camera(*(bool*)value,false);
	}
}

void Camera::set_view_width(float value,bool refreshUI)
{
	view_width = value;
	if (refreshUI && GameEngine::get_instance().getSelectedGameObject() == gameObject)
	{
		GameEngine::get_instance().onPropertyChange(properties[0]);
	}
}

float Camera::get_view_width()
{
	return view_width;
}

Camera::Camera(GameObject* gameObj,float viewWidth):Component(gameObj)
{
	componentType = CAMERA;
	view_width = viewWidth;
	is_main_camera_ = false;
	properties.emplace("view_width",new Property("view_width", &(this->view_width), Property::FLOAT,this));
	properties.emplace("is_main_camera",new Property("is_main_camera", &(this->is_main_camera_), Property::BOOL,this));
}

void Camera::serialize(PHString& str)
{
	str.appendLine(std::to_string((int)componentType));
	str.appendLine(std::to_string(view_width));
	str.appendLine(std::to_string(is_main_camera_));
}

void Camera::deserialize(std::stringstream& ss)
{
	std::string s;
	std::getline(ss, s);
	view_width = std::stof(s);
	std::getline(ss, s);
	is_main_camera_ = std::stoi(s);
	if (is_main_camera_)
		SceneMgr::get_instance().set_main_camera(*this);
}

void Camera::reset()
{
	view_width = default_view_width;
}
