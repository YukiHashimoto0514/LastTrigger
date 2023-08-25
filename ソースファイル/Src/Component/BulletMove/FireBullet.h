//@file FireBullet.h

#ifndef COMPONENT_FIREBULLET_H_INCLUDED
#define COMPONENT_FIREBULLET_H_INCLUDED
#include "../../Application/Component.h"
#include "../../Engine/SpritePriority.h"
#include "../../Application/GameObject.h"
#include "../../Engine/Engine.h"
#include "../Move/Move.h"
#include "../OutOfSreen.h"
#include "../Damage/DamageSource.h"

//敵弾発射コンポーネント

class FireBullet :public Component
{
public:
	FireBullet() = default;				//コンストラクタ
	virtual ~FireBullet() = default;	//デストラクタ

	virtual void Update(GameObject& gameObject, float deltaTime)override
	{
		//発射用タイマーが発射間隔を超えたら、発射変数を初期化
		timerA += deltaTime;//タイマーを加算

		if (timerA >= intervalA)	//タイマーがインターバルを超えたら
		{
			isFiring = true;		//発射フラグtrue
			timerA -= intervalA;	//タイマーを減らす
			timerB = intervalB;		//一発目が即座に発射される
			fireCounter = numberOfFire;

			//ターゲット指定があれば、発射ベクトルをターゲットにむける
			if (target)
			{
				//三平方
				vx = target->x - gameObject.x;
				vy = target->y - gameObject.y;
				const float v = sqrt(vx * vx + vy * vy);
				vx = vx / v * speed;
				vy = vy / v * speed;
			}
			else//ターゲット指定がなければ、真下に打つ
			{
				vx = 0;
				vy = -speed;
			}

		}
			//発射モードでなければ、何もしない
		if (!isFiring)
		{
			return;
		}

		//連射用タイマーが連射間隔を超えたら、弾を発射
		timerB += deltaTime;
		if (timerB < intervalB)
		{
			return;
		}
		timerB -= intervalB;
		//連射中の弾の方向をかえたいならこここ

		
		//弾を発射
		auto bullet = gameObject.engine->Create<GameObject>(
			"enemy bullet", gameObject.x + x, gameObject.y + y);

		bullet->AddSprite(tc);					//テクスチャ
		bullet->priority = prioEnemyBullet;		//優先順位
		bullet->AddCollision({ -10,-10,10,10 });	//当たり判定

		auto move = bullet->AddComponent<Move>();	//移動
		move->vx = vx;
		move->vy = vy;

		bullet->AddComponent<OutOfScreen>();	//画面外

		auto damageSource = bullet->AddComponent<DamageSource>();	//ダメージ
		damageSource->targetName = "player";	//プレイヤーを指定
		
		//連射カウンタを減らし、０以上になったら発射モード解除
		--fireCounter;

		if (fireCounter <= 0)
		{
			isFiring = false;	//発射フラグをfalseにする
		}
	}

	//初期化時に設定する変数
	GameObjectPtr target;	//標的
	Texcoord tc = MakeTexcoord(144, 365, 16, 16);	//テクスチャ座標
	float intervalA = 1;	//発射間隔
	float intervalB = 0.1f;	//連連射間隔
	int numberOfFire = 3;	//連射数
	float speed = 200;		//弾の速度
	float x = 0;			//発射位置のＸ座標
	float y = 0;			//発射位置のＹ座標

	//実行中に変化する変数
	bool isFiring = false;	//発射フラグ
	float timerA = 0;		//発射用タイマー
	float timerB = 0;		//連射用タイマー
	int fireCounter = 0;	//射数カウンタ
	float vx = 0;			//発射ベクトルＸ
	float vy = 0;			//発射ベクトルＹ

};



#endif // !COMPONENT_FIREBULLET_H_INCLUDED

