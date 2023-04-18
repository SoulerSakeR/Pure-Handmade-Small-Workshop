#pragma once
#include "Component.h"
#include "Core/Utils/PHPath.h"

class Script : public Component
{
public:
    Script(GameObject* gameObj,const std::string& name = "",const std::string& path = "");

    void reset() override;
    
    std::string get_name();
    void set_name(const std::string& name);
    std::string get_path();
    void set_path(const std::string& path);

protected:
    void serialize(PHString& str) override;
	void deserialize(std::stringstream& ss) override;
    void set_property(Property* property, void* value) override;

    std::string name;
    PHPath path;
};


