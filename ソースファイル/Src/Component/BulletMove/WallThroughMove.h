//@file boundMove.h

#ifndef COMPONENT_WALLTHROUGHMOVE_H_INCLUDED

#define COMPONENT_WALLTHROUGHMOVE_H_INCLUDED

#include "../../Application/Component.h"
#include "../../Application/GameObject.h"
#include <math.h>

//�ǂɓ�����Ɣ��Α��̕ǂ���o�Ă���
class WallThroughMove :public Component
{
public:
	WallThroughMove() = default;			//�R���X�g���N�^
	virtual ~WallThroughMove() = default;	//�f�X�g���N�^


	virtual void Update(GameObject& gameObject, float deltaTime)override
	{

		//��ʂ̒[�����ɂ���������A�ђ�
		if (Counter < 2)
		{
			//����
			if (gameObject.x <= 0)
			{
				gameObject.x = 543;
				Counter++;
			}

			//�E��
			if (gameObject.x >= 544)
			{
				gameObject.x = 0;
				Counter++;
			}

			//��
			if (gameObject.y >= 700)
			{
				gameObject.y = 1;
				Counter++;
			}

			//��
			if (gameObject.y <= 0)
			{
				gameObject.y = 699;
				Counter++;

			}
		}

		gameObject.x += vx * deltaTime;	//���Ɉړ�������
		gameObject.y += vy * deltaTime;	//�O�i������

	}

	float vx = 0;//100����������
	float vy = 0;//1000����������

	int Counter = 0;	//���񂷂蔲���������J�E���g
};




#endif //COMPONENT_WALLTHROUGHMOVE_H_INCLUDED
