#include "Core/Core/Script.h"
#include "GameLoop.h"
#include "Core/ResourceManagement/ResourceMgr.h"
#include "Core/ResourceManagement/SceneMgr.h"
#include "Core/Utils/Time.h"
#include "Core/Physics/PhysicsEngine.h"

#include "lib/sol/sol.hpp"



using std::chrono::system_clock;


void GameLoop::updatePlayer(float deltaTime) {
        // 更新玩家状态-比如位置和执行动作
        
        // TODO:Player的输入检测
    
        // TODO:更新Player的速度 
    this->getPlayer()->setVelocity(this->getPlayer()->getVelocity().x, this->getPlayer()->getVelocity().y);

        // TODO:Player的移动
    this->getPlayer()->move(deltaTime);

    }

void GameLoop::updateScene(RenderWidget* aWidget) {
    // 更新场景信息-需要和光夫哥和敬哥对接
    // 获取场景信息(光夫哥)并且进行渲染（敬哥）
    // ...       
        // 游戏循环
    //lua.open_libraries(sol::lib::base);
    // bindAllClasses(*lua);
    //lua.script_file("./input.lua");
    auto starttime = system_clock::now();
    auto endtime = system_clock::now();
    while (isRunning) {
        // 计算每一帧的时间间隔

        // 等待一段时间，控制游戏速度
        endtime = system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endtime - starttime);
        if (duration.count() < 10) {
            continue;//Sleep(1 - elapsed_time);
        }
        starttime = system_clock::now();
        if (isPlaying && aWidget->isGameWidget || !GameEngine::get_instance().getInEditor())
            //updateGame(aWidget,*lua,duration.count());
        aWidget->update();
        endtime = system_clock::now();
        //std::cout << "end_time：" << end_time << "\tstart_time" << start_time << std::endl;
    }
}

float GameLoop::updateRender()
{
    auto start = Time::now();
    RenderWidget::getCurrentWidget().update();
    auto end = Time::now();
    return end - start;
}


void GameLoop::updateGameState(float deltaTime) {
    }

void GameLoop::printDataInfo(float deltaTime) {
        //输出数据信息-测试用
    }

//加载所有游戏对象的所有挂载的脚本
void GameLoop::preloadScriptFiles(sol::state& lua) {
    auto& scripts = SceneMgr::get_instance().script_behaviours;
    for (auto& script : scripts) {
        if (auto s = dynamic_cast<Script*>(script))
        {
            if (!QFile::exists(s->get_path().c_str()))
            {
                Debug::logWarning() << "GameObject : " << s->gameObject->get_name() << " preloading script file : " << s->get_path() << " failed , file not exist . \n";
				continue;
            }
            s->lua = &lua;
            s->lua->script_file(s->get_path());
            auto classProxy = lua[s->get_name()];
            if (classProxy.valid())
            {
                string index = "ptr" + std::to_string((size_t)s);
                
                lua.script(index + "=" + s->get_name() + ".new()");
                auto instancesProxy = lua[index];
                if (instancesProxy.valid())
                {
                    instancesProxy["gameObject"] = s->gameObject;
                    s->sol_instance = index;
                    sol::protected_function awake_func = instancesProxy["awake"];
                    if (awake_func.valid())
                        s->awake_func = new sol::protected_function(awake_func);
                    sol::protected_function start_func = instancesProxy["start"];
                    if (start_func.valid())
                        s->start_func = new sol::protected_function(start_func);
                    sol::protected_function beforeUpdate_func = instancesProxy["beforeUpdate"];
                    if (beforeUpdate_func.valid())
                        s->beforeUpdate_func = new sol::protected_function(beforeUpdate_func);
                    sol::protected_function update_func = instancesProxy["update"];
                    if (update_func.valid())
                        s->update_func = new sol::protected_function(update_func);
                    sol::protected_function afterUpdate_func = instancesProxy["afterUpdate"];
                    if (afterUpdate_func.valid())
                        s->afterUpdate_func = new sol::protected_function(afterUpdate_func);
				}
                else
                {
                    Debug::logError()<< "Can not create instance of " << s->get_name() << " at script : " <<s->get_path()<<"\n";
                }
            }
            else
            {
                Debug::logError() << "Can not find class " << s->get_name() << " at script : " << s->get_path() << " , please make sure you implement it correctly . \n";
            }
        }
	}
}

//更新游戏逻辑（指调用lua脚本的update函数）  东哥，你看这个函数应该放到什么地方？这个要循环调用
void GameLoop::updateScripts(float deltaTime, sol::state &lua) {
    for (GameObject* gameObject : gameObjects) {
        std::vector<Script*> scripts = gameObject->getComponents<Script>();
        for (Script* script : scripts) {
            std::string gameObjectName = gameObject->get_name();
            lua["this"] = gameObject;

        }
    }
}



void GameLoop::updateGame(RenderWidget* gameWidget,sol::state &lua,float deltaTime) {

    auto starttime = system_clock::now();
    auto endtime = system_clock::now();
        // 计算每一帧的时间间隔

        // 等待一段时间，控制游戏速度
        endtime = system_clock::now();

        //不clear的话会导致运行速度越来越慢，会导致东西越来越多
        allScripts.clear();

        // 测试用的duaration.count()
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endtime - starttime);

        for (auto &gameObj : GameEngine::get_instance().getCurrentScene()->getAllGameObjs()) {
            if (auto script = gameObj.second->getComponent<Script>(); script != nullptr && 
                !script->get_path().empty() && script->get_enabled() && 
                gameObj.second->is_active()) {
             gameObjects.push_back(gameObj.second);
            allScripts.push_back(script);
            }
        }
           

        //gameWidget->update();
        //preloadScriptFiles();
        updateScripts(deltaTime, lua);

        endtime = system_clock::now();
        //std::cout << "end_time：" << endtime << "\tstart_time" << starttime << std::endl;
        //std::cout << "1" << std::endl;

    /*
    lua.open_libraries(sol::lib::base);// 打开所需的Lua库void GameLoop::update(float deltaTime, RenderWidget* Rwg) {

    // 预加载脚本文件
    preloadScriptFiles();

    
        // 游戏逻辑更新函数，每帧调用一次
        updatePlayer(deltaTime);
        updateScene(sceneWidget, gameWidget);
        updateGameState(deltaTime);
        printDataInfo(deltaTime);
    */

    }

// 获得这个Player的指针
Player* GameLoop::getPlayer(){ 
   return this->player;
}

// 设置这个游戏循环中的Player
void GameLoop::setPlayer(Player* player) {
   this->player = player;
}

void GameLoop::startGameLoop()
{
    //initialize game scene
    auto gameProject = GameEngine::get_instance().getCurrentGameProject();
    gameProject->openScene(gameProject->get_current_scene_index());

    // initialize physics engine
    auto physicsEngine = PhysicsEngine::get_instance();
    physicsEngine->clear();

    //initialize lua
    if(lua!=nullptr)
		delete lua;
    lua = new sol::state;
    lua->open_libraries(sol::lib::base ,sol::lib::package);
    bindAllClasses(*lua);
    preloadScriptFiles(*lua);

    //call script function
    awake();
    start();

    isRunning = true;
    isClosed = false;
    float deltaTime = 0.f;
    while (isRunning)
    {
        auto start = Time::now();

        //update physics
        physicsEngine->update(Time::deltaTime);
        onCollide(physicsEngine->collisonInfo);
        //update game logic
        beforeUpdate();
        update();
        afterUpdate();

        updateRender();

        

        auto end = Time::now();
        auto frameTime = 1000.f / SceneMgr::get_instance().get_render_setting()->get_target_frame_rate() - deltaTime;
        while (end - start < frameTime)
        {
            end = Time::now();
        }
        Time::deltaTime = end - start;
        deltaTime = Time::deltaTime - frameTime;
    }
    SceneMgr::get_instance().exitScene();
    isClosed = true;
}

void GameLoop::startRenderLoop()
{
    //initialize game scene
    auto gameProject = GameEngine::get_instance().getCurrentGameProject();
    gameProject->openScene(gameProject->get_current_scene_index());

    isRunning = true;
    isClosed = false;
    float deltaTime = 0.f;
    while (isRunning)
    {
        auto start = Time::now();
        auto renderTime = updateRender();
        auto end = Time::now();
        auto frameTime =1000.f/ SceneMgr::get_instance().get_render_setting()->get_target_frame_rate() - deltaTime;
        while(end - start <  frameTime )
        {
            end = Time::now();
        }
        Time::deltaTime = end - start;
        deltaTime = Time::deltaTime - frameTime;
    }
    SceneMgr::get_instance().exitScene();
    isClosed = true;
}

void GameLoop::awake()
{
    for (auto& script : SceneMgr::get_instance().script_behaviours)
    {
        script->awake();
    }
}

void GameLoop::start()
{
    for (auto& script : SceneMgr::get_instance().script_behaviours)
    {
        script->start();
    }
}
void GameLoop::onCollide(const std::vector<CollisonInfo>& collisionInfo)
{
    for (auto& script : SceneMgr::get_instance().script_behaviours)
    {
        script->onCollide(collisionInfo);
    }
}
void GameLoop::beforeUpdate()
{
    for (auto& script : SceneMgr::get_instance().script_behaviours)
    {
        script->beforeUpdate();
    }
}

void GameLoop::update()
{
    for (auto& script : SceneMgr::get_instance().script_behaviours)
    {
        script->update();
    }
}

void GameLoop::afterUpdate()
{
    for (auto& script : SceneMgr::get_instance().script_behaviours)
    {
        script->afterUpdate();
    }
}

void GameLoop::shutdown() {
    this->isRunning = false;
}

void GameLoop::setPlayingStatus(bool flag) {
    this->isPlaying = flag;
}