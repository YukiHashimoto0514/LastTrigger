//@file CrossdMove.h

#ifndef COMPONENT_CROSSMOVE_H_INCLUDED

#define COMPONENT_CROSSMOVE_H_INCLUDED

#include "../../Application/Component.h"
#include "../../Application/GameObject.h"
#include <math.h>

//���ɔ�яo�Ă�����x���ꂽ��G��ǔ�����
class CrossMove :public Component
{
public:
	CrossMove() = default;			//�R���X�g���N�^
	virtual ~CrossMove() = default;	//�f�X�g���N�^

	virtual void Update(GameObject& gameObject, float deltaTime)override
	{
		//��x��������
		if (!GoFlg)
		{
			//�K���ȋ�������Ă��Ȃ�������
			if (abs(gameObject.x - Boss->x) < 160)
			{
				//�ŏ��ɉ��ɔ��
				angle = 0;
			}
			else//�v���C���[�����b�N�I�����Ĕ��
			{
				//�������v��
				float disx = target->x - gameObject.x;
				float disy = target->y - gameObject.y;

				//�p�x�����߂�
				angle = atan2(disy, disx);
				vx = abs(vx);
				GoFlg = true;
			}
		}
		gameObject.x += vx * cos(angle) * deltaTime;	//�O�i������
		gameObject.y += vy * sin(angle) * deltaTime;	//�O�i������
	}

	float vx = 0;		//100����������
	float vy = 0;		//1000����������
	float angle = 0;	//�p�x

	bool GoFlg = false;	//��񂾂�����

	GameObjectPtr target;	//�W�I
	GameObjectPtr Boss;		//�{�X
};

#endif //COMPONENT_UMOVE_H_INCLUDED
