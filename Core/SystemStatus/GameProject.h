#pragma once
#include <vector>
#include <string>

#include "Core/Core/Scene.h"
using namespace std;
class GameProject
{
public:
	string name; //项目名称
	string path; //项目路径
	vector<Scene*> Scenes; //游戏的所有场景
	Scene* currentScene; //当前游戏场景
	GameProject(string name, string path);
	bool openScene(int index);
};

