#pragma once
#include <vector>
#include <unordered_map>

#include "ISerializable.h"
#include "GameObject.h"
#include "Core/Utils/Test.h"

enum InsertMode { BEFORE, AFTER, INSIDE };

class Scene :
    public ISerializable
{
public:
    
    std::string name;

    //constructor
    Scene(std::string name="ExampleScene");

    //method

    void insertGameObject(GameObject& value, GameObject* target=nullptr,InsertMode insertMode = INSIDE);
    
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
    std::unordered_map<int,GameObject*> allGameObjsByID; //当前场景下所有的GameObject
    std::unordered_multimap<std::string, GameObject*> allGameObjsByName;
    std::vector<GameObject*> rootGameObjs; //当前场景下的根GameObject

    void addGameObject(GameObject* newObject);
    void initRootGameObject(GameObject* rootObject);
};

