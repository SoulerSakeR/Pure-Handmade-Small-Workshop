#include "GameObject.h"

using namespace std;

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
    result.append(to_string(components.size()));
    result.append("\n");
    for(int i=0;i<components.size();i++)
    {
        result.append(components[i]->serialize());
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
    components = vector<Component*>();
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
        components.push_back(result);
    return result;
}

void GameObject::addComponent(Component* component)
{
    if(component!=nullptr)
    {
        component->gameObject = this;
        components.push_back(component);
    }
}
