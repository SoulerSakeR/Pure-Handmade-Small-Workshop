#pragma once
#include <vector>
#include <unordered_map>
#include <map>

#include "ISerializable.h"
#include "GameObject.h"
#include "Core/Utils/Test.h"
#include "Core/Utils/Result.h"

enum InsertMode { BEFORE, AFTER, INSIDE };

class Scene :
    public ISerializable
{
public:
    
    std::string name;

    //constructor
    Scene(std::string name="ExampleScene");

    //method
    Result<void*> renameGameObject(GameObject* gameObject, std::string newName);
    void insertGameObject(GameObject& value, GameObject* target=nullptr,InsertMode insertMode = INSIDE);

    void insertExistGameObject(GameObject* gameObject, GameObject* target, InsertMode insertMode);
    
    void removeGameObject(GameObject* gameObject);
    void removeGameObject(int id);
    void serialize(PHString&) override;
    void deserialize(std::stringstream& ss) override;
    const std::unordered_map<int, GameObject*> getAllGameObjs();
    const std::vector<GameObject*> getRootGameObjs();
    std::map<int, GameObject*>& getAllGameObjsByDepth();

#ifdef TEST
    static Scene* loadFromText(const std::string& text);
#endif // TEST
    static Scene* loadFromPath(std::string path,Scene* scene = nullptr);
private:
    std::unordered_map<int,GameObject*> allGameObjsByID; //当前场景下所有的GameObject
    std::unordered_map<std::string, std::vector<GameObject*>> allGameObjsByName;
    std::map<int, GameObject*> allGameObjsByDepth;
    std::vector<GameObject*> rootGameObjs; //当前场景下的根GameObject

    void addGameObject(GameObject* newObject);
    void addGameObjectWithChildren(GameObject* newObject);
    void removeGameObjectWithChildren(GameObject* gameObject);
    void initRootGameObject(GameObject* rootObject);

};

