#include "Scene.h"
#include "Core/Core/Test.h"
#include <iostream>

using namespace std;

const string rootGameObjectPrefix = "rootGameObject:";

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
	allGameObjs.insert(pair<int,GameObject*>(newObject->getID(), newObject));
}

void Scene::addGameObject(GameObject *newObject)
{
	allGameObjs.insert(pair<int,GameObject*>(newObject->getID(), newObject));
}

void Scene::removeGameObject(GameObject* gameObject)
{
	removeGameObject(gameObject->getID());
}

void Scene::removeGameObject(int id)
{

}

void Scene::serialize(PHString& result)
{
	result.appendLine("Scene:", name);
	int size = rootGameObjs.size();
	result.appendLine("rootGameObject:", to_string(size));
	for(int i=0;i<size;i++)
	{
		rootGameObjs[i]->serialize(result);
	}
	result.appendLine("SceneEnd");
}

void Scene::deserialize(std::stringstream& ss)
{
	string line;
	do
	{
		getline(ss, line);
		if (line.find("Scene:") != string::npos)
		{
			name = line.substr(6, line.size() - 1);
			getline(ss, line);
			auto index = line.find(rootGameObjectPrefix);
			if (index != string::npos)
			{
				int size = stoi(line.substr(index +rootGameObjectPrefix.size(), line.size() - 1));
				for (int i = 0;i < size;i++)
				{
					GameObject* root = new GameObject();
					root->deserialize(ss);
					addRootGameObject(root);
				}
			}
		}
	} while (ss.good()&&line!="SceneEnd");
}
#ifdef TEST
Scene* Scene::loadFromText(const std::string& text)
{
	Scene* scene = new Scene();
	stringstream ss(text);
	scene->deserialize(ss);
	return scene;
}
#endif // TEST


Scene* Scene::loadFromPath(std::string path)
{
	Scene* scene = new Scene();
	//TODO: stringstream ss(file.readText(path));
	//scene->deserialize(ss);
	return scene;
}
