#include "GameProject.h"

using namespace std;

const std::string sceneExtensionName = ".scene";
const std::string gameProjectPrefix = "GameProject:";
const std::string gameProjectPostfix = "GameProject:";
const std::string scenePrefix = "Scene:";
const std::string sceneProjectPostfix = "SceneEnd:";

GameProject::GameProject(string name, string path, bool withDefaultScene)
{
	this->name = name;
	this->path = path;
	this->Scenes = vector<Scene*>();
	if(withDefaultScene)
		this->Scenes.push_back(new Scene());
	// TODO: creat project directory
}

bool GameProject::openScene(int index)
{
	if(Scenes.empty())
    	return false;
	// TODO: refresh hierarchy 刷新面板
	// TODO: render Scene 
	currentScene = Scenes[index];
	return true;
}

bool GameProject::save()
{
	saveCurrentScene();
	PHString content = PHString("");
	serialize(content);
	// TODO: 写入文件
	// file.write(gameProject,path+".gameProject");
    return true;
}

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
						Scene* scene = Scene::loadFromPath(s);
						Scenes.push_back(scene);
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
	string path = this->path;
	path.append("/Scene/");
	path.append(fileName);
	// TODO: 写入文件
	// file.write(scene,path);
	return true;		
}

void GameProject::serialize(PHString& result)
{
	result.appendLine("GameProject:",name);
	result.appendLine(path);
	result.appendLine("Scene:",to_string(Scenes.size()));
	for(int i=0;i<Scenes.size();i++)
	{
		string scenePath = path + "/Scene/" + Scenes[i]->name + sceneExtensionName;
		result.appendLine(scenePath);
	}
	result.appendLine("SceneEnd");
	result.appendLine("GameProjectEnd");
}
