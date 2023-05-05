#pragma once
#include "Component.h"
#include "IScriptBehaviour.h"

class Media;

class AudioSource : public Component , public IScriptBehaviour
{
public :
	AudioSource(GameObject* gameObj);
	virtual ~AudioSource();
	// api for lua script 需要绑定
	bool setAudio(const std::string& name);
	bool setAudio(const std::string& path, bool isRelativePath);
	void play(int loop);
	void pause();
	void stop();

	// getters and setters

	std::string get_name();
	void set_name(std::string name);

	std::string get_path();
	void set_path(std::string path);

	int get_loop();
	void set_loop(int loop);

	bool get_auto_play();
	void set_auto_play(bool auto_play);

	// inherited via Component
	virtual void reset() override;
	
	// inherited via IScriptBehaviour
	virtual void awake() override;

private:
	std::string name;
	std::string path;
	int loop;
	bool auto_play;
	Media* media;
};

