#include "GameObject.h"

string& GameObject::serialize()
{
    string result = "GameObject:";
    result.append(to_string(id));    
    result.append("\n");
    result.append(name);
    result.append("\n");
    result.append(to_string(isActive));
    result.append("\n");
    result.append("Components:");
    result.append(to_string(Components.size()));
    result.append("\n");
    for(int i=0;i<Components.size();i++)
    {
        result.append(Components[i]->serialize());
    }
    result.append("ComponentsEnd");
    result.append("Children:");
    result.append(to_string(transform->children.size()));
    result.append("\n");
    for (int i = 0;i < transform->children.size();i++)
    {
        GameObject& child = *(transform->children[i]->gameObject);
        result.append(child.serialize());
    }
    result.append("ChildrenEnd\n");
    result.append("GameObject{");
    result.append(to_string(id));
    result.append("}End\n");
    return result;
}

GameObject::GameObject(string name)
{
    this->name = name;
    isActive = false;
    // TODO: 随机生成uuid
    Components = vector<Component*>();
    addComponent(TRANSFORM);
}

Component* GameObject::addComponent(ComponentType type)
{
    Component* result = nullptr;
    switch (type)
    {
    case TRANSFORM:
        result = new Transform(this);
        break;
    default:
        break;
    }
    if(result!=nullptr)
        Components.push_back(result);
    return result;
}
