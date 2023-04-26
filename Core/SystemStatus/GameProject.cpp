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
const std::string SCENE_POSTFIX = "SceneEnd";

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
		GameEngine::get_instance().gameLoop = new GameLoop();
		if (GameEngine::get_instance().getInEditor()) {
			auto renderLoop = std::bind(&GameLoop::updateScene, GameEngine::get_instance().gameLoop, &RenderWidget::getSceneWidget());
			GameEngine::get_instance().pool.enqueue(renderLoop);
			auto gameLoop = std::bind(&GameLoop::updateScene, GameEngine::get_instance().gameLoop, &RenderWidget::getGameWidget());
			GameEngine::get_instance().pool.enqueue(gameLoop);
		}
		else {
			auto exportGameLoop = std::bind(&GameLoop::updateScene, GameEngine::get_instance().gameLoop, RenderWidget::currentWidget);
			GameEngine::get_instance().pool.enqueue(exportGameLoop);
		}

		
		return true;
	}	
	return false;
}

bool GameProject::openScene(const std::string& name)
{
	for (auto& s : scenes)
	{
		if (s == name)
		{
			PHPath path = PHPath(GameEngine::get_instance().getGamePath()).combinePath("\\Scenes\\" + name + sceneExtensionName);
			currentScene = SceneMgr::get_instance().loadScene(path.getNewPath());
			GameEngine::get_instance().refreshHierarchy();
			GameEngine::get_instance().gameLoop = new GameLoop();
			if (GameEngine::get_instance().getInEditor()) {
				auto renderLoop = std::bind(&GameLoop::updateScene, GameEngine::get_instance().gameLoop, &RenderWidget::getSceneWidget());
				GameEngine::get_instance().pool.enqueue(renderLoop);
				auto gameLoop = std::bind(&GameLoop::updateScene, GameEngine::get_instance().gameLoop, &RenderWidget::getGameWidget());
				GameEngine::get_instance().pool.enqueue(gameLoop);
			}
			else {
				auto exportGameLoop = std::bind(&GameLoop::updateScene, GameEngine::get_instance().gameLoop, RenderWidget::currentWidget);
				GameEngine::get_instance().pool.enqueue(exportGameLoop);
			}
			return true;
		}
	}
	Debug::warningBox(GameEngine::get_instance().getWindow(),"Scene not found!");
	return false;
}

bool GameProject::openScene(Scene* scene)
{
	GameEngine::get_instance().refreshHierarchy();
	GameEngine::get_instance().gameLoop = new GameLoop();
	if (GameEngine::get_instance().getInEditor()) {
		auto renderLoop = std::bind(&GameLoop::updateScene, GameEngine::get_instance().gameLoop, &RenderWidget::getSceneWidget());
		GameEngine::get_instance().pool.enqueue(renderLoop);
		auto gameLoop = std::bind(&GameLoop::updateScene, GameEngine::get_instance().gameLoop, &RenderWidget::getGameWidget());
		GameEngine::get_instance().pool.enqueue(gameLoop);
	}
	else {
		auto exportGameLoop = std::bind(&GameLoop::updateScene, GameEngine::get_instance().gameLoop, RenderWidget::currentWidget);
		GameEngine::get_instance().pool.enqueue(exportGameLoop);
	}
	return true;
}

void GameProject::deleteScene(const std::string name)
{
	for (auto& s : scenes)
	{
		if (s == name)
		{
			PHPath path = PHPath("\\Scenes\\" + name + sceneExtensionName);
			auto it = std::find(scenes.begin(), scenes.end(), name);
			if (it != scenes.end())
			{
				scenes.erase(it);
			}
			it = std::find(SceneMgr::get_instance().scenes.begin(), SceneMgr::get_instance().scenes.end(), path.getNewPath());
			if(it != SceneMgr::get_instance().scenes.end())
			{
				SceneMgr::get_instance().scenes.erase(it);
			}
		}
	}
}

bool GameProject::importScene(const std::string& path)
{
	currentScene = SceneMgr::get_instance().loadScene(path);
	GameEngine::get_instance().refreshHierarchy();
	auto renderLoop = std::bind(&GameLoop::updateScene, GameEngine::get_instance().gameLoop, &RenderWidget::getSceneWidget());
	GameEngine::get_instance().pool.enqueue(renderLoop);
	auto gameLoop = std::bind(&GameLoop::updateScene, GameEngine::get_instance().gameLoop, &RenderWidget::getGameWidget());
	GameEngine::get_instance().pool.enqueue(gameLoop);
	return true;
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

Scene* GameProject::creatNewScene(const std::string& name)
{
	Scene* scene = new Scene(name);
	currentScene = scene;
	auto& cam = GameEngine::get_instance().addGameObject("MainCamera",nullptr, Component::CAMERA);
	cam.getComponent<Camera>()->set_main_camera(true);
	scenes.push_back(scene->name);
	SceneMgr::get_instance().addScene("\\Scenes\\" + name + sceneExtensionName);
	SceneMgr::get_instance().current_scene = scene;
	return scene;
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
			} while (ss.good() && s != SCENE_POSTFIX);
		}
	} while (ss.good() && s != gameProjectPostfix);
}
#endif // TEST

void GameProject::deserialize(std::stringstream& ss)
{
	Debug::logInfo()<<"Deserialing gameProject \n";
	bool skipLine = false;
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

			Debug::logInfo() << "Deserialing scenes paths \n";
			do //deserialize scenes
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
				else if(s != SCENE_POSTFIX)
				{
					skipLine = true;
					Debug::logError() << "Can not find scene saved \n";
					break;
				}
			} while (ss.good() && s != SCENE_POSTFIX);

			do //deserialize render settings
			{
				if (skipLine) {
					skipLine = false;					
				}else{
					getline(ss, s);
				}
				auto index = s.find(RENDER_SETTING_PREFIX);
				if (index != string::npos)
				{
					render_setting = new RenderSetting();
					render_setting->deserialize(ss);
					break;
				}
				else if (s != RENDER_SETTING_POSTFIX)
				{
					skipLine = true;
					render_setting = RenderSetting::getDefaultSetting();
					Debug::logError() << "Can not find render setting saved! \n";
					break;
				}
			} while (ss.good() && s!=RENDER_SETTING_POSTFIX);
		}
	} while (ss.good()&&s!=gameProjectPostfix);	
	Debug::logInfo() << "Deserialing gameProject finished \n";
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
	render_setting->serialize(result);
	result.appendLine("GameProjectEnd");
}
