#pragma once
#include <vector>
#include <string>

#include "Core/Core/Scene.h"
#include "Core/Core/PHString.h"


class GameProject : public ISerializable
{
public:

	std::string name; //项目名称
	std::string path; //项目路径
	std::vector<Scene*> Scenes; //游戏的所有场景
	Scene* currentScene; //当前游戏场景
	GameProject(std::string name, std::string path, bool initProject = true);
	bool openScene(int index);
	bool save();
	void deserialize(std::stringstream& ss) override;

private:
	bool saveCurrentScene();
	void serialize(PHString&);
};

