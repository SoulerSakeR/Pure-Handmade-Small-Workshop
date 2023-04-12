#include <iostream>
#include <string>
#include <conio.h>
#include <Windows.h>
#include "Player.h"


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

// 跟光夫哥和坤佯讨论完后在考虑这个
void Player::updateInfo(float deltatime) {

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


