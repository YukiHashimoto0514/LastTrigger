//@file UMove.h

#ifndef COMPONENT_UMOVE_H_INCLUDED

#define COMPONENT_UMOVE_H_INCLUDED

#include "../../Application/Component.h"
#include "../../Application/GameObject.h"
#include <math.h>

//U字に飛んでいく
class UMove :public Component
{
public:
	UMove() = default;			//コンストラクタ
	virtual ~UMove() = default;	//デストラクタ


	virtual void Update(GameObject& gameObject, float deltaTime)override
	{
		movement += deltaTime * 0.5f;		//加算

		//U時に湾曲する
		gameObject.x += movement * vx * deltaTime;		
		gameObject.y += movement * vy * deltaTime;		
	}

	float vx = 0;		//150がおすすめ
	float vy = 0;		//４００がおすすめ
	float movement = 0;	//移動量
};




#endif //COMPONENT_UMOVE_H_INCLUDED
