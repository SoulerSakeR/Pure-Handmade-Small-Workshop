#pragma once
#include <vector>
#include <unordered_map>

#include "ISerializable.h"
#include "GameObject.h"
#include "Core/Utils/Test.h"

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
    void removeGameObject(GameObject* gameObject);
    void removeGameObject(int id);
    void serialize(PHString&) override;
    void deserialize(std::stringstream& ss) override;
    const std::unordered_map<int, GameObject*> getAllGameObjs();
#ifdef TEST
    static Scene* loadFromText(const std::string& text);
#endif // TEST
    static Scene* loadFromPath(std::string path);
private:
    std::unordered_map<int,GameObject*> allGameObjs; //当前场景下所有的GameObject
    std::vector<GameObject*> rootGameObjs; //当前场景下的根GameObject
};

