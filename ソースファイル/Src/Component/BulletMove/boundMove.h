//@file boundMove.h

#ifndef COMPONENT_boundMOVE_H_INCLUDED

#define COMPONENT_boundMOVE_H_INCLUDED

#include "../../Application/Component.h"
#include "../../Application/GameObject.h"
#include <math.h>

//ゲームオブジェクトを壁で反射させる
class boundMove :public Component
{
public:
	boundMove() = default;			//コンストラクタ
	virtual ~boundMove() = default;	//デストラクタ

	virtual void Update(GameObject& gameObject, float deltaTime)override
	{
		gameObject.x += vx * deltaTime;	//左右方向にランダムに散らす
		gameObject.y += vy * deltaTime;	//前進させる

		//画面の端っこにあたったら、バウンド
		if (Counter < 3)
		{
			//左壁
			if (gameObject.x <= 0)
			{
				vx = vx * (-1);
				Counter++;
			}

			//右壁
			if (gameObject.x >= 544)
			{
				vx = vx * (-1);
				Counter++;
			}

			//下壁
			if (gameObject.y >= 704)
			{
				vy = vy * (-1);
				Counter++;
			}

			//上壁
			if (gameObject.y <= 0)
			{
				vy = vy * (-1);
				Counter++;
			}
		}
	}

	float vx = 0;	//横方向に与える値
	float vy = 0;	//縦方向に与える値

	int Counter = 0;//壁に何回触れたか
};




#endif //COMPONENT_UMOVE_H_INCLUDED
