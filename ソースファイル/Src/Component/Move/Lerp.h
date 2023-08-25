//@file Lerp.h

#ifndef COMPONENT_LERP_H_INCLUDED

#define COMPONENT_LERP_H_INCLUDED

#include "../../Application/Component.h"
#include "../../Application/GameObject.h"
#include <math.h>

//�Q�[���I�u�W�F�N�g���ړ�����
class Lerp :public Component
{
public:
	Lerp() = default;			//�R���X�g���N�^
	virtual ~Lerp() = default;	//�f�X�g���N�^

	virtual void Start(GameObject& gameObject)override
	{
		BeforeX = gameObject.x;	//�ŏ��̈ʒu����
		BeforeY = gameObject.y;	//�ŏ��̈ʒu����
		move = 0;				//�ړ��ʂ�������
		TakeTime = 0;			//�����鎞�Ԃ�������
	}

	virtual void Update(GameObject& gameObject, float deltaTime)override
	{
		if (TakeTime < 1) 
		{
			//�ő厞�Ԃɂ���ĉ��Z����l���ω�����
			TakeTime += deltaTime / MaxTime;
		}
		else
		{
			TakeTime = 1;
		}

		//�ړ��v�Z
		MoveCalc(SetCalc);

		//�Q�[���I�u�W�F�N�g�̈ړ�
		gameObject.x = BeforeX + move * (TargetX-BeforeX);
		gameObject.y = BeforeY + move * (TargetY-BeforeY);

	}


	//�ړ��̌v�Z�����߂�֐�
	void  MoveCalc(int _num)
	{
		if (_num == 0)	//���ʂ�߂���
		{
			move = 1 + 2.7f * static_cast<float>(pow(TakeTime - 1, 3)) + 1.7f * static_cast<float>(pow(TakeTime - 1, 2));
		}
		else if (_num == 1)	//�ŏ��̈ʒu�ɖ߂��Ă���
		{
			if (TakeTime < 0.5f)
			{
				move = static_cast<float>(pow(TakeTime, 2)) + 0.5f;
			}
			else if (TakeTime >= 0.5 && TakeTime < 1)
			{
				move = -static_cast<float>(pow(TakeTime, 2)) + 1;
			}
		}
		else if (_num == 2)	//�߂�����������Ă�
		{
			move = static_cast<float>(pow(TakeTime, 5));
		}
	}


	float BeforeX, BeforeY;	//�v�Z���n�߂�O�̍��W
	float move;		        //���s���ɉ��Z����l
	float TargetX = 400;	//�ڕW�n�_
	float TargetY = 500;	//�ڕW�n�_

	float MaxTime = 1;		//�ړ��ɂ����鎞��
	float TakeTime = 0;		//���s���ɂ������鎞��
	int SetCalc = 0;

};

//�p�΂���0~1~0
inline float Scaling(float deltaTime,float val)
{
	if (deltaTime <= 0.5)
	{
		val = static_cast<float>(pow(deltaTime*2, 2));
	}
	else if (deltaTime > 0.5 && deltaTime < 1)
	{
		val = 1 - static_cast<float>(pow(deltaTime * 2 - 1, 2));
	}
	return val;
}

//���炩��0~1~0
inline float Smooth(float deltaTime)
{
	return  -(static_cast<float>(cos(3.14f * deltaTime * 2)) - 1) / 2;
}

//��ԑ���
inline float Drop(float deltaTime)
{
	return static_cast<float>(pow(2, 10 * deltaTime - 10));
}
//�Ō�ɒ��˂�i�ʂ�z���j
inline float Bound(float deltaTime)
{
	return 1 + 2.7f * static_cast<float>(pow(deltaTime - 1, 3)) + 1.7f * static_cast<float>(pow(deltaTime - 1, 2));
}

//�ŏ��ɒ��˂�i�ʂ�z���j
inline float Elastic(float deltaTime)
{
	return static_cast<float>(pow(2, -10 * deltaTime)) * static_cast<float>(sin((deltaTime * 10 - 0.75f)) * (2 * 3.14f) / 3) + 1;
}

//�^�񒆂�����������
inline float Circle(float deltaTime)
{
	if (deltaTime < 0.5)
	{
		return (1 - static_cast<float>(sqrt(1 - pow(2 * deltaTime, 2)))) / 2;
	}
	else
	{
		return static_cast<float>((sqrt(1 - pow(-2 * deltaTime + 2, 2))) + 1) / 2;
	}
}

//�ڂ��ڂ��
inline float Soft(float deltaTime)
{
	if (deltaTime < 0.5)
	{
		return -(static_cast<float>(pow(2, 20 * deltaTime - 10)) *
			static_cast<float>(sin((20 * deltaTime - 11.125f)) * 2 * 3.14f / 4.5f)) / 2;
	}
	else
	{
		return (static_cast<float>(pow(2, -20 * deltaTime + 10)) 
			* static_cast<float>(sin((20.0f * deltaTime - 11.125f)) * 2 * 3.14f / 4.5f)) / 2 + 1;
	}

}

#endif //COMPONENT_ANGlEMOVE_H_INCLUDED
