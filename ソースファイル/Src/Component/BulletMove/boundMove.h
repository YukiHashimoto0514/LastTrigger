//@file boundMove.h

#ifndef COMPONENT_boundMOVE_H_INCLUDED

#define COMPONENT_boundMOVE_H_INCLUDED

#include "../../Application/Component.h"
#include "../../Application/GameObject.h"
#include <math.h>

//�Q�[���I�u�W�F�N�g��ǂŔ��˂�����
class boundMove :public Component
{
public:
	boundMove() = default;			//�R���X�g���N�^
	virtual ~boundMove() = default;	//�f�X�g���N�^

	virtual void Update(GameObject& gameObject, float deltaTime)override
	{
		gameObject.x += vx * deltaTime;	//���E�����Ƀ����_���ɎU�炷
		gameObject.y += vy * deltaTime;	//�O�i������

		//��ʂ̒[�����ɂ���������A�o�E���h
		if (Counter < 3)
		{
			//����
			if (gameObject.x <= 0)
			{
				vx = vx * (-1);
				Counter++;
			}

			//�E��
			if (gameObject.x >= 544)
			{
				vx = vx * (-1);
				Counter++;
			}

			//����
			if (gameObject.y >= 704)
			{
				vy = vy * (-1);
				Counter++;
			}

			//���
			if (gameObject.y <= 0)
			{
				vy = vy * (-1);
				Counter++;
			}
		}
	}

	float vx = 0;	//�������ɗ^����l
	float vy = 0;	//�c�����ɗ^����l

	int Counter = 0;//�ǂɉ���G�ꂽ��
};




#endif //COMPONENT_UMOVE_H_INCLUDED
