#pragma once
#include <vector>
#include <unordered_map>

#include "ISerializable.h"
#include "GameObject.h"

class Scene :
    public ISerializable
{
public:
    
    std::string name;

    //constructor
    Scene();
    Scene(std::string name);

    //method
    void addRootGameObject(GameObject* newObject);
    void addGameObject(GameObject* newObject);
    std::string& serialize();
    static Scene* deserialize(std::stringstream ss);

private:
    std::unordered_map<int,GameObject*> allGameObjs; //当前场景下所有的GameObject
    std::vector<GameObject*> rootGameObjs; //当前场景下的根GameObject
};

