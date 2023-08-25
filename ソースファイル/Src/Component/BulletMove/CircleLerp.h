//@file CircleLerp.h

#ifndef COMPONENT_CIRCLELERP_H_INCLUDED
#define COMPONENT_CIRCLELERP_H_INCLUDED

#include "../../Application/Component.h"
#include "../../Application/GameObject.h"

//円を描きながらターゲットを追いかける
class CircleLerp :public Component
{
public:
	CircleLerp() = default;				//コンストラクタ
	virtual ~CircleLerp() = default;	//デストラクタ


	virtual void Update(GameObject& gameObject, float deltaTime)override
	{
		//一度だけ
		if (!isOnce)
		{
			//ターゲットが存在するなら、値をもらう
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

			//自身のxyを取得
			MyX = gameObject.x;
			MyY = gameObject.y;

			//ターゲットと、自分とのなす角を求める
			angle = atan2(MyY - targetY, MyX - targetX);
			Around = angle + 6.2f;
			isOnce = true;
		}
		//ターゲットとの距離を求める
			//三平方
			vx = MyX - targetX;
			vy = MyY - targetY;

			//半径を求める
			r  = sqrt(vx * vx + vy * vy);//直径
			r *= 0.5;

		//角度を増やす
			angle += deltaTime * 3;

		//弾の座標を更新する
		if (angle > Around)//一周していたら
		{
			gameObject.isDead = true;
		}
		else
		{
			//プレイヤーと敵の円周上を移動させる
			gameObject.x =(MyX - vx * 0.5f + (r * cos(angle)));
			gameObject.y =(MyY - vy * 0.5f + (r * sin(angle)));
		}
	}


	float vx = 0;		//ターゲットとのｘ方向の距離
	float vy = 0;		//ターゲットとのｙ方向の距離

	float angle = 0;	//角	度
	float Around = 0;	//一周したか動かを測る
	float r = 0;		//半径
	float targetX = 0;	//弾を撃つときのターゲットのX座標
	float targetY = 0;	//弾を撃つときのターゲットのy座標

	float MyX = 0;		//自分のx座標
	float MyY = 0;		//自分のy座標

	bool isOnce = false;	//一回だけ呼ばれる
	GameObjectPtr target;	//標的

};




#endif //COMPONENT_CIRCLELERP_H_INCLUDED
