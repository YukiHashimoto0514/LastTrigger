//@file Health.h

#ifndef COMPONENT_HEALTH_H_INCLUDED
#define COMPONENT_HEALTH_H_INCLUDED
#include "../../Application/Component.h"
#include "../../Application/GameObject.h"

//耐久値コンポーネント

class Health :public Component
{
public: 
	Health() = default;				//コンストラクタ
	virtual ~Health() = default;	//デストラクタ

	virtual void OnTakeDamage(GameObject& gameObject, GameObject& other, const Damage& damage)override
	{
		gameObject.HP -= damage.amount;

		//HPが０以下になったら死
		if (gameObject.HP <= 0)
		{
			gameObject.isDead = true;
		}

		//HPが半分より小さくなったら
		if (MaxHealth / 2 >= gameObject.HP)
		{
			gameObject.PinchFlg = true;
		}
	}
	float health = 3;	//耐久値
	float MaxHealth = 5;//最大耐久値
};


#endif  COMPONENT_HEALTH_H_INCLUDED