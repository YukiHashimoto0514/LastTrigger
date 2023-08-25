//@file CircleLerp.h

#ifndef COMPONENT_CIRCLELERP_H_INCLUDED
#define COMPONENT_CIRCLELERP_H_INCLUDED

#include "../../Application/Component.h"
#include "../../Application/GameObject.h"

//�~��`���Ȃ���^�[�Q�b�g��ǂ�������
class CircleLerp :public Component
{
public:
	CircleLerp() = default;				//�R���X�g���N�^
	virtual ~CircleLerp() = default;	//�f�X�g���N�^


	virtual void Update(GameObject& gameObject, float deltaTime)override
	{
		//��x����
		if (!isOnce)
		{
			//�^�[�Q�b�g�����݂���Ȃ�A�l�����炤
			if (target)
			{
				targetX = target->x;
				targetY = target->y;
			}
			else
			{
				targetX = 270;
				targetY = 0;
			}

			//���g��xy���擾
			MyX = gameObject.x;
			MyY = gameObject.y;

			//�^�[�Q�b�g�ƁA�����Ƃ̂Ȃ��p�����߂�
			angle = atan2(MyY - targetY, MyX - targetX);
			Around = angle + 6.2f;
			isOnce = true;
		}
		//�^�[�Q�b�g�Ƃ̋��������߂�
			//�O����
			vx = MyX - targetX;
			vy = MyY - targetY;

			//���a�����߂�
			r  = sqrt(vx * vx + vy * vy);//���a
			r *= 0.5;

		//�p�x�𑝂₷
			angle += deltaTime * 3;

		//�e�̍��W���X�V����
		if (angle > Around)//������Ă�����
		{
			gameObject.isDead = true;
		}
		else
		{
			//�v���C���[�ƓG�̉~������ړ�������
			gameObject.x =(MyX - vx * 0.5f + (r * cos(angle)));
			gameObject.y =(MyY - vy * 0.5f + (r * sin(angle)));
		}
	}


	float vx = 0;		//�^�[�Q�b�g�Ƃ̂������̋���
	float vy = 0;		//�^�[�Q�b�g�Ƃ̂������̋���

	float angle = 0;	//�p	�x
	float Around = 0;	//��������������𑪂�
	float r = 0;		//���a
	float targetX = 0;	//�e�����Ƃ��̃^�[�Q�b�g��X���W
	float targetY = 0;	//�e�����Ƃ��̃^�[�Q�b�g��y���W

	float MyX = 0;		//������x���W
	float MyY = 0;		//������y���W

	bool isOnce = false;	//��񂾂��Ă΂��
	GameObjectPtr target;	//�W�I

};




#endif //COMPONENT_CIRCLELERP_H_INCLUDED
