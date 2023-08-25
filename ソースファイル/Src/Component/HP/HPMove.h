//@file HPMove.h

#ifndef COMPONENT_HPMOVE_H_INCLUDED
#define COMPONENT_HPMOVE_H_INCLUDED
#include "../../Application/Component.h"
#include "../../Application/GameObject.h"
#include "../Move/Lerp.h"

//HPバーを動かすコンポーネント

class HPMove :public Component
{
public:
	HPMove() = default;			//コンストラクタ
	virtual ~HPMove() = default;	//デストラクタ

	virtual void Start(GameObject& gameObject)override
	{
		gameObject.y = 50;
		CurrentHP = gameObject.HP;
	}

	virtual void Update(GameObject& gameObject, float deltaTime)override
	{
		if (gameObject.HP != CurrentHP)
		{
			if (Timer <= 1)
			{
				Timer += deltaTime * 5;
				gameObject.y = 50 + Smooth(Timer) * 10;
				CurrentHP -= deltaTime * 5;
				gameObject.Red = 4;
			}
			else
			{
				gameObject.Red = 1;
				Timer = 0;
				CurrentHP = gameObject.HP;
			}
		}
	}

	float CurrentHP = 0;
	float Timer = 0;
};


#endif // !COMPONENT_HPMOVE_H_INCLUDED
