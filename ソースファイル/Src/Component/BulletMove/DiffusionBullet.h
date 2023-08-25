//@file DiffusionBullet.h

#ifndef COMPONENT_DIFFUSIONBULLET_H_INCLUDED
#define COMPONENT_DIFFUSIONBULLET_H_INCLUDED
#include "../../Application/Component.h"
#include "../../Engine/SpritePriority.h"
#include "../../Application/GameObject.h"
#include "../../Engine/Engine.h"
#include "../Move/Move.h"
#include "../OutOfSreen.h"
#include "../Damage/DamageSource.h"

//拡散弾コンポーネント
class DiffusionBullet :public Component
{
public:
	DiffusionBullet() = default;				//コンストラクタ
	virtual ~DiffusionBullet() = default;	//デストラクタ

	virtual void Update(GameObject& gameObject, float deltaTime)override
	{
		//発射用タイマーが発射間隔を超えたら、発射変数を初期化
		timerA += deltaTime;//タイマーを加算

		if (timerA >= intervalA)	//タイマーがインターバルを超えたら
		{
			isFiring = true;		//発射フラグtrue
			timerA -= intervalA;	//タイマーを減らす
		}

		//発射モードでなければ、何もしない
		if (!isFiring)
		{
			return;
		}


		//弾を発射
		for (angle=0; angle <= 360;angle+=12)
		{
			auto bullet = gameObject.engine->Create<GameObject>(
				"enemy bullet", gameObject.x + x, gameObject.y + y);

			bullet->AddSprite(tc);						//テクスチャ
			bullet->priority = prioEnemyBullet;			//優先順位
			bullet->AddCollision({ -10,-10,10,10 });	//当たり判定

			const float rad = angle * 3.14f / 180;
			auto move = bullet->AddComponent<Move>();	//移動
			move->vx = speed * cos(rad);
			move->vy = speed * sin(rad);

			bullet->AddComponent<OutOfScreen>();		//画面外

			auto damageSource = bullet->AddComponent<DamageSource>();	//ダメージ
			damageSource->targetName = "player";	//プレイヤーを指定
		}

		isFiring = false;	//発射フラグをfalseにする
	}

	//初期化時に設定する変数
	Texcoord tc = MakeTexcoord(144, 365, 16, 16);	//テクスチャ座標

	float intervalA = 3;	//発射間隔
	float speed = 200;		//弾の速度
	float x = 0;			//発射位置のＸ座標
	float y = 0;			//発射位置のＹ座標

	//実行中に変化する変数
	int angle = 0;			//発射角度
	float timerA = 0;		//発射用タイマー
	bool isFiring = false;	//発射フラグ

};



#endif // !COMPONENT_DIFFUSIONBULLET_H_INCLUDED
