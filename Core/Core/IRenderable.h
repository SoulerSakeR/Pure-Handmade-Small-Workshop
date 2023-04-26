#pragma once
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>
#include "Component.h"
#include "Core/ResourceManagement/Texture2D.h"

struct Vertex
{
	float position[3];
	float texCoord[2];
};

class IRenderable: public Component
{
public:
	IRenderable(GameObject* gameobj);
	virtual ~IRenderable();
	virtual void updateVertices() = 0;
	virtual void createIndices() = 0;
	virtual bool isTextureValid();
	QOpenGLTexture* get_texture();

	// Inherited via Component
	virtual void set_property(Property* property, void* value) override;
	virtual void reset() override;

	virtual void serialize(PHString&) override;
	virtual void deserialize(std::stringstream& ss) override;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices; 
	Color32 color;
	Texture2D* texture2D;
	QOpenGLVertexArrayObject* vao;
	QOpenGLBuffer* vbo;
	QOpenGLBuffer* ibo;
};

