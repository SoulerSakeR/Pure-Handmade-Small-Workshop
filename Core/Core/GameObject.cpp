#include "GameObject.h"

using namespace std;

int GameObject::idCount = 0;

/// @brief serialize game object to custom string
/// @return the reference of the string
void GameObject::serialize(PHString& result)
{
    result.appendLine("GameObject:", to_string(id));
    result.appendLine(name);
    result.appendLine(to_string(isActive));
    result.appendLine("Components:", to_string(components.size()));
    for(int i=0;i<components.size();i++)
    {
        components[i]->serialize(result);
    }
    result.appendLine("ComponentsEnd");
    result.appendLine("Children:", to_string(transform->children.size()));
    for (int i = 0;i < transform->children.size();i++)
    {
        GameObject& child = *(transform->children[i]->gameObject);
        child.serialize(result);
    }
    result.appendLine("ChildrenEnd");
    result.appendLine("GameObjectEnd");
}

void GameObject::deserialize(std::stringstream& ss)
{
    string s;
    do
    {
        getline(ss, s);
        size_t index = s.find("GameObject:");
        if (index != string::npos)
        {
            id = stoi(s.substr(11, s.size() - 1));
            getline(ss, s);
            name = s;
            getline(ss, s);
            isActive = (bool)stoi(s);

            do // deserialize components
            {
                getline(ss, s);
                size_t index = s.find("Components:");
                if (index != string::npos)
                {
                    int componentSize = stoi(s.substr(11, s.size() - 1));
                    for (int i = 0;i < componentSize;i++)
                    {
                        getline(ss, s);
                        Component* component = addComponent((ComponentType)(stoi(s)));
                        component->deserialize(ss);
                    }
                }
            } while (ss.good() && s != "ComponentsEnd");
            
            do // deserialize children game object
            {
                getline(ss, s);
                size_t index = s.find("Children:");
                if (index != string::npos)
                {
                    int childrenSize = stoi(s.substr(9, s.size() - 1));
                    for (int i = 0;i < childrenSize;i++)
                    {
                        GameObject* child = new GameObject();
                        child->deserialize(ss);
                        transform->children.push_back(child->transform);
                        child->transform->parent = transform;                    
                    }
                }
            } while (ss.good() && s != "ChildrenEnd");
        }
    } while (ss.good() && s!="GameObjectEnd");
}

GameObject::GameObject()
{
    name = "";
    isActive = false;
    id = 0;
    components = vector<Component*>();
}

GameObject::GameObject(string name)
{
    this->name = name;
    isActive = false;
    // TODO: 随机生成uuid
    id = idCount + 1;
    idCount++;
    // 
    components = vector<Component*>();
    addComponent(TRANSFORM);
}

/// @brief add component to game object
/// @param type component type 
/// @return the pointer of component
Component* GameObject::addComponent(ComponentType type)
{
    Component* result = nullptr;
    switch (type)
    {
    case TRANSFORM:
        result = new Transform(this);
        transform =(Transform*)result;
        break;
    default:
        break;
    }
    if(result!=nullptr)
        components.push_back(result);
    return result;
}

/// @brief add component to game object
/// @param component the pointer of component
void GameObject::addComponent(Component* component)
{
    if(component!=nullptr)
    {
        component->gameObject = this;
        components.push_back(component);
    }
}


