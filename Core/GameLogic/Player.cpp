#include <iostream>
#include <string>
#include <conio.h>
#include <Windows.h>
#include "Player.h"


// �������ƫ��ֵ
void Player::setMoveValue(Vector2D value) {
	this->gameObject->transform->translate(value);
}

// ��ȡ���λ��
Vector2D Player::getPosition() {
	return this->gameObject->transform->getWorldPosition();
}

// ��������ٶ�
void Player::setVelocity(float Vx, float Vy) {
	this->m_velocity.x = Vx;
	this->m_velocity.y = Vy;
}

// ��ȡ����ٶ�
Vector2D Player::getVelocity() {
	return this->m_velocity;
}

// ��ҽ��в�������ʱֱ�ӵ��ã�ͨ�����õ��ٶ�ֱ���ƶ�����
void Player::move(float deltaTime) {
	// ���GameLoop�е�inputDetection��⵽��WASD�İ���
	// ������Ӧ�Ķ���
	// 
	// TODO:��Lua�������ã�
	Vector2D originVelocity = this->getVelocity();

	//ƫ���� = �ٶ� * ���ʱ��
	this->setMoveValue(Vector2D(originVelocity.x * deltaTime, originVelocity.y * deltaTime));
}

// �������������������ڿ������
void Player::updateInfo(float deltatime) {

}


// ���HP(������ܷŵ�Lua�ű���)
int Player::getHealth() {
	return this->m_health;
}

// ����HP(������ܷŵ�Lua�ű���)
void Player::setHealth(int health) {
	this->m_health = health;
}

// ��÷���(������ܷŵ�Lua�ű���)
int Player::getScore() {
	return this->m_score;
}

// ���÷���(������ܷŵ�Lua�ű���)
void Player::setScore(int score) {
	this->m_score = score;
}


