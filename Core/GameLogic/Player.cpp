#include <iostream>
#include <string>
#include <conio.h>
#include <Windows.h>
#include "Player.h"

/*
1.���������ƣ����������������Ĵ��룬������Ӧ��������������¼��Ĵ�����Щ������Ҫ����Ϸѭ����ʵ�֡�

2.��ײ��⣺ʵ����������Ϸ�������ײ��⡣��������Ҫ��������Ϸ���󽻻�������������ϰ�����ײ������Ҫ����Ϸѭ����ʵ����Ӧ����ײ����߼���

3.״̬��Ϣ�������˽���ֵ�ͷ�����״̬��Ϣ�����ǻ�û��ʵ�����ǵľ����߼�����������Ҫ����Ϸ���ܵ��˺����ߵõ�����������Ҫ����Ϸѭ����ʵ����Ӧ���߼���

4.������Ϣ��ʵ����������Ϸ����Ľ����߼�����������Ҫ����ˡ���Ʒ��NPC���������繥�����˻��߻����Ʒ������Ҫ����Ϸѭ����ʵ����Ӧ���߼���
*/


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

// ����Player��������Ͷ���
void Player::updatePlayerInfo(float deltaTime) {
	this->move(deltaTime);
}

// ����������
void Player::playerInputDetection(float deltaTime) {

}