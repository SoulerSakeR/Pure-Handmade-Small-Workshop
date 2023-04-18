#include "GameLoop.h"
using std::chrono::system_clock;


void GameLoop::updatePlayer(float deltaTime) {
        // 更新玩家状态-比如位置和执行动作
        
        // TODO:Player的输入检测
    
        // TODO:更新Player的速度 
    this->getPlayer()->setVelocity(this->getPlayer()->getVelocity().x, this->getPlayer()->getVelocity().y);

        // TODO:Player的移动
    this->getPlayer()->move(deltaTime);

    }

void GameLoop::updateScene(RenderWidget* Rwg) {
        // 更新场景信息-需要和光夫哥和敬哥对接
        // 获取场景信息(光夫哥)并且进行渲染（敬哥）
        // ...       
            // 游戏循环
        DWORD start_time = GetTickCount();
        DWORD end_time = GetTickCount();
        auto starttime= system_clock::now();
        auto endtime = system_clock::now();
        while (true) {
            // 计算每一帧的时间间隔
            
            // 等待一段时间，控制游戏速度
            endtime = system_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endtime - starttime);
            if (duration.count() < 10) {
                continue;//Sleep(1 - elapsed_time);
            }
            starttime = system_clock::now();
            Rwg->update();
            endtime = system_clock::now();
            //std::cout << "end_time：" << end_time << "\tstart_time" << start_time << std::endl;
        }

    }


void GameLoop::updateGameState(float deltaTime) {
    }

void GameLoop::printDataInfo(float deltaTime) {
        //输出数据信息-测试用
    }

//加载所有游戏对象的所有挂载的脚本，并放入容器中
void preloadScriptFiles() {
    for (Script* script : allScripts) {
        lua.script_file(script->get_path);
        updateFunctions[script->get_name] = lua["update"];
    }
}

//更新游戏逻辑（指调用lua脚本的update函数）  东哥，你看这个函数应该放到什么地方？这个要循环调用
void updateScripts(float deltaTime) {
    for (GameObject* gameObject : gameObjects) {
        std::vector<Script*> scripts = gameObject->getComponents<Script>();
        for (Script* script : scripts) {
            std::string gameObjectName = gameObject->getName();
            lua[gameObjectName] = gameObject;
            updateFunctions[script->get_name](deltaTime);
        }
    }
}

void GameLoop::update(float deltaTime, RenderWidget* Rwg) {

    sol::state lua;
    lua.open_libraries(sol::lib::base);// 打开所需的Lua库void GameLoop::update(float deltaTime, RenderWidget* Rwg) {
    std::unordered_map<std::string, sol::protected_function> updateFunctions;

    // 预加载脚本文件
    preloadScriptFiles();

    // 游戏逻辑更新函数，每帧调用一次
        updatePlayer(deltaTime);
        updateScene(Rwg);
        updateGameState(deltaTime);
        printDataInfo(deltaTime);
    }
        // 游戏逻辑更新函数，每帧调用一次
        updatePlayer(deltaTime);
        updateScene(Rwg);
        updateGameState(deltaTime);
        printDataInfo(deltaTime);
    }

// 获得这个Player的指针
Player* GameLoop::getPlayer(){ 
   return this->player;
}

// 设置这个游戏循环中的Player
void GameLoop::setPlayer(Player* player) {
   this->player = player;
}