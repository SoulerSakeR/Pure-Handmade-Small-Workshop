#pragma once
#include "Core/Utils/Vector2D.h"
#include "IRenderable.h"

class IBoxResizable : public IRenderable
{
public:
	IBoxResizable(GameObject* gameobj);
	virtual ~IBoxResizable();

	// inherited from IRenderable
	virtual void updateVertices() override;
	virtual void createIndices() override;

	// getters and setters
	virtual Vector2D get_size();
	virtual void set_size(Vector2D newSize);

	virtual void updateBorderVertices();
	virtual void createBorderIndices();
	virtual void updateOperatorVertices();
	bool isHovered(Vector2D screenPos);
	
	QOpenGLBuffer* borderVbo;
	QOpenGLBuffer* borderIbo;
	std::vector<Vertex> operatorVertices;
	std::vector<Vertex> borderVertices;
	std::vector<unsigned int> borderIndices;
protected:
	// inherited from Component
	virtual void serialize(PHString&) override;
	virtual void deserialize(std::stringstream& ss) override;

	Vector2D size;
};


