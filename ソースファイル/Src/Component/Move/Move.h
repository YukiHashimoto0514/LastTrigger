//@file Move.h

#ifndef COMPONENT_MOVE_H_INCLUDED
#define COMPONENT_MOVE_H_INCLUDED

#include "../../Application/Component.h"
#include "../../Application/GameObject.h"

//�Q�[���I�u�W�F�N�g���ړ�����
class Move :public Component
{
public:
	Move() = default;//�R���X�g���N�^
	virtual ~Move() = default;//�f�X�g���N�^

	virtual void Update(GameObject& gameObject, float deltaTime)override
	{
		gameObject.x += vx * deltaTime;
		gameObject.y += vy * deltaTime;
	}

	float vx = 0;
	float vy = 0;

};




#endif //COMPONENT_MOVE_H_INCLUDED
