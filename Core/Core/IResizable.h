#pragma once
#include "Core/Utils/Vector2D.h"
#include "IRenderable.h"

class IBoxResizable : public IRenderable
{
public:
	IBoxResizable(GameObject* gameobj);

	// inherited from Component
	virtual void set_property(Property* property, void* value) override;

	// inherited from IRenderable
	virtual void updateVertices() override;
	virtual void createIndices() override;

	// getters and setters
	virtual Vector2D get_size();
	virtual void set_size(Vector2D newSize);

	virtual void createBorderIndices();
	virtual void updateOperatorVertices();
	bool isHovered(Vector2D screenPos);
	
	QOpenGLBuffer* borderIbo;
	std::vector<Vertex> operatorVertices;
	std::vector<unsigned int> borderIndices;
protected:
	Vector2D size;
};


