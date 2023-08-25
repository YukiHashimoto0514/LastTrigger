//@file MoveAndStop.h

#ifndef COMPONENT_MOVEANDSTOP_H_INCLUDED
#define COMPONENT_MOVEANDSTOP_H_INCLUDED

#include "../../Application/Component.h"
#include "../../Application/GameObject.h"

//�����Ď~�܂�
class MoveAndStop :public Component
{
public:
	MoveAndStop() = default;			//�R���X�g���N�^
	virtual ~MoveAndStop() = default;	//�f�X�g���N�^

	virtual void Update(GameObject& gameObject, float deltaTime)override
	{
		//�~�܂鎞�Ԃɍ��킹�Čo�ߎ��Ԃ��v�Z
		CalcTime += deltaTime / Stop;

		//�v�Z�������Ԃ��P�b�o�߂�����
		if (CalcTime > 1)
		{
			Timer += static_cast<int>(CalcTime);
			CalcTime = 0;
		}

		//�����ɓ����Ă��Ȃ��ꍇ�͎~�܂�
		if (Timer % 2 == 0)
		{
			gameObject.x += vx * deltaTime;
			gameObject.y += vy * deltaTime;
		}
	}

	float vx = 0;
	float vy = 0;
	int Stop = 1;	//�~�܂�܂ł̎���

private:
	int Timer = 0;	//�o�ߎ���
	float CalcTime = 0;	//�v�Z��������
};



#endif //COMPONENT_MOVEANDSTOP_H_INCLUDED
