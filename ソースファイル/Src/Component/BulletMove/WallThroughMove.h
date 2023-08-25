//@file boundMove.h

#ifndef COMPONENT_WALLTHROUGHMOVE_H_INCLUDED

#define COMPONENT_WALLTHROUGHMOVE_H_INCLUDED

#include "../../Application/Component.h"
#include "../../Application/GameObject.h"
#include <math.h>

//壁に当たると反対側の壁から出てくる
class WallThroughMove :public Component
{
public:
	WallThroughMove() = default;			//コンストラクタ
	virtual ~WallThroughMove() = default;	//デストラクタ


	virtual void Update(GameObject& gameObject, float deltaTime)override
	{

		//画面の端っこにあたったら、貫通
		if (Counter < 2)
		{
			//左壁
			if (gameObject.x <= 0)
			{
				gameObject.x = 543;
				Counter++;
			}

			//右壁
			if (gameObject.x >= 544)
			{
				gameObject.x = 0;
				Counter++;
			}

			//下
			if (gameObject.y >= 700)
			{
				gameObject.y = 1;
				Counter++;
			}

			//上
			if (gameObject.y <= 0)
			{
				gameObject.y = 699;
				Counter++;

			}
		}

		gameObject.x += vx * deltaTime;	//横に移動させる
		gameObject.y += vy * deltaTime;	//前進させる

	}

	float vx = 0;//100がおすすめ
	float vy = 0;//1000がおすすめ

	int Counter = 0;	//何回すり抜けたかをカウント
};




#endif //COMPONENT_WALLTHROUGHMOVE_H_INCLUDED
