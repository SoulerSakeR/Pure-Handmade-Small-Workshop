#include <iostream>
#include <string>
#include <conio.h>
#include <Windows.h>
#include "Player.h"

/*
1.玩家输入控制：包含处理玩家输入的代码，比如响应按键、鼠标点击等事件的处理。这些代码需要在游戏循环中实现。

2.碰撞检测：实现与其他游戏对象的碰撞检测。如果玩家需要与其他游戏对象交互，例如避免与障碍物碰撞，就需要在游戏循环中实现相应的碰撞检测逻辑。

3.状态信息：定义了健康值和分数等状态信息，但是还没有实现它们的具体逻辑。如果玩家需要在游戏中受到伤害或者得到分数，就需要在游戏循环中实现相应的逻辑。

4.交互信息：实现与其他游戏对象的交互逻辑。如果玩家需要与敌人、物品或NPC交互，例如攻击敌人或者获得物品，就需要在游戏循环中实现相应的逻辑。
*/


// 设置玩家偏移值
void Player::setMoveValue(Vector2D value) {
	this->gameObject->transform->translate(value);
}

// 获取玩家位置
Vector2D Player::getPosition() {
	return this->gameObject->transform->getWorldPosition();
}

// 设置玩家速度
void Player::setVelocity(float Vx, float Vy) {
	this->m_velocity.x = Vx;
	this->m_velocity.y = Vy;
}

// 获取玩家速度
Vector2D Player::getVelocity() {
	return this->m_velocity;
}

// 玩家进行操作输入时直接调用，通过设置的速度直接移动人物
void Player::move(float deltaTime) {
	// 如果GameLoop中的inputDetection检测到了WASD的按键
	// 进行相应的动作
	// 
	// TODO:被Lua函数调用？
	Vector2D originVelocity = this->getVelocity();

	//偏移量 = 速度 * 间隔时间
	this->setMoveValue(Vector2D(originVelocity.x * deltaTime, originVelocity.y * deltaTime));
}




// 获得HP(后面可能放到Lua脚本中)
int Player::getHealth() {
	return this->m_health;
}

// 设置HP(后面可能放到Lua脚本中)
void Player::setHealth(int health) {
	this->m_health = health;
}

// 获得分数(后面可能放到Lua脚本中)
int Player::getScore() {
	return this->m_score;
}

// 设置分数(后面可能放到Lua脚本中)
void Player::setScore(int score) {
	this->m_score = score;
}

// 更新Player调用这个就对了
void Player::updatePlayerInfo(float deltaTime) {
	this->move(deltaTime);
}

// 检测玩家输入
void Player::playerInputDetection(float deltaTime) {

}