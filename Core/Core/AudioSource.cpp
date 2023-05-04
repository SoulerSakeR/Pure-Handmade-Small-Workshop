#include "AudioSource.h"
#include "Core/ResourceManagement/ResourceMgr.h"

AudioSource::AudioSource(GameObject* gameObj) :Component(gameObj)
{
	reset();
	componentType = ComponentType::AUDIO_SOURCE;
}

void AudioSource::setAudio(std::string name)
{

}


std::string AudioSource::get_name()
{
	return name;
}

void AudioSource::set_name(std::string name)
{
	this->name = name;
}

std::string AudioSource::get_path()
{
	return path;
}

void AudioSource::set_path(std::string path)
{
	this->path = path;
}

void AudioSource::reset()
{
	name = "None";
	path = "";
}


