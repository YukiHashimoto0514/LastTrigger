//@file MoveAndStop.h

#ifndef COMPONENT_MOVEANDSTOP_H_INCLUDED
#define COMPONENT_MOVEANDSTOP_H_INCLUDED

#include "../../Application/Component.h"
#include "../../Application/GameObject.h"

//動いて止まる
class MoveAndStop :public Component
{
public:
	MoveAndStop() = default;			//コンストラクタ
	virtual ~MoveAndStop() = default;	//デストラクタ

	virtual void Update(GameObject& gameObject, float deltaTime)override
	{
		//止まる時間に合わせて経過時間を計算
		CalcTime += deltaTime / Stop;

		//計算した時間が１秒経過したら
		if (CalcTime > 1)
		{
			Timer += static_cast<int>(CalcTime);
			CalcTime = 0;
		}

		//ここに入っていない場合は止まる
		if (Timer % 2 == 0)
		{
			gameObject.x += vx * deltaTime;
			gameObject.y += vy * deltaTime;
		}
	}

	float vx = 0;
	float vy = 0;
	int Stop = 1;	//止まるまでの時間

private:
	int Timer = 0;	//経過時間
	float CalcTime = 0;	//計算した時間
};



#endif //COMPONENT_MOVEANDSTOP_H_INCLUDED
