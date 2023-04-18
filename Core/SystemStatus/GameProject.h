#pragma once
#include <vector>

#include "Core/Core/Scene.h"
#include "Core/Core/PHString.h"
#include "Core/Utils/Test.h"
#include "Core/Utils/PHPath.h"
#include <string>


class GameProject : public ISerializable
{
public:	
	GameProject(const std::string& name,const std::string& path, bool initProject = true);

	bool openScene(int index);
	bool openScene(const std::string& name);
	bool openScene(Scene* scene);
	void deleteScene(const std::string name);
	bool importScene(const std::string& path);
	bool save();
	Scene* creatNewScene(const std::string& name = "ExampleScene");

	std::string name; //项目名称
	PHPath path; //项目路径
	std::vector<std::string> scenes; //游戏的所有场景
	Scene* currentScene; //当前游戏场景
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

