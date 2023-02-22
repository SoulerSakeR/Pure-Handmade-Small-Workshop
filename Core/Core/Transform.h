#pragma once
#include "Component.h"
#include <vector>
#include "Vector2D.h"
class Transform : public Component
{
public:
    Vector2D localPosition = Vector2D::zero(); //��������λ��
    float localRotation = 0.0f; //������ת�Ƕ�
    vector<Transform*> children ; //�ӽڵ�
    Transform* parent = NULL; //���ڵ�
    string serialize();
    Transform();
    ~Transform();
    Vector2D getWorldPosition(); //��ȡ��������λ��
    float getWorldRotation(); //��ȡ������ת�Ƕ�
    Transform* translate(Vector2D value); //�����������ƶ�
};

