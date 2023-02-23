#pragma once
#include "ISerializable.h"
#include <vector>
#include "GameObject.h"

class Scene :
    public ISerializable
{
public:
    std::string name;
    std::vector<GameObject*> allGameObjs; //当前场景下所有的GameObject
    std::vector<GameObject*> rootGameObjs; //当前场景下的根GameObject
    Scene();
    Scene(std::string name);
    std::string& serialize();
    Scene* deserialize(std::string value);
};

