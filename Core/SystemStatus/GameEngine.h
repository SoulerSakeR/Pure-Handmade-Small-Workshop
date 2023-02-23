#pragma once
#include "GameProject.h"
#include "Core/ResourceManagement/ResourceMgr.h"
class GameEngine
{
public:
	static GameEngine* instance; //单例游戏引擎
	GameProject* gameProject; //当前游戏项目
	ResourceMgr* resourceMgr; //资源管理器
	static GameEngine* getInstance();
	GameProject* creatGameProject(string name, string path); //创建新项目
	bool openGameProject(string path); //打开已有项目
	bool saveGameProject(); //保存当前项目
};

