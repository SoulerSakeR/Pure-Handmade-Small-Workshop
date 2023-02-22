#include "Scene.h"

Scene::Scene()
{
	this->name = "ExampleScene";
	allGameObjs = vector<GameObject*>();
	rootGameObjs = vector<GameObject*>();
}

Scene::Scene(string name)
{
	this->name = name;
	allGameObjs = vector<GameObject*>();
	rootGameObjs = vector<GameObject*>();
}

string Scene::serialize()
{
	//TODO: 序列化
	return string();
}
