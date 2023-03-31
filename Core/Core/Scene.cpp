#include "Scene.h"
#include "Core/Utils/Test.h"
#include <iostream>

using namespace std;

const string rootGameObjectPrefix = "rootGameObject:";


Scene::Scene(string name)
{
	this->name = name;
	allGameObjsByID = unordered_map<int,GameObject*>();
	allGameObjsByName = unordered_multimap<string, GameObject*>();
	rootGameObjs = vector<GameObject*>();
}

void Scene::insertGameObject(GameObject& value,GameObject* target,InsertMode insertMode)
{
	addGameObject(&value);
	if (target == nullptr)
	{
		rootGameObjs.push_back(&value);
		return;
	}
	switch (insertMode)
	{
	case BEFORE:
		if (target->isRootGameObject())
		{
			auto it = find(rootGameObjs.begin(), rootGameObjs.end(), target);
			rootGameObjs.insert(it, &value);
		}
		else
		{
			auto& vec = target->transform->parent->children;
			auto it = find(vec.begin(), vec.end(), target->transform);
			vec.insert(it, value.transform);
			value.transform->parent = target->transform->parent;
		}
		break;
	case AFTER:
		if (target->isRootGameObject())
		{
			auto it = find(rootGameObjs.begin(), rootGameObjs.end(), target);
			rootGameObjs.insert(++it, &value);
		}
		else
		{
			auto& vec = target->transform->parent->children;
			auto it = find(vec.begin(), vec.end(), target->transform);
			vec.insert(++it, value.transform);
			value.transform->parent = target->transform->parent;
		}
		break;
	case INSIDE:
		target->transform->children.push_back(value.transform);
		value.transform->parent = target->transform;
	default:
		break;
	}
}


void Scene::addGameObject(GameObject *newObject)
{
	allGameObjsByID.insert(pair<int,GameObject*>(newObject->getID(), newObject));
	allGameObjsByName.insert(pair<string, GameObject*>(newObject->name, newObject));
}

void Scene::initRootGameObject(GameObject* rootObject)
{

}

void Scene::removeGameObject(GameObject* gameObject)
{
	if (gameObject->isRootGameObject())
		for (auto obj : rootGameObjs)
			if (obj == gameObject)
				return;
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
					insertGameObject(*root); //TODO: need to fix to add child objs
				}
			}
		}
	} while (ss.good()&&line!="SceneEnd");
}
const std::unordered_map<int, GameObject*> Scene::getAllGameObjs()
{
	return allGameObjsByID;
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
