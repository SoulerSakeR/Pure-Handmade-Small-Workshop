#include "GameProject.h"

GameProject::GameProject(string name, string path)
{
	this->name = name;
	this->path = path;
	this->Scenes = vector<Scene*>();
	this->Scenes.push_back(new Scene());
	// TODO: creat project directory
}

bool GameProject::openScene(int index)
{
	if(Scenes.empty())
    	return false;
	// TODO: refresh hierarchy 刷新面板
	// TODO; render Scene 
	currentScene = Scenes[index];
	return true;
}
