#include "GameProject.h"
#include "Core/Core/Debug.h"
#include "Core/FileIO/IO.h"
#include "Core/Core/Scene.h"
#include <Core/ResourceManagement/SceneMgr.h>
#include <Core/Utils/PHPath.h>
#include "GameEngine.h"
#include <Core/GameLogic/GameLoop.h>
#include <Core/ThreadPool/ThreadPool.h>

using namespace std;

const std::string sceneExtensionName = ".scene";
const std::string gameProjectPrefix = "GameProject:";
const std::string gameProjectPostfix = "GameProject:";
const std::string scenePrefix = "Scene:";
const std::string sceneProjectPostfix = "SceneEnd:";

GameProject::GameProject(const string& name,const string& path, bool initDefaultScene):name(name), path(path)
{
	this->scenes = vector<string>();
	currentScene = nullptr;
}

bool GameProject::openScene(int index)
{
	if(scenes.empty())
    	return false;
	if (index >= 0 && index < scenes.size())
	{				
		currentScene = SceneMgr::get_instance().loadScene(index);
		GameEngine::get_instance().refreshHierarchy();
		auto loop = std::bind(&GameLoop::updateScene, new GameLoop(&RenderWidget::getInstance()), &RenderWidget::getInstance());
		GameEngine::get_instance().pool.enqueue(loop).get();
		for (int i = 0; i < 10; i++) {
			PHPath path = PHPath("C:/Program Files/My/App.exe");
			auto getOldPath = std::bind(&PHPath::getOldPath, &path);
			auto oldpath = GameEngine::get_instance().pool.enqueue(getOldPath).get();// pool.enqueue(函数的地址，对象实例的地址，参数1，参数2 ...), 然后用.get()获取返回值
			std::cout << i << "-OldPath: " << oldpath << std::endl;
			std::cout << i << "-NewPath: " << path.getNewPath() << std::endl;
		}
		return true;
	}
	// TODO: refresh hierarchy 刷新面板
	// TODO: render Scene 
	
}

bool GameProject::save()
{
	IO::createPathIfNotExists(path.getNewPath());
	saveCurrentScene();
	PHString content = PHString("");
	serialize(content);	
	IO::write(content.str(), path.combinePath(name + ".gameProject").getNewPath(), 1);
    return true;
}

void GameProject::creatNewScene(const std::string& name)
{
	Scene* scene = new Scene(name);
	currentScene = scene;
	auto& cam = GameEngine::get_instance().addGameObject("MainCamera",nullptr,CAMERA);
	cam.addComponent<Camera>()->view_width = 2000;
	cam.getComponent<Camera>()->set_main_camera(true);
	saveCurrentScene();
	scenes.push_back(scene->name);
	SceneMgr::get_instance().addScene("\\Scenes\\" + name + sceneExtensionName);
}

#ifdef TEST
PHString* GameProject::saveTest()
{
	auto scene = new PHString(saveCurrentSceneTest());
	PHString* projectConfig = new PHString("");
	serialize(*projectConfig);
	PHString* result= new PHString[2]{*projectConfig,*scene};
	// TODO: 写入文件
	// file.write(gameProject,path+".gameProject");
	return result;
}
void GameProject::deserializeTest(std::stringstream& ss, const std::string** scenes)
{
	string s;
	do
	{
		getline(ss, s);
		auto index = s.find(gameProjectPrefix);
		if (index != string::npos)
		{
			name = s.substr(index + gameProjectPrefix.size(), s.size() - 1);
			Debug::log("read "+name);
			getline(ss, s);
			path = s;
			do
			{
				getline(ss, s);
				auto index = s.find(scenePrefix);
				if (index != string::npos)
				{
					int size = stoi(s.substr(index + scenePrefix.size(), s.size() - 1));
					for (int i = 0;i < size;i++)
					{
						Scene* scene = Scene::loadFromText(*(scenes[i]));
						scenes.push_back(scene);
					}
				}
			} while (ss.good() && s != sceneProjectPostfix);
		}
	} while (ss.good() && s != gameProjectPostfix);
}
#endif // TEST

void GameProject::deserialize(std::stringstream& ss)
{
	string s;
	do 
	{
		getline(ss, s);
		auto index = s.find(gameProjectPrefix);
		if (index != string::npos)
		{
			name = s.substr(index + gameProjectPrefix.size(), s.size() - 1);
			getline(ss, s);
			path = s;
			do
			{
				getline(ss, s);
				auto index = s.find(scenePrefix);
				if (index != string::npos)
				{
					int size = stoi(s.substr(index + scenePrefix.size(), s.size() - 1));
					for (int i = 0;i < size;i++)
					{
						getline(ss, s);
						scenes.push_back(PHPath(s).getFileName(false));
						SceneMgr::get_instance().addScene(s);
					}
				}
			} while (ss.good() && s != sceneProjectPostfix);
		}
	} while (ss.good()&&s!=gameProjectPostfix);	
}

bool GameProject::saveCurrentScene()
{
	if(currentScene==nullptr)
		return false;
	PHString scene = PHString("");
	currentScene->serialize(scene);
	string fileName = currentScene->name + sceneExtensionName;
	IO::createPathIfNotExists(path.combinePath("Scenes").getNewPath());
	return IO::write(scene.str(), path.combinePath("Scenes").combinePath(fileName).getNewPath(), 1);
}

#ifdef TEST
PHString GameProject::saveCurrentSceneTest()
{
	PHString scene = PHString("");
	if (currentScene == nullptr)
		return scene;
	currentScene->serialize(scene);
	return scene;
}
#endif // TEST


void GameProject::serialize(PHString& result)
{
	result.appendLine("GameProject:",name);
	result.appendLine(path.getNewPath());
	result.appendLine("Scene:",to_string(scenes.size()));
	for(int i=0;i<scenes.size();i++)
	{
		string scenePath = "\\Scenes\\" + scenes[i] + sceneExtensionName;
		result.appendLine(scenePath);
	}
	result.appendLine("SceneEnd");
	result.appendLine("GameProjectEnd");
}
