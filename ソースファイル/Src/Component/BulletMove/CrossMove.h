//@file CrossdMove.h

#ifndef COMPONENT_CROSSMOVE_H_INCLUDED

#define COMPONENT_CROSSMOVE_H_INCLUDED

#include "../../Application/Component.h"
#include "../../Application/GameObject.h"
#include <math.h>

//横に飛び出てある程度離れたら敵を追尾する
class CrossMove :public Component
{
public:
	CrossMove() = default;			//コンストラクタ
	virtual ~CrossMove() = default;	//デストラクタ

	virtual void Update(GameObject& gameObject, float deltaTime)override
	{
		//一度だけ入る
		if (!GoFlg)
		{
			//適当な距離離れていなかったら
			if (abs(gameObject.x - Boss->x) < 160)
			{
				//最初に横に飛ぶ
				angle = 0;
			}
			else//プレイヤーをロックオンして飛ぶ
			{
				//距離を計測
				float disx = target->x - gameObject.x;
				float disy = target->y - gameObject.y;

				//角度を求める
				angle = atan2(disy, disx);
				vx = abs(vx);
				GoFlg = true;
			}
		}
		gameObject.x += vx * cos(angle) * deltaTime;	//前進させる
		gameObject.y += vy * sin(angle) * deltaTime;	//前進させる
	}

	float vx = 0;		//100がおすすめ
	float vy = 0;		//1000がおすすめ
	float angle = 0;	//角度

	bool GoFlg = false;	//一回だけ入る

	GameObjectPtr target;	//標的
	GameObjectPtr Boss;		//ボス
};

#endif //COMPONENT_UMOVE_H_INCLUDED
