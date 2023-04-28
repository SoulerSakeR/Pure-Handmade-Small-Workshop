#pragma once
#include "Component.h"
#include "Core/Utils/PHPath.h"
#include "IScriptBehaviour.h"
#include "GameObject.h"

class Script : public Component , public IScriptBehaviour
{
public:
    Script(GameObject* gameObj,const std::string& name = "",const std::string& path = "");

    void reset() override;
    
    // getters and setters
    std::string get_name();
    void set_name(const std::string& name);
    std::string get_path();
    void set_path(const std::string& path);

    // inherited via IScriptBehaviour
    void awake() override;
    void start() override;
    void beforeUpdate() override;
    void update() override;
    void afterUpdate() override;

protected:
    void serialize(PHString& str) override;
	void deserialize(std::stringstream& ss) override;
    void set_property(Property* property, void* value) override;

    std::string name;
    PHPath path;
    sol::state* lua;

    friend class GameLoop;
};


