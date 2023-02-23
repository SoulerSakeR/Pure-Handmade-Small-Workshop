#include "Scene.h"

#include <iostream>

using namespace std;
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

string& Scene::serialize()
{
	string result = "Scene:";
	result.append(name);
	result.append("\n");
	for(int i=0;i<rootGameObjs.size();i++)
	{
		result.append(rootGameObjs[i]->serialize());
	}
	result.append("SceneEnd");
	return result;
}
