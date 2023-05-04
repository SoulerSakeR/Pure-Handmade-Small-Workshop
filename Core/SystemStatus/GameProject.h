#pragma once
#include <vector>

#include "Core/Core/Scene.h"
#include "Core/Core/PHString.h"
#include "Core/Utils/Test.h"
#include "Core/Utils/PHPath.h"
#include <string>
#include "Core/Render/RenderSetting.h"


class GameProject : public ISerializable
{
	friend class GameEngine;
public:	
	GameProject(const std::string& name,const std::string& path, bool initProject = true);

	bool openScene(int index);
	bool openScene(const std::string& name);
	bool openScene(Scene* scene);
	void deleteScene(const std::string name);
	bool importScene(const std::string& path);
	bool isChanged();
	bool save();
	Scene* creatNewScene(const std::string& name = "ExampleScene");
	int get_current_scene_index() const {return current_scene_index<scenes.size()?current_scene_index:0;}

	std::string name; //项目名称
	PHPath path; //项目路径
	std::vector<std::string> scenes; //游戏的所有场景
	Scene* currentScene; //当前游戏场景
	void deserialize(std::stringstream& ss) override;

	virtual void serialize_1_0(PHString& str) override;
	virtual void deserialize_1_0(std::stringstream& ss) override;

private:
	bool saveCurrentScene();
	void serialize(PHString&);
	bool is_changed;
	int current_scene_index;

	RenderSetting* render_setting;
};

