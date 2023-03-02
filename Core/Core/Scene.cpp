#include "Scene.h"

#include <iostream>

using namespace std;
Scene::Scene()
{
	this->name = "ExampleScene";
	allGameObjs = unordered_map<int,GameObject*>();
	rootGameObjs = vector<GameObject*>();
}

Scene::Scene(string name)
{
	this->name = name;
	allGameObjs = unordered_map<int,GameObject*>();
	rootGameObjs = vector<GameObject*>();
}

void Scene::addRootGameObject(GameObject* newObject)
{
	rootGameObjs.push_back(newObject);
	allGameObjs.insert(pair<int,GameObject*>(newObject->id,newObject));
}

void Scene::addGameObject(GameObject *newObject)
{
	allGameObjs.insert(pair<int,GameObject*>(newObject->id,newObject));
}

string &Scene::serialize()
{
	string result = "Scene:";
	result.append(name);
	result.append("\n");
	int size = rootGameObjs.size();
	result.append("rootGameObject:");
	result.append(to_string(size));
	result.append("\n");
	for(int i=0;i<size;i++)
	{
		result.append(rootGameObjs[i]->serialize());
	}
	result.append("SceneEnd");
	return result;
}

Scene *Scene::deserialize(std::stringstream ss)
{
	Scene* scene = new Scene();
	string line;
	getline(ss,line);
	if(line.find("Scene:")!=string::npos)
	{
		scene->name = line.substr(6,line.size()-1);
	}
    return nullptr;
}
