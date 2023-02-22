#include "GameProject.h"

GameProject::GameProject(string name, string path)
{
	this->name = name;
	this->path = path;
	this->Scenes = vector<Scene*>();
	this->Scenes.push_back(new Scene());
}

bool GameProject::openScene(int index)
{
	if(Scenes.empty())
    	return false;
	// TODO: hierarchy 刷新面板
	// TODO; render 渲染场景 
	currentScene = Scenes[index];
	return true;
}
