//@file WaveMove.h

#ifndef COMPONENT_WAVEMOVE_H_INCLUDED

#define COMPONENT_WAVEMOVE_H_INCLUDED

#include "../../Application/Component.h"
#include "../../Application/GameObject.h"
#include <math.h>

//�g��`���Ȃ�����ōs��
class WaveMove :public Component
{
public:
	WaveMove() = default;			//�R���X�g���N�^
	virtual ~WaveMove() = default;	//�f�X�g���N�^

	virtual void Update(GameObject& gameObject, float deltaTime)override
	{
		angle += deltaTime * 1000;	//�p�x�����Z

		//�ʓx�@�ɕϊ�
		float b = angle * radian;

		gameObject.x += 5 * cos(b) * vx * deltaTime;	//�g���Č�
		gameObject.y += vy * deltaTime;					//�O�i������
	}

	float vx = 0;	//200
	float vy = 0;	//700

	float angle = 0;	//�p�x
	const float radian = 3.14f / 180;	//���W�A���ɕϊ�����
};




#endif //COMPONENT_ANGlEMOVE_H_INCLUDED
