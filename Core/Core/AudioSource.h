#pragma once
#include "Component.h"

class AudioSource : public Component
{
public :
	AudioSource(GameObject* gameObj);

	// api for lua script ÐèÒª°ó¶¨
	void setAudio(std::string name);
	void setAudio(std::string path, bool isRelativePath);
	void play(int loop);
	void stop();

	// getters and setters

	std::string get_name();
	void set_name(std::string name);

	std::string get_path();
	void set_path(std::string path);

	// inherited via Component
	virtual void reset() override;
	

private:
	std::string name;
	std::string path;
};

