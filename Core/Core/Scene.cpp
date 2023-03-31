#include "Scene.h"
#include "Core/Utils/Test.h"
#include <iostream>

using namespace std;

const string rootGameObjectPrefix = "rootGameObject:";


Scene::Scene(string name)
{
	this->name = name;
	allGameObjsByID = unordered_map<int,GameObject*>();
	allGameObjsByName = unordered_map<std::string, std::vector<GameObject*>>();
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
	auto it = allGameObjsByName.find(newObject->name);
	if (it != allGameObjsByName.end())
		it->second.push_back(newObject);
	else
	{
		auto it2 =allGameObjsByName.emplace(pair<string, vector<GameObject*>>(newObject->name, vector<GameObject*>()));
		it2.first->second.push_back(newObject);
	}
}

void Scene::addGameObjectWithChildren(GameObject* newObject)
{
	allGameObjsByID.insert(pair<int, GameObject*>(newObject->getID(), newObject));
	auto it = allGameObjsByName.find(newObject->name);
	if (it != allGameObjsByName.end())
		it->second.push_back(newObject);
	else
	{
		auto it2 = allGameObjsByName.emplace(pair<string, vector<GameObject*>>(newObject->name, vector<GameObject*>()));
		it2.first->second.push_back(newObject);
	}
	for (auto t : newObject->transform->children)
		addGameObjectWithChildren(t->gameObject);
}



void Scene::initRootGameObject(GameObject* rootObject)
{

}

void Scene::removeGameObject(GameObject* gameObject)
{
	if (gameObject->isRootGameObject())
	{
		for (auto it = rootGameObjs.begin();it < rootGameObjs.end();it++)
			if (*it == gameObject)
			{
				removeGameObjectWithChildren(*it);
				rootGameObjs.erase(it);
			}
	}
	else
	{
		removeGameObjectWithChildren(gameObject);
	}			
}

void Scene::removeGameObjectWithChildren(GameObject* gameObject)
{
	auto it = allGameObjsByName.find(gameObject->name);
	if (it != allGameObjsByName.end())
	{
		auto& vec = it->second;
		for (auto i = vec.begin();i < vec.end();i++)
			if (*i == gameObject)
				vec.erase(i);
	}		
	allGameObjsByID.erase(gameObject->getID());
	for (auto t : gameObject->transform->children)
		removeGameObjectWithChildren(t->gameObject);
}

void Scene::removeGameObject(int id)
{
	std::unordered_map<int,GameObject*>::iterator it = allGameObjsByID.find(id);
	if (it != allGameObjsByID.end())
	{
		removeGameObject(it->second);
	}
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
					GameObject* root = new GameObject("",false);
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
