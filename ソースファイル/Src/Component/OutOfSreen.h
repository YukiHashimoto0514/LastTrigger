//@file OutOfScreen.h

#ifndef COMPONENT_OUTOFSCREEN_H_INCLUDED
#define COMPONENT_OUTOFSCREEN_H_INCLUDED

#include "../Application/Component.h"
#include "../Application/GameObject.h"

//画面外と判定されたオブジェクトを削除する
class OutOfScreen :public Component
{
public:
	OutOfScreen() = default;			//コンストラクタ
	virtual ~OutOfScreen() = default;	//デストラクタ

	virtual void Update(GameObject& gameObject, float deltaTime)override
	{
		const float xMin = -32;
		const float xMax = 18 * 32;
		const float yMin = -64;
		const float yMax = 23 * 32;

		//画面外にいる場合、生死フラグをtrue
		if (gameObject.x <= xMin || gameObject.x >= xMax || gameObject.y <= yMin || gameObject.y >= yMax)
		{
			gameObject.Point = 0;
			gameObject.isDead = true;
			gameObject.Scale = 0;
		}
	}

};


#endif //COMPONENT_OUTOFSCREEN_H_INCLUDED
