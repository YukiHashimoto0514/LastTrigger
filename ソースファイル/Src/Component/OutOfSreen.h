//@file OutOfScreen.h

#ifndef COMPONENT_OUTOFSCREEN_H_INCLUDED
#define COMPONENT_OUTOFSCREEN_H_INCLUDED

#include "../Application/Component.h"
#include "../Application/GameObject.h"

//��ʊO�Ɣ��肳�ꂽ�I�u�W�F�N�g���폜����
class OutOfScreen :public Component
{
public:
	OutOfScreen() = default;			//�R���X�g���N�^
	virtual ~OutOfScreen() = default;	//�f�X�g���N�^

	virtual void Update(GameObject& gameObject, float deltaTime)override
	{
		const float xMin = -32;
		const float xMax = 18 * 32;
		const float yMin = -64;
		const float yMax = 23 * 32;

		//��ʊO�ɂ���ꍇ�A�����t���O��true
		if (gameObject.x <= xMin || gameObject.x >= xMax || gameObject.y <= yMin || gameObject.y >= yMax)
		{
			gameObject.Point = 0;
			gameObject.isDead = true;
			gameObject.Scale = 0;
		}
	}

};


#endif //COMPONENT_OUTOFSCREEN_H_INCLUDED
