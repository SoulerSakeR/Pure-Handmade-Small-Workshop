#pragma once
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>
#include "Component.h"
#include "Core/ResourceManagement/Texture2D.h"
class IRenderable: public Component
{
public:
	IRenderable(GameObject* gameobj);
	~IRenderable();
	virtual void updateVertices() = 0;
	virtual void createIndices() = 0;
	virtual void reset() override;
	float* vertices; 
	unsigned int* indices; 
	Color32 color;
	QOpenGLTexture* texture;
	QOpenGLVertexArrayObject* vao;
	QOpenGLBuffer* vbo;
	QOpenGLBuffer* ibo;	
};

