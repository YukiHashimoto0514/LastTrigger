//@file UChaseMove.h

#ifndef COMPONENT_UCHASEMOVE_H_INCLUDED

#define COMPONENT_UCHASEMOVE_H_INCLUDED

#include "../../Application/Component.h"
#include "../../Application/GameObject.h"
#include "../../Application/VecMath.h"
#include <math.h>

//２段階敵を追跡する
class UChaseMove :public Component
{
public:
	UChaseMove() = default;				//コンストラクタ
	virtual ~UChaseMove() = default;	//デストラクタ


	virtual void Update(GameObject& gameObject, float deltaTime)override
	{
		//yの座標の差が５以下だったら
		if (Bound == 0)
		{
			if (abs(target->y - gameObject.y) < 5)
			{
				//敵のほうを向き追跡
				if (target->x > gameObject.x)
				{
					Bound = 1;
					gameObject.rotation[2] = VecMath::radians(90);//z軸回転
				}
				else
				{
					vx *= -1;
					Bound = 1;
					gameObject.rotation[2] = VecMath::radians(270);//z軸回転
				}
			}
		}
		//ｘの座標の差が５以下だったら
		else if (Bound == 1)
		{
			//敵のほうに向き追跡
			if (abs(target->x - gameObject.x) < 5)
			{
				if (target->y > gameObject.y)
				{
					Bound = 2;
					vy *= -1;
					gameObject.rotation[2] = VecMath::radians(180);//z軸回転
				}
				else
				{
					Bound = 2;
					gameObject.rotation[2] = VecMath::radians(0);//z軸回転
				}
			}
		}
		
		//移動処理
		if (gameObject.rotation[2] == VecMath::radians(0))
		{
			gameObject.y += vy * deltaTime;			//前進させる
		}
		else if (gameObject.rotation[2] == VecMath::radians(90) || gameObject.rotation[2] == VecMath::radians(270))
		{
			gameObject.x += vx * deltaTime;			//横に移動
		}
		else if (gameObject.rotation[2] == VecMath::radians(180))
		{
			gameObject.y += vy * deltaTime;			//前進させる
		}
	}

	GameObjectPtr target;	//標的
	float vx = 0;	//200
	float vy = 0;	//700
	int Bound = 0;	//追跡回数
};

#endif //COMPONENT_UCHASEMOVE_H_INCLUDED
