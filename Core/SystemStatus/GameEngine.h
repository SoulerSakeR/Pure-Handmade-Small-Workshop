#pragma once
#include "GameProject.h"
#include "Core/Core/PHString.h"
#include "Core/Utils/Test.h"

class ResourceMgr;

const std::string sceneExtensionName = ".scene";

/// @brief game engine singleton
class GameEngine
{
public:	
	//method
	static GameEngine& get_instance();	
	Scene* const  getCurrentScene();
	GameProject* const getCurrentGameProject();
	GameProject& creatGameProject(const std::string& name,const std::string& path); //创建新项目
	Vector2D get_resolution();

#ifdef TEST
	bool openGameProjectTest(const std::string& project,const std::string** scenes); //打开已有项目
#endif // TEST

	bool openGameProject(const std::string& path); //打开已有项目
	bool saveGameProject(); //保存当前项目
	GameObject& addGameObject(const std::string& name = "GameObject", GameObject* const parent = nullptr);
	void deleteGameObject(GameObject* obj);
	const std::string& getRootPath();

private:

	GameEngine();
	~GameEngine() {
		//TODO: 析构函数
	};
	bool initialize();
	void renderLoop();
	static GameEngine* instance ; //游戏引擎实例	
	std::string rootPath;
	GameProject* gameProject; //current game project
	ResourceMgr* resourceMgr; //resource manager
};

