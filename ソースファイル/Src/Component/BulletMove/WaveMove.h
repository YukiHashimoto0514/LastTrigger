//@file WaveMove.h

#ifndef COMPONENT_WAVEMOVE_H_INCLUDED

#define COMPONENT_WAVEMOVE_H_INCLUDED

#include "../../Application/Component.h"
#include "../../Application/GameObject.h"
#include <math.h>

//波を描きながら飛んで行く
class WaveMove :public Component
{
public:
	WaveMove() = default;			//コンストラクタ
	virtual ~WaveMove() = default;	//デストラクタ

	virtual void Update(GameObject& gameObject, float deltaTime)override
	{
		angle += deltaTime * 1000;	//角度を加算

		//弧度法に変換
		float b = angle * radian;

		gameObject.x += 5 * cos(b) * vx * deltaTime;	//波を再現
		gameObject.y += vy * deltaTime;					//前進させる
	}

	float vx = 0;	//200
	float vy = 0;	//700

	float angle = 0;	//角度
	const float radian = 3.14f / 180;	//ラジアンに変換する
};




#endif //COMPONENT_ANGlEMOVE_H_INCLUDED
