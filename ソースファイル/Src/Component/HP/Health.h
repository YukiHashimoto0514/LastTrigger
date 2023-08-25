//@file Health.h

#ifndef COMPONENT_HEALTH_H_INCLUDED
#define COMPONENT_HEALTH_H_INCLUDED
#include "../../Application/Component.h"
#include "../../Application/GameObject.h"

//�ϋv�l�R���|�[�l���g

class Health :public Component
{
public: 
	Health() = default;				//�R���X�g���N�^
	virtual ~Health() = default;	//�f�X�g���N�^

	virtual void OnTakeDamage(GameObject& gameObject, GameObject& other, const Damage& damage)override
	{
		gameObject.HP -= damage.amount;

		//HP���O�ȉ��ɂȂ����玀
		if (gameObject.HP <= 0)
		{
			gameObject.isDead = true;
		}

		//HP��������菬�����Ȃ�����
		if (MaxHealth / 2 >= gameObject.HP)
		{
			gameObject.PinchFlg = true;
		}
	}
	float health = 3;	//�ϋv�l
	float MaxHealth = 5;//�ő�ϋv�l
};


#endif  COMPONENT_HEALTH_H_INCLUDED