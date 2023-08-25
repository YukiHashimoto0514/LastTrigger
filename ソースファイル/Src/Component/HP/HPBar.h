//@file HPBar.h

#ifndef COMPONENT_HPBAR_H_INCLUDED
#define COMPONENT_HPBAR_H_INCLUDED
#include "../../Application/Component.h"
#include "../../Application/GameObject.h"

//HPバーを表示するコンポーネント

class HPBar :public Component
{
public:
	HPBar() = default;			//コンストラクタ
	virtual ~HPBar() = default;	//デストラクタ

	virtual void Start(GameObject& gameObject)override
	{
		GreenBar = gameObject.engine->Create<GameObject>(
			"HPBar", gameObject.x - 200, gameObject.y);
		GreenBar->AddSprite(tc[0]);
		GreenBar->priority = prioEnemy;
	}

	virtual void Update(GameObject& gameObject, float deltaTime)override
	{
		if (GreenBar->Red >= 1)
		{
			GreenBar->Red -= deltaTime * 2;
		}

		//HPを同期させる
		NowHP = gameObject.HP / MaxHP * 300;

		//ゲームオブジェクトについていく+ずらして右側から減るようにしている
		GreenBar->x = gameObject.x - ViewSizeX * 0.5f + (MaxHP * 0.5f + NowHP * 0.5f) - 125;

		GreenBar->y = gameObject.y;

		//描画更新
		tc[0] = MakeTexcoord(0, 220, NowHP, 10);	//緑色
		GreenBar->spriteList.clear();
		GreenBar->AddSprite(tc[0]);
	}

	virtual void OnTakeDamage(GameObject& gameObject, GameObject& other, const Damage& damage)override
	{
		if (gameObject.HP <= 0)
		{
			//死んだときHPバーを消す
			GreenBar->spriteList.clear();
		}
	}

	float MaxHP = 0;		//最大HP
	float NowHP = 0;		//現在のHP
	int HPScale = 30;		//HPバーの拡縮
	const int ViewSizeX = 540;	//画面サイズのX
	const int VIewSIzeY = 704;	//画面サイズのｙ
	
	GameObjectPtr GreenBar;

	Texcoord tc[2] = { MakeTexcoord(0, 220, NowHP/2, 10),	//緑色
		   MakeTexcoord(0, 230, MaxHP, 10)		//赤色
	};

};


#endif // !COMPONENT_HPBAR_H_INCLUDED
