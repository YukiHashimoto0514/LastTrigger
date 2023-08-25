//@file Move.h

#ifndef COMPONENT_MOVE_H_INCLUDED
#define COMPONENT_MOVE_H_INCLUDED

#include "../../Application/Component.h"
#include "../../Application/GameObject.h"

//ゲームオブジェクトを移動する
class Move :public Component
{
public:
	Move() = default;//コンストラクタ
	virtual ~Move() = default;//デストラクタ

	virtual void Update(GameObject& gameObject, float deltaTime)override
	{
		gameObject.x += vx * deltaTime;
		gameObject.y += vy * deltaTime;
	}

	float vx = 0;
	float vy = 0;

};




#endif //COMPONENT_MOVE_H_INCLUDED
