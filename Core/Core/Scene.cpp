#include "Scene.h"
#include "Core/Utils/Test.h"
#include <iostream>
#include <Core/FileIO/IO.h>
#include "Image.h"
#include "Core/Core/Renderer.h"
#include <Core/ResourceManagement/SceneMgr.h>
#include <Core/ResourceManagement/ResourceMgr.h>

using namespace std;

const string rootGameObjectPrefix = "rootGameObject:";


Scene::Scene(string name)
{
	this->name = name;
	allGameObjsByID = unordered_map<int,GameObject*>();
	allGameObjsByName = unordered_map<std::string, std::vector<GameObject*>>();
	rootGameObjs = vector<GameObject*>();
	is_changed = false;
}

Scene::~Scene()
{
	for (auto& gameobj : rootGameObjs)
	{
		delete gameobj;
	}
}

Result<void*> Scene::renameGameObject(GameObject* gameObject, std::string newName)
{
	if (gameObject == nullptr)
		return Result<void*>(false,"gameObject is nullptr");
	is_changed = true;
	// Removing references to old names
	SceneMgr::get_instance().nameToGameObjects_remove(gameObject->get_name(), gameObject);
	if (auto it = allGameObjsByName.find(gameObject->name); it != allGameObjsByName.end())
	{
		auto& vec = it->second;
		auto it2 = find(vec.begin(), vec.end(), gameObject);
		if (it2 != vec.end())
		{
			vec.erase(it2);
		}
	}

	// Adding a reference to a new name
	if (auto it = allGameObjsByName.find(newName); it != allGameObjsByName.end())
	{
		it->second.push_back(gameObject);
		gameObject->name = newName;
		SceneMgr::get_instance().nameToGameObjects_add(newName, gameObject);
		return Result<void*>();
	}
		
	
	allGameObjsByName[newName] = vector<GameObject*>();
	allGameObjsByName[newName].push_back(gameObject);
	gameObject->name = newName;
	SceneMgr::get_instance().nameToGameObjects_add(newName, gameObject);
	return Result<void*>();
}

void Scene::insertGameObject(GameObject& value,GameObject* target,InsertMode insertMode)
{
	is_changed = true;
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
	SceneMgr::get_instance().get_render_setting()->refreshLater();
}

void Scene::insertExistGameObject(GameObject* gameObject, GameObject* target, InsertMode insertMode)
{
	is_changed = true;
	//remove from old parent
	if (gameObject->isRootGameObject())
	{
		auto it = find(rootGameObjs.begin(), rootGameObjs.end(), gameObject);
		rootGameObjs.erase(it);
	}
	else
	{
		auto& vec = gameObject->transform->parent->children;
		auto it = find(vec.begin(), vec.end(), gameObject->transform);
		vec.erase(it);
	}

	//add to new parent
	if (target->isRootGameObject())
	{
		gameObject->transform->parent = nullptr;
		switch (insertMode)
		{
		case BEFORE:
		{
			auto it = find(rootGameObjs.begin(), rootGameObjs.end(), target);
			rootGameObjs.insert(it, gameObject);
			break;
		}			
		case AFTER:
		{
			auto it = find(rootGameObjs.begin(), rootGameObjs.end(), target);
			rootGameObjs.insert(++it, gameObject);
			break;
		}
		case INSIDE:
		{
			target->transform->children.push_back(gameObject->transform);
			gameObject->transform->parent = target->transform;
			break;
		}			
		default:
			break;
		}
	}
	else
	{
		gameObject->transform->parent = target->transform->parent;
		switch (insertMode)
		{
		case BEFORE:
		{
			auto& vec = target->transform->parent->children;
			auto it = find(vec.begin(), vec.end(), target->transform);
			vec.insert(it, gameObject->transform);
			break;
		}
		case AFTER:
		{
			auto& vec = target->transform->parent->children;
			auto it = find(vec.begin(), vec.end(), target->transform);
			vec.insert(++it, gameObject->transform);
			break;
		}
		case INSIDE:
		{
			target->transform->children.push_back(gameObject->transform);
			gameObject->transform->parent = target->transform;
			break;
		}
		default:
			Debug::logError("insertMode is not valid");
			break;
		}
	}
	SceneMgr::get_instance().get_render_setting()->refreshLater();
}


void Scene::addGameObject(GameObject *newObject)
{
	is_changed = true;
	allGameObjsByID.insert(pair<int,GameObject*>(newObject->get_id(), newObject));
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
	is_changed = true;
	allGameObjsByID.insert(pair<int, GameObject*>(newObject->get_id(), newObject));
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
	is_changed = true;
	rootGameObjs.push_back(rootObject);
	addGameObjectWithChildren(rootObject);
}

void Scene::onGameObjectPropertyChangedHandle(GameObject* gameobj)
{
	is_changed = true;
}

void Scene::removeGameObject(GameObject* gameObject)
{
	is_changed = true;
	if (gameObject->isRootGameObject())
	{
		for (auto it = rootGameObjs.begin();it < rootGameObjs.end();it++)
			if (*it == gameObject)
			{
				removeGameObjectWithChildren(*it);
				rootGameObjs.erase(it);
				break;
			}
	}
	else
	{
		removeGameObjectWithChildren(gameObject);
	}			
}

void Scene::removeGameObjectWithChildren(GameObject* gameObject)
{
	is_changed = true;
	auto it = allGameObjsByName.find(gameObject->name);
	if (it != allGameObjsByName.end())
	{
		auto& vec = it->second;
		for (auto i = vec.begin();i < vec.end();i++)
			if (*i == gameObject)
			{
				vec.erase(i);
				break;
			}				
	}		
	allGameObjsByID.erase(gameObject->get_id());
	for (auto t : gameObject->transform->children)
		removeGameObjectWithChildren(t->gameObject);
}

void Scene::removeGameObject(int id)
{
	is_changed = true;
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
		if (line.find("Version:1.0") != string::npos)
		{
			deserialize_1_0(ss);
			return;
		}
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
					initRootGameObject(root);
				}
			}
		}
	} while (ss.good()&&line!="SceneEnd");
}
void Scene::serialize_1_0(PHString& str)
{
	str.appendLine("Version:1.0");
	str.appendLine("Scene:", name);
	int size = rootGameObjs.size();
	str.appendLine("rootGameObject:", to_string(size));
	for (int i = 0;i < size;i++)
	{
		rootGameObjs[i]->serialize_1_0(str);
	}
	str.appendLine("SceneEnd");
}

void Scene::deserialize_1_0(std::stringstream& ss)
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
				int size = stoi(line.substr(index + rootGameObjectPrefix.size(), line.size() - 1));
				for (int i = 0;i < size;i++)
				{
					GameObject* root = new GameObject("", false);
					root->deserialize_1_0(ss);
					initRootGameObject(root);
				}
			}
		}
	} while (ss.good() && line != "SceneEnd");
}

const std::unordered_map<int, GameObject*> Scene::getAllGameObjs()
{
	return allGameObjsByID;
}
const std::vector<GameObject*> Scene::getRootGameObjs()
{
	return rootGameObjs;
}



Scene* Scene::loadFromPath(std::string path,Scene* scene)
{
	Debug::logInfo()<<"load scene from path:"<<path <<"\n";
	if(scene==nullptr)
		scene = new Scene();
	stringstream ss(IO::readText(path));
	scene->deserialize(ss);
	scene->is_changed = false;
	return scene;
}


