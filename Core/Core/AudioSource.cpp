#include "AudioSource.h"
#include "Core/ResourceManagement/ResourceMgr.h"
#include <Core/ResourceManagement/Audio.h>
#include "Core/FileIO/Media.h"

using namespace std;

AudioSource::AudioSource(GameObject* gameObj) :Component(gameObj),IScriptBehaviour(gameObj)
{
	reset();
	componentType = ComponentType::AUDIO_SOURCE;
	auto name_property = new Property("Name", &this->name, Property::ASSET, this);
	name_property->set_property_func<string>(&AudioSource::get_name,&AudioSource::set_name, this);
	name_property->asset_type = PHAsset::AUDIO;
	properties.emplace(name_property);
	auto path_property = new Property("Path", &this->path, Property::STRING, this);
	path_property->set_property_func<string>(&AudioSource::get_path, &AudioSource::set_path, this);
	properties.emplace(path_property);
	auto loop_property = new Property("Loop", &this->loop, Property::INT, this);
	loop_property->set_property_func<int>(&AudioSource::get_loop, &AudioSource::set_loop, this);
	properties.emplace(loop_property);
	auto auto_play_property = new Property("AutoPlay", &this->auto_play, Property::BOOL, this);
	auto_play_property->set_property_func<bool>(&AudioSource::get_auto_play, &AudioSource::set_auto_play, this);
	properties.emplace(auto_play_property);
}

AudioSource::~AudioSource()
{
	if (media != nullptr)
		Media::release_Instance(media);
}

bool AudioSource::setAudio(const std::string& name)
{
	if (ResourceMgr::get_instance().isExist<Audio>(name))
	{
		auto audio = ResourceMgr::get_instance().loadFromName<Audio>(name);
		if(media==nullptr)
			media = Media::get_Instance();
		media->loadAudio(audio->getAbsolutePath().c_str());
		this->name = name;
		this->path = audio->get_path();
		onPropertyChanged(properties["Name"]);
		onPropertyChanged(properties["Path"]);
		return true;
	}
	Debug::logWarning() << "AudioSource::setAudio: " << name << " doesn't exist!\n";
	return false;
}

bool AudioSource::setAudio(const std::string& path, bool isRelativePath)
{
	if (auto audio = ResourceMgr::get_instance().loadFromPath<Audio>(path, isRelativePath);audio!=nullptr)
	{
		if (media == nullptr)
			media = Media::get_Instance();
		media->loadAudio(audio->get_path().c_str());
		this->name = audio->get_name();
		onPropertyChanged(properties["Name"]);
		return true;
	}
	else
	{
		Debug::logWarning() << "AudioSource::setAudio: load audio failed from" << path << " \n";
		return false;
	}
}


void AudioSource::play(int loop)
{
	if (media != nullptr)
	{
		media->playAudio(loop);
	}
}

void AudioSource::pause()
{
	if (media != nullptr)
	{
		media->pauseAudio();
	}
}

void AudioSource::stop()
{
	if (media != nullptr)
	{
		media->stopAudio();
	}
}


std::string AudioSource::get_name()
{
	return name;
}

void AudioSource::set_name(std::string name)
{
	if (name == "None" || name == "")
	{
		this->name = "None";
		this->path = "";
		properties["Path"]->is_editable = true;
		onPropertyChanged(properties["Name"]);
		onPropertyChanged(properties["Path"]);
		return;
	}
	this->name = name;
	if (setAudio(name))
	{
		properties["Path"]->is_editable = false;
	}	
	onPropertyChanged(properties["Name"]);
}

std::string AudioSource::get_path()
{
	return path;
}

void AudioSource::set_path(std::string path)
{
	if (this->path == path || !properties["Path"]->is_editable)
		return;
	this->path = path;
	// if path is relative path
	if (path.find(":") == string::npos)
	{
		setAudio(path, true);
	}
	else
	{
		setAudio(path, false);
	}
	onPropertyChanged(properties["Path"]);
}

int AudioSource::get_loop()
{
	return loop;
}

void AudioSource::set_loop(int loop)
{
	this->loop = loop;
}

bool AudioSource::get_auto_play()
{
	return auto_play;
}

void AudioSource::set_auto_play(bool auto_play)
{
	this->auto_play = auto_play;
}

void AudioSource::reset()
{
	name = "None";
	path = "";
	media = nullptr;
	loop = 1;
	auto_play = true;
}

void AudioSource::awake()
{
	if (auto_play)
	{
		play(loop);
	}
}


