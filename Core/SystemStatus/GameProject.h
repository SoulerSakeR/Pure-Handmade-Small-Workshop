#pragma once
#include <vector>

#include "Core/Core/Scene.h"
#include "Core/Core/PHString.h"
#include "Core/Core/Test.h"
#include <string>


class GameProject : public ISerializable
{
public:

	std::string name; //项目名称
	std::string path; //项目路径
	std::vector<Scene*> Scenes; //游戏的所有场景
	Scene* currentScene; //当前游戏场景
	GameProject(const std::string& name,const std::string& path, bool initProject = true);
	bool openScene(int index);
	bool save();
#ifdef TEST
	PHString* saveTest();
	void	deserializeTest(std::stringstream& ss, const std::string ** scene);
#endif
	void deserialize(std::stringstream& ss) override;

private:
	bool saveCurrentScene();
#ifdef TEST
	PHString saveCurrentSceneTest();
#endif
	void serialize(PHString&);
};

