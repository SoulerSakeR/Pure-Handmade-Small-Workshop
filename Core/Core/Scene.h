#pragma once
#include "ISerializable.h"
#include <vector>
#include "GameObject.h"
class Scene :
    public ISerializable
{
public:
    string name;
    vector<GameObject*> allGameObjs; //当前场景下所有的GameObject
    vector<GameObject*> rootGameObjs; //当前场景下的根GameObject
    Scene();
    Scene(string name);
    string& serialize();
};

