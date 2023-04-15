#pragma once
#include "Core/Utils/Vector2D.h"
#include "IRenderable.h"
class IResizable : public IRenderable
{
public:
	IResizable(GameObject* gameobj);
	bool isHovered(Vector2D screenPos);
	Vector2D size;
	QOpenGLBuffer* borderIbo;
};

