#include "Camera.h"
#include "Core/SystemStatus/GameEngine.h"
#include <Core/ResourceManagement/SceneMgr.h>

bool Camera::is_main_camera()
{
	return is_main_camera_;
}

void Camera::set_main_camera(bool value)
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
	onPropertyChanged(properties["Is Main Camera"]);
}

QMatrix4x4 Camera::CalculateProjectionMulViewMatrix()
{
	QMatrix4x4 result;
	auto resolution = GameEngine::get_instance().get_resolution();
	float ratio = resolution.y / resolution.x;
	result.ortho(-view_width / 2, view_width / 2, -view_width / 2 * ratio, view_width / 2 * ratio,-10.f,10.f);
	QMatrix4x4 matrix;
	matrix.rotate(gameObject->transform->getWorldRotation(), 0.f, 0.f, 1.f);	
	result.lookAt(gameObject->transform->getWorldPosition().toQVector3D(1.f), gameObject->transform->getWorldPosition().toQVector3D(-1.f), matrix* QVector4D(0.f, 1.f, 0.f,1.f).toVector3D());
	return result;
}

Vector2D Camera::screenToWorld(Vector2D screenPos) // camera rotaion todo
{
	Vector2D res = GameEngine::get_instance().get_resolution();
	auto ratio = view_width /res.x ;
	Vector2D leftBottom2point = screenPos * ratio;
	Vector2D cameraPos = gameObject->transform->getWorldPosition();
	Vector2D origin2leftBottom = cameraPos + Vector2D(-view_width / 2, -view_width / 2 * res.y / res.x);
	return origin2leftBottom + leftBottom2point;
}

Vector2D Camera::WorldToScreen(Vector2D worldPos) // bug
{
	auto matrix = SceneMgr::get_instance().get_main_camera()->CalculateProjectionMulViewMatrix();
	matrix.translate(worldPos.toQVector3D());
	return Vector2D((matrix * QVector4D(0.f,0.f,0.f,1.f)).toVector3DAffine().toVector2D());
}

void Camera::lookAt(Vector2D worldPos)
{
	Vector2D cameraPos = gameObject->transform->getWorldPosition();
	Vector2D dir = worldPos - cameraPos;
	gameObject->transform->translate(dir);
}


void Camera::set_view_width(float value)
{
	view_width = value;
	onPropertyChanged(properties["View Width"]);
}

float Camera::get_view_width()
{
	return view_width;
}

bool Camera::is_overlay()
{
	return is_overlay_;
}

void Camera::set_overlay(bool value)
{
	is_overlay_ = value;
	onPropertyChanged(properties["Is Overlay"]);
}

Camera::Camera(GameObject* gameObj,float viewWidth):IBoxResizable(gameObj)
{
	componentType = CAMERA;
	view_width = viewWidth;
	is_main_camera_ = false;
	is_overlay_ = false;
	properties["Size"]->is_editable = false;
	auto view_width_property = new Property("View Width", &(this->view_width), Property::FLOAT,this);
	view_width_property->set_property_func<float>(&Camera::get_view_width,&Camera::set_view_width,this);
	properties.emplace(view_width_property);
	auto is_main_camera_property = new Property("Is Main Camera", &(this->is_main_camera_), Property::BOOL,this);
	is_main_camera_property->set_property_func<bool>(&Camera::is_main_camera,&Camera::set_main_camera,this);
	properties.emplace(is_main_camera_property);
	auto is_overlay_property = new Property("Is Overlay", &(this->is_overlay_), Property::BOOL,this);
	is_overlay_property->set_property_func<bool>(&Camera::is_overlay,&Camera::set_overlay,this);
	properties.emplace(is_overlay_property);
	if (SceneMgr::get_instance().get_current_scene() != nullptr)
	{
		auto& cameras = SceneMgr::get_instance().cameras;
		if (auto it = std::find(cameras.begin(), cameras.end(), this); it == cameras.end())
			cameras.push_back(this);
	}
}

Camera::~Camera()
{
	if (SceneMgr::get_instance().get_current_scene() != nullptr)
	{
		auto& cameras = SceneMgr::get_instance().cameras;
		if(auto it = std::find(cameras.begin(),cameras.end(),this); it != cameras.end())
			cameras.erase(it);
	}
}

void Camera::serialize(PHString& str)
{
	str.appendLine(std::to_string((int)componentType));
	IBoxResizable::serialize(str);
	str.appendLine(std::to_string(view_width));
	str.appendLine(std::to_string(is_main_camera_));
	str.appendLine(std::to_string(is_overlay_));
}

void Camera::deserialize(std::stringstream& ss)
{
	IBoxResizable::deserialize(ss);
	std::string s;
	std::getline(ss, s);
	view_width = std::stof(s);
	std::getline(ss, s);
	is_main_camera_ = std::stoi(s);
	if (is_main_camera_)
		SceneMgr::get_instance().set_main_camera(*this);
	std::getline(ss, s);
	is_overlay_ = std::stoi(s);
	if (SceneMgr::get_instance().get_current_scene() != nullptr)
	{
		auto& cameras = SceneMgr::get_instance().cameras;
		if (auto it = std::find(cameras.begin(), cameras.end(), this); it == cameras.end())
			cameras.push_back(this);
	}
}

void Camera::reset()
{
	view_width = CAMERA_DEFAULT_VIEW_WIDTH;
}
