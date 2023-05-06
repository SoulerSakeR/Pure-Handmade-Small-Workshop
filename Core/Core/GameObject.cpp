#include "GameObject.h"
#include "Core/Core/Image.h"
#include <Core/SystemStatus/GameEngine.h>
#include "Camera.h"
#include "RigidBody.h"
#include "BoxCollider.h"
#include "Script.h"
#include "Text.h"
#include "Core/ResourceManagement/SceneMgr.h"
#include "Renderer.h"
#include "SpineAnimator.h"
#include "AudioSource.h"
#include "Light.h"
#include "Core/Core/SpineAnimator.h"

using namespace std;


int GameObject::idCount = 0;

int GameObject::get_id()
{
    return id;
}

Result<void*> GameObject::set_name(const std::string& name)
{
    if (this->name == name)
        return Result<void*>();
    if (auto scene = SceneMgr::get_instance().get_current_scene();scene != nullptr)
    {       
        return scene->renameGameObject(this, name);       
    }
    return Result<void*>();
}

bool GameObject::is_active() const
{
    return is_active_;
}

void GameObject::set_active(bool value)
{
    if (is_active_ == value)
        return;
    is_active_ = value;
    onPropertyChanged(properties["Is Active"]);
}

std::string GameObject::get_tag()
{
    return tag;
}

void GameObject::set_tag(const std::string& tag)
{
    if (SceneMgr::get_instance().get_current_scene() != nullptr)
    {
        SceneMgr::get_instance().tagToGameObjects_remove(this->tag, this);
    }
    SceneMgr::get_instance().get_current_scene()->is_changed = true;
    this->tag = tag;
    if (SceneMgr::get_instance().get_current_scene() != nullptr)
    {
        SceneMgr::get_instance().tagToGameObjects_add(this->tag, this);
    }
}

std::string GameObject::get_name()
{
    return name;
}


/// @brief serialize game object to custom string
/// @return the reference of the string
void GameObject::serialize(PHString& result)
{
    result.appendLine("GameObject:", to_string(id));
    result.appendLine(name);
    result.appendLine(to_string(is_active_));
    result.appendLine(tag);
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
            set_name(s);
            getline(ss, s);
            is_active_ = (bool)stoi(s);
            getline(ss, s);
            set_tag(s);

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
                        Component* component = addComponent((Component::ComponentType)(stoi(s)));
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
                        GameObject* child = new GameObject("", false);
                        child->deserialize(ss);
                        child->transform->parent = transform;
                        transform->children.push_back(child->transform);
                    }
                }
            } while (ss.good() && s != "ChildrenEnd");
        }
    } while (ss.good() && s!="GameObjectEnd");
}

void GameObject::serialize_1_0(PHString& result)
{
    result.appendLine("GameObject:", to_string(id));
    result.appendLine(name);
    result.appendLine(to_string(is_active_));
    result.appendLine(tag);
    result.appendLine("Components:", to_string(components.size()));
    for (int i = 0;i < components.size();i++)
    {
        components[i]->serialize_1_0(result);
    }
    result.appendLine("ComponentsEnd");
    result.appendLine("Children:", to_string(transform->children.size()));
    for (int i = 0;i < transform->children.size();i++)
    {
        GameObject& child = *(transform->children[i]->gameObject);
        child.serialize_1_0(result);
    }
    result.appendLine("ChildrenEnd");
    result.appendLine("GameObjectEnd");
}

void GameObject::deserialize_1_0(std::stringstream& ss)
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
            set_name(s);
            getline(ss, s);
            is_active_ = (bool)stoi(s);
            getline(ss, s);
            set_tag(s);

            do // deserialize components
            {
                getline(ss, s);
                size_t index = s.find("Components:");
                if (index != string::npos)
                {
                    int componentSize = stoi(s.substr(11));
                    for (int i = 0;i < componentSize;i++)
                    {
                        getline(ss, s);
                        auto index = s.find("Component:");
                        if (index != string::npos)
                        {
							s = s.substr(10);
                            Component* component = addComponent((Component::ComponentType)(stoi(s)));
                            component->deserialize_1_0(ss);
						}                      
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
                        GameObject* child = new GameObject("", false);
                        child->deserialize_1_0(ss);
                        child->transform->parent = transform;
                        transform->children.push_back(child->transform);
                    }
                }
            } while (ss.good() && s != "ChildrenEnd");
        }
    } while (ss.good() && s != "GameObjectEnd");
}


GameObject::GameObject(string name,bool withTransform)
{
    id = idCount + 1;
    this->name = name;
    tag = "Untagged";
    is_active_ = true;
    auto name_property = new Property("Name", &this->name,Property::STRING,this);
    name_property->set_property_func<string>(&GameObject::get_name,&GameObject::set_name,this);
    properties.emplace(name_property);
    auto is_active_property = new Property("Is Active", &is_active_, Property::BOOL, this);
    is_active_property->set_property_func<bool>(&GameObject::is_active, &GameObject::set_active, this);
    properties.emplace(is_active_property);
    auto tag_property = new Property("Tag", &tag, Property::STRING, this);
    tag_property->set_property_func<string>(&GameObject::get_tag, &GameObject::set_tag, this);
    properties.emplace(tag_property);

    idCount++;
    components = vector<Component*>();
    if(withTransform)
        addComponent<Transform>();
}

GameObject::~GameObject()
{
    if (transform->parent != nullptr)
    {
        auto it = std::find(transform->parent->children.begin(), transform->parent->children.end(), transform);
        if (it != transform->parent->children.end())
			transform->parent->children.erase(it);
    }       
    auto children = transform->children.data();
    for (int i=0;i<transform->children.size();++i)
    {
        delete children[i]->gameObject;
    }
    for (auto component : components)
    {
        delete component;
    }

}

/// @brief add component to game object
/// @param type component type 
/// @return the pointer of component
Component* GameObject::addComponent(Component::ComponentType type)
{
    Component* result = nullptr;
    if (getComponent(type) != nullptr)
        return result;
    switch (type)
    {
    case Component::TRANSFORM:
        result = new Transform(this);
        transform =(Transform*)result;
        break;
    case Component::IMAGE:
        result = new Image(this);
        break;
    case Component::CAMERA:
        result = new Camera(this);
        break;
    case Component::RIGID_BODY:
        result = new RigidBody(this);
		break;
    case Component::BOX_COLLIDER:
        result = new BoxCollider(this);
		break;
    case Component::SCRIPT:
		result = new Script(this);
		break;
    case Component::TEXT:
        result = new Text(this);
		break;
    case Component::RENDERER:
        result = new Renderer(this);
		break;
    case Component::SPINE_ANIMATOR:
        result = new SpineAnimator(this);
		break;
    case Component::AUDIO_SOURCE:
        result = new AudioSource(this);
        break;
    case Component::LIGHT:
        result = new Light(this);
		break;
    default:
        break;
    }
    if (result != nullptr)
    {
        result->gameObject = this;
        components.push_back(result);
        result->onPropertyChanged.registerFunc(&GameObject::onComponentPropertyChangedHandler, this);
    }      
    return result;
}

Component* GameObject::getComponent(Component::ComponentType type)
{
    for (auto component : components)
    {
        if (component->componentType == type)
            return component;
    }
    return nullptr;
}

SpineAnimator* GameObject::getAnimator()
{
    return getComponent<SpineAnimator>();
}

void GameObject::removeComponent(Component* component)
{
    if (component->componentType == Component::TRANSFORM)
    {
        Debug::logWarning()<<"Can not remove transform component\n";
        return;
    }
    for (int i = 0;i < components.size();i++)
    {
        if (components[i] == component)
        {
			components.erase(components.begin() + i);
			delete component;
			return;
		}
	}
}


bool GameObject::isRootGameObject() const
{
    return transform->parent == nullptr;
}

void GameObject::destroy()
{
    GameEngine::get_instance().deleteGameObject(this);
    delete this;
}

GameObject* GameObject::clone(const std::string newName, GameObject* parent)
{
    string new_name = newName;
    if(new_name == name && new_name.find("_clone") != string::npos)
    {
        new_name.append("_clone");
	}
    GameObject* result = new GameObject(newName, false);
    result->name = newName;
    result->tag = tag;
    result->is_active_ = is_active();
    stringstream ss;
    for (auto component : components)
    {
        ss.clear();		
        PHString str;
        component->serialize(str);
        ss << str.str();
        string line;
        getline(ss, line);
        Component* newComponent = result->addComponent(component->componentType);
		newComponent->deserialize(ss);
	}
    for (auto child : transform->children)
    {
		child->gameObject->clone(child->gameObject->name,result);
	}
    SceneMgr::get_instance().current_scene->insertGameObject(*result,parent,INSIDE);
    return result;
}

void GameObject::onComponentPropertyChangedHandler(Property* property)
{
    if (SceneMgr::get_instance().hasCurrentScene())
    {
        auto& map = SceneMgr::get_instance().get_current_scene()->allGameObjsByID;
        if (map.find(id) == map.end())
            return;
        SceneMgr::get_instance().get_current_scene()->is_changed = true;
        onPropertyChanged(property);
    }        
}

GameObject* GameObject::find(const std::string& name)
{
    return SceneMgr::get_instance().findGameObjectByName(name);
}

std::vector<GameObject*> GameObject::findTag(const std::string& tag)
{
    return SceneMgr::get_instance().findGameObjectsByTag(tag);
}




