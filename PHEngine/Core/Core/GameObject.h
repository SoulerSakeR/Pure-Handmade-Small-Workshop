#pragma once
#include "ISerializable.h"
class GameObject :
    public ISerializable
{
public:
    bool isActive; //��ǰ��Ϸ����ļ���״̬
    string name; //����
    int id; //Ψһid
    Transform transform; //��Ϸ�����transform���
    string serialize();
};

