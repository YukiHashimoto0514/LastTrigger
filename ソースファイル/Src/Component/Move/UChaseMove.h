//@file UChaseMove.h

#ifndef COMPONENT_UCHASEMOVE_H_INCLUDED

#define COMPONENT_UCHASEMOVE_H_INCLUDED

#include "../../Application/Component.h"
#include "../../Application/GameObject.h"
#include "../../Application/VecMath.h"
#include <math.h>

//�Q�i�K�G��ǐՂ���
class UChaseMove :public Component
{
public:
	UChaseMove() = default;				//�R���X�g���N�^
	virtual ~UChaseMove() = default;	//�f�X�g���N�^


	virtual void Update(GameObject& gameObject, float deltaTime)override
	{
		//y�̍��W�̍����T�ȉ���������
		if (Bound == 0)
		{
			if (abs(target->y - gameObject.y) < 5)
			{
				//�G�̂ق��������ǐ�
				if (target->x > gameObject.x)
				{
					Bound = 1;
					gameObject.rotation[2] = VecMath::radians(90);//z����]
				}
				else
				{
					vx *= -1;
					Bound = 1;
					gameObject.rotation[2] = VecMath::radians(270);//z����]
				}
			}
		}
		//���̍��W�̍����T�ȉ���������
		else if (Bound == 1)
		{
			//�G�̂ق��Ɍ����ǐ�
			if (abs(target->x - gameObject.x) < 5)
			{
				if (target->y > gameObject.y)
				{
					Bound = 2;
					vy *= -1;
					gameObject.rotation[2] = VecMath::radians(180);//z����]
				}
				else
				{
					Bound = 2;
					gameObject.rotation[2] = VecMath::radians(0);//z����]
				}
			}
		}
		
		//�ړ�����
		if (gameObject.rotation[2] == VecMath::radians(0))
		{
			gameObject.y += vy * deltaTime;			//�O�i������
		}
		else if (gameObject.rotation[2] == VecMath::radians(90) || gameObject.rotation[2] == VecMath::radians(270))
		{
			gameObject.x += vx * deltaTime;			//���Ɉړ�
		}
		else if (gameObject.rotation[2] == VecMath::radians(180))
		{
			gameObject.y += vy * deltaTime;			//�O�i������
		}
	}

	GameObjectPtr target;	//�W�I
	float vx = 0;	//200
	float vy = 0;	//700
	int Bound = 0;	//�ǐՉ�
};

#endif //COMPONENT_UCHASEMOVE_H_INCLUDED
