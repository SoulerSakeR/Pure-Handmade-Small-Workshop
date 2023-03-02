#pragma once
#include "GameProject.h"
#include "Core/Core/PHString.h"

class ResourceMgr;

const std::string sceneExtensionName = ".scene";

/// @brief game engine singleton
class GameEngine
{
public:	
	//field
	GameProject* gameProject; //current game project
	ResourceMgr* resourceMgr; //resource manager

	//constructor
	GameEngine();
	~GameEngine(){
		//TODO: 析构函数
	};

	//method
	static GameEngine* getInstance();
	bool initialize();
	GameProject* creatGameProject(string name, string path); //创建新项目
	bool openGameProject(string path); //打开已有项目
	bool saveGameProject(); //保存当前项目
	GameObject* addGameObject(GameObject* parent=nullptr, std::string name = "GameObject");

private:
	static GameEngine* instance ; //游戏引擎实例
};

