#include <iostream>
#include <string>
#include <conio.h>
#include <Windows.h>
#include "Player.h"


// 设置玩家位置
void Player::setPosition(float x, float y) {
	this->m_position.x = x;
	this->m_position.y = y;
}

// 获取玩家位置
Vector2D Player::getPosition() {
	return this->m_position;
}

// 设置玩家速度
void Player::setVelocity(float Vx, float Vy) {
	this->m_position.x = Vx;
	this->m_position.y = Vy;
}

// 获取玩家速度
Vector2D Player::getVelocity() {
	return this->m_velocity;
}

void Player::move(float Vx, float Vy, float deltaTime) {
	// 如果GameLoop中的inputDetection检测到了WASD的按键
	// 进行相应的动作
	// 
	// 被LUA调用,根据原始位置和速度更新新的位置坐标
	Vector2D originPos = this->getPosition();
	Vector2D originVelocity = this->getVelocity();

	this->setPosition(originPos.x + originVelocity.x * deltaTime, originPos.y + originVelocity.y * deltaTime);
}

void Player::updateInfo(float deltatime) {
}