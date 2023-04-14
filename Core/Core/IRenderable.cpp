#include "IRenderable.h"

IRenderable::IRenderable(GameObject* gameobj) :Component(gameobj)
{
	vao = nullptr;
	vbo = nullptr;
	ibo = nullptr;
	vertices = nullptr;
	indices = nullptr;
	color = Color32{ 0.5f,0.5f,0.5f,0.5f };
	texture = nullptr;
}

IRenderable::~IRenderable()
{
	if (vao != nullptr)
	{
		vao->destroy();
		delete vao;
		vao = nullptr;
	}
	if (vbo != nullptr)
	{
		vbo->destroy();
		delete vbo;
		vbo = nullptr;
	}
	if (ibo != nullptr)
	{
		ibo->destroy();
		delete ibo;
		ibo = nullptr;
	}
	if (vertices != nullptr)
	{
		delete[] vertices;
		vertices = nullptr;
	}
	if (indices != nullptr)
	{
		delete[] indices;
		indices = nullptr;
	}
}

void IRenderable::reset()
{
	if (vao != nullptr)
	{
		vao->destroy();
		delete vao;
		vao = nullptr;
	}
	if (vbo != nullptr)
	{
		vbo->destroy();
		delete vbo;
		vbo = nullptr;
	}
	if (ibo != nullptr)
	{
		ibo->destroy();
		delete ibo;
		ibo = nullptr;
	}
	if (vertices != nullptr)
	{
		delete[] vertices;
		vertices = nullptr;
	}
}
