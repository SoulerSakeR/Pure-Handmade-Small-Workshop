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
	Color32 get_color();
	void set_color(Color32 color);

	// Inherited via Component
	virtual void reset() override;

	virtual void serialize(PHString&) override;
	virtual void deserialize(std::stringstream& ss) override;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices; 
	
	Texture2D* texture2D;
	QOpenGLVertexArrayObject* vao;
	QOpenGLBuffer* vbo;
	QOpenGLBuffer* ibo;

protected:
	Color32 color;
};

