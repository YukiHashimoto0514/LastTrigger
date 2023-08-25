//@file UMove.h

#ifndef COMPONENT_UMOVE_H_INCLUDED

#define COMPONENT_UMOVE_H_INCLUDED

#include "../../Application/Component.h"
#include "../../Application/GameObject.h"
#include <math.h>

//U���ɔ��ł���
class UMove :public Component
{
public:
	UMove() = default;			//�R���X�g���N�^
	virtual ~UMove() = default;	//�f�X�g���N�^


	virtual void Update(GameObject& gameObject, float deltaTime)override
	{
		movement += deltaTime * 0.5f;		//���Z

		//U���ɘp�Ȃ���
		gameObject.x += movement * vx * deltaTime;		
		gameObject.y += movement * vy * deltaTime;		
	}

	float vx = 0;		//150����������
	float vy = 0;		//�S�O�O����������
	float movement = 0;	//�ړ���
};




#endif //COMPONENT_UMOVE_H_INCLUDED
