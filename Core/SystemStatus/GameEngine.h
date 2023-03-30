#pragma once
#include "GameProject.h"
#include "Core/Core/PHString.h"
#include "Core/Core/Test.h"

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
	Scene& getCurrentScene();
	GameProject* creatGameProject(std::string name, std::string path); //创建新项目
#ifdef TEST
	bool openGameProjectTest(const std::string& project,const std::string** scenes); //打开已有项目
#endif // TEST
	bool openGameProject(const std::string& path); //打开已有项目
	bool saveGameProject(); //保存当前项目
	GameObject* addGameObject(const std::string& name = "GameObject", GameObject* parent = nullptr);
	void deleteGameObject(GameObject* obj);
	std::string& getRootPath();

private:
	static GameEngine* instance ; //游戏引擎实例
	std::string rootPath;
};

