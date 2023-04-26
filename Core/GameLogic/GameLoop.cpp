#include "GameLoop.h"
using std::chrono::system_clock;
std::vector<GameObject*> gameObjects;
std::vector<Script*> allScripts;
//sol::state lua;
std::unordered_map<std::string, sol::protected_function> updateFunctions;

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
    sol::state lua;
    //lua.open_libraries(sol::lib::base);
    bindAllClasses(lua);
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
            updateGame(aWidget,lua,duration.count());
        aWidget->update();
        endtime = system_clock::now();
        //std::cout << "end_time：" << end_time << "\tstart_time" << start_time << std::endl;
    }
    updateFunctions.clear();
}


void GameLoop::updateGameState(float deltaTime) {
    }

void GameLoop::printDataInfo(float deltaTime) {
        //输出数据信息-测试用
    }

//加载所有游戏对象的所有挂载的脚本，并放入容器中
void GameLoop::preloadScriptFiles(sol::state &lua) {
    updateFunctions.clear();
    for (Script* script : allScripts) {
        lua.script_file(script->get_path());
        updateFunctions[script->get_name()] = lua["update"];
    }
}

//更新游戏逻辑（指调用lua脚本的update函数）  东哥，你看这个函数应该放到什么地方？这个要循环调用
void GameLoop::updateScripts(float deltaTime, sol::state &lua) {
    for (GameObject* gameObject : gameObjects) {
        std::vector<Script*> scripts = gameObject->getComponents<Script>();
        for (Script* script : scripts) {
            std::string gameObjectName = gameObject->get_name();
            lua["this"] = gameObject;
            auto result = updateFunctions[script->get_name()](deltaTime);

            //if (result.valid() == false) {
            //    sol::error err = result;
            //    std::cout << "----- RUN LUA ERROR ----" << std::endl;
            //    std::cout << err.what() << std::endl;
            //    std::cout << "------------------------" << std::endl;
            //}

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
        gameObjects.clear();
        allScripts.clear();

        // 测试用的duaration.count()
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endtime - starttime);

        for (auto &gameObj : GameEngine::get_instance().getCurrentScene()->getAllGameObjs()) {
            if (auto script = gameObj.second->getComponent<Script>(); script != nullptr && 
                !script->get_path().empty() && script->get_enabled() && 
                gameObj.second->isActive) {
             gameObjects.push_back(gameObj.second);
            allScripts.push_back(script);
            }
        }
           

        //gameWidget->update();
        preloadScriptFiles(lua);
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

void GameLoop::shutdown() {
    this->isRunning = false;
}

void GameLoop::setPlayingStatus(bool flag) {
    this->isPlaying = flag;
}