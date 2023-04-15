#include "IResizable.h"
#include "Core/ResourceManagement/SceneMgr.h"

IResizable::IResizable(GameObject* gameobj):IRenderable(gameobj)
{
	size = Vector2D::one();
	borderIbo = nullptr;
}

bool IResizable::isHovered(Vector2D screenPos)
{
	if (auto cam = SceneMgr::get_instance().get_main_camera();cam != nullptr)
	{
		//TODO: implement this
	}
	return false;
}
