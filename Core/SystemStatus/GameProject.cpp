#include "GameProject.h"
#include "Core/Core/Debug.h"
#include "Core/FileIO/IO.h"

using namespace std;

const std::string sceneExtensionName = ".scene";
const std::string gameProjectPrefix = "GameProject:";
const std::string gameProjectPostfix = "GameProject:";
const std::string scenePrefix = "Scene:";
const std::string sceneProjectPostfix = "SceneEnd:";

GameProject::GameProject(const string& name,const string& path, bool initDefaultScene)
{
	this->name = name;
	this->path = path;
	this->Scenes = vector<Scene*>();
	currentScene = nullptr;
	if (initDefaultScene)
	{
		this->Scenes.push_back(new Scene());
	}	
}

bool GameProject::openScene(int index)
{
	if(Scenes.empty())
    	return false;
	if (index >= 0 && index < Scenes.size())
	{
		currentScene = Scenes[index];
		return true;
	}
	// TODO: refresh hierarchy 刷新面板
	// TODO: render Scene 
	
}

bool GameProject::save()
{
	IO::createPathIfNotExists(path);
	saveCurrentScene();
	PHString content = PHString("");
	serialize(content);	
	IO::write(content.str(), path+"\\"+name + ".gameProject", 1);
    return true;
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
						Scenes.push_back(scene);
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
						Scene* scene = Scene::loadFromText(IO::readText(s));
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
	string final_path = this->path;
	final_path.append("\\Scene");
	IO::createPathIfNotExists(final_path);
	final_path.append("\\").append(fileName);
	return IO::write(scene.str(), final_path, 1);;
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
	result.appendLine(path);
	result.appendLine("Scene:",to_string(Scenes.size()));
	for(int i=0;i<Scenes.size();i++)
	{
		string scenePath = path + "\\Scene\\" + Scenes[i]->name + sceneExtensionName;
		result.appendLine(scenePath);
	}
	result.appendLine("SceneEnd");
	result.appendLine("GameProjectEnd");
}
