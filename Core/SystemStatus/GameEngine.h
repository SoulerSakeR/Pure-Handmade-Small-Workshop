#pragma once
#include "GameProject.h"
#include "Core/Core/PHString.h"
#include "Core/Utils/Test.h"
#include <Core/UI/mainwindow.h>
#include <Core/ThreadPool/ThreadPool.h>
#include <Core/UI/HierarchyWidget.h>
#include <Core/UI/GameWindow.h>


class ResourceMgr;
class GameLoop;

/// @brief game engine singleton
class GameEngine
{
	friend class MainWindow;
	friend class GameWindow;
public:	
	//method
	static GameEngine& get_instance();	
	Scene* const  getCurrentScene();
	GameProject* getCurrentGameProject();
	GameProject& creatGameProject(const std::string& name,const std::string& path); //创建新项目
	Vector2D get_resolution();
	void set_resolution(const Vector2D& resolution);
	void refreshHierarchy();	
	bool needToRefeshUI(GameObject* gameobj);
#ifdef TEST
	bool openGameProjectTest(const std::string& project,const std::string** scenes); //打开已有项目
#endif // TEST
	MainWindow* getWindow();
	GameWindow* getGameWindow();// add by syj
	bool openGameProject(const std::string& path); //打开已有项目
	bool saveGameProject(); //保存当前项目
	GameObject& addGameObject(const std::string& name = "GameObject", GameObject* const parent = nullptr,Component::ComponentType type = Component::UNKNOWN,InsertMode insertMode = INSIDE);
	void deleteGameObject(GameObject* obj);
	const std::string& getRootPath();
	std::string getGamePath();	
	bool initialize(MainWindow* window);
	bool initializeGame(GameWindow* window); // add by jz
	GameObject* getSelectedGameObject();
	void onPropertyChange(Property* property);
	void test(int a, int b) {
				std::cout << a << " " << b << std::endl;
	}
	bool getInEditor();
	ThreadPool pool;
	GameLoop* gameLoop;
private:

	GameEngine();
	~GameEngine() {
		//TODO: 析构函数
	};
	
	void renderLoop();
	MainWindow* window = nullptr;
	GameWindow* gameWindow = nullptr; // add by jz
	HierarchyWidget* hierarchy;
	static GameEngine* instance ; //游戏引擎实例		
	std::string rootPath;
	GameProject* gameProject; //current game project
	ResourceMgr* resourceMgr; //resource manager
	bool inEditor;
};

