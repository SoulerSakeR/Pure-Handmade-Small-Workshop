#include <iostream>
#include <string>
#include <conio.h>
#include <Windows.h>
#include "Player.h"


// �������λ��
void Player::setPosition(float x, float y) {
	this->m_position.x = x;
	this->m_position.y = y;
}

// ��ȡ���λ��
Vector2D Player::getPosition() {
	return this->m_position;
}

// ��������ٶ�
void Player::setVelocity(float Vx, float Vy) {
	this->m_position.x = Vx;
	this->m_position.y = Vy;
}

// ��ȡ����ٶ�
Vector2D Player::getVelocity() {
	return this->m_velocity;
}

void Player::move(float Vx, float Vy, float deltaTime) {
	// ���GameLoop�е�inputDetection��⵽��WASD�İ���
	// ������Ӧ�Ķ���
	// 
	// ��LUA����,����ԭʼλ�ú��ٶȸ����µ�λ������
	Vector2D originPos = this->getPosition();
	Vector2D originVelocity = this->getVelocity();

	this->setPosition(originPos.x + originVelocity.x * deltaTime, originPos.y + originVelocity.y * deltaTime);
}

void Player::updateInfo(float deltatime) {
}