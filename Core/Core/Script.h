#pragma once

#include "Component.h"
#include "Core/Utils/PHPath.h"
#include "IScriptBehaviour.h"


namespace sol
{
	class state;
    template <bool b>
    class basic_reference;
    using reference = basic_reference<false>;
    template <typename T, bool, typename H>
    class basic_protected_function;
    using protected_function = basic_protected_function<reference, false, reference>;
}

class Script : public Component , public IScriptBehaviour
{
public:
    Script(GameObject* gameObj,const std::string& name = "None",const std::string& path = "");
    virtual ~Script();
    void reset() override;
    
    // getters and setters
    std::string get_name();                              //需要绑定            
    void set_name(const std::string& name);//需要绑定 
    std::string get_path();							  //需要绑定
    void set_path(const std::string& path);			  //需要绑定


    // inherited via IScriptBehaviour
    void awake() override;
    void start() override;
    void onCollide(const std::vector<CollisonInfo>&) override;
    void beforeUpdate() override;
    void update() override;
    void afterUpdate() override;

protected:
    void serialize(PHString& str) override;
	void deserialize(std::stringstream& ss) override;

    std::string name;
    PHPath path;

    sol::state* lua = nullptr;
    sol::protected_function* awake_func = nullptr;
    sol::protected_function* start_func = nullptr;
    sol::protected_function* onCollide_func = nullptr;
    sol::protected_function* beforeUpdate_func = nullptr;
    sol::protected_function* update_func = nullptr;
    sol::protected_function* afterUpdate_func = nullptr;
    std::string sol_instance;

    friend class GameLoop;
};


