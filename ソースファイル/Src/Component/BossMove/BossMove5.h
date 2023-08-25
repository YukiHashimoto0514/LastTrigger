//@file BossMove5.h

#ifndef COMPONENT_BOSSMOVE5_H_INCLUDED
#define COMPONENT_BOSSMOVE5_H_INCLUDED

#include "../../Application/Component.h"
#include "../../Application/GameObject.h"
#include <time.h>

//貫通をイメージしたボス
class BossMove5 :public Component
{
public:
	BossMove5() = default;			//コンストラクタ
	virtual ~BossMove5() = default;	//デストラクタ

	virtual void Start(GameObject& gameObject)override
	{
		MovePattern = 100;
		CenterX = 272;
		CenterY = 500;
		srand((unsigned int)time(NULL));
	}

	virtual void Update(GameObject& gameObject, float deltaTime)override
	{

		//登場したら
		if (MoveOK)
		{
			switch (BulletPattern)
			{

			//下方向に一定の間隔で5発ずつロックオンしながら放つ
			case 0:
				LockOnShot(gameObject, deltaTime);
				break;

			//クロススティンガー
			case 1:
				CrossStingger(gameObject, deltaTime);
				break;

			//円を描きながら攻撃
			case 2:
				CircleShot(gameObject, deltaTime);
				break;

			//プレイヤーの方向にブレスのように弾を吐く
			case 3:
				BressShot(gameObject, deltaTime);
				break;
			}

		}
		else
		{
			//行動開始
			MoveStart(gameObject, deltaTime);
		}
	}

	//攻撃パターンを変える関数
	void ChabgeAttack(int old)
	{
		while (old == BulletPattern)
		{
			BulletPattern = rand() % 4;
		}
	}



		//初期化時に設定する変数
	int MovePattern = 100;	//行動パターン
	int rndMove = 0;		//行動する方向
	bool IsOnce = false;

		//弾のテクスチャ座標
	Texcoord tc[3] = { MakeTexcoord(144, 365, 16, 16),
					   MakeTexcoord(144, 365, 16, 16),
					   MakeTexcoord(144, 365, 16, 16),
	};

	float intervalA = 1;	//発射間隔
	float intervalB = 0.5f;	//連連射間隔
	int numberOfFire = 28;	//連射数
	float speed = 200;		//弾の速度
	float x = 0;			//発射位置のＸ座標
	float y = 0;			//発射位置のＹ座標
	GameObjectPtr target;	//標的
	GameObjectPtr boss;		//攻撃スクリプトに必要

	//実行中に変化する変数
	int BulletPattern = 0;	//行動パターン
	float angle = 0;		//発射角度

	bool LRMove = true;		//左右どっちに移動するか true=左に行く　false＝右に行く
	float timerA = 0;		//発射用タイマー
	float timerB = 0;		//連射用タイマー
	int fireCounter = 0;	//射数カウンタ
	float vx = 0;			//発射ベクトルＸ
	float vy = 0;			//発射ベクトルＹ
	float Timer = 0;		//移動なりに使う
	float Angle = 0;		//角度
	float CenterX = 272;	//ボスのx定位置
	float CenterY = 570;	//ボスのy定位置
	float MoveSpeed = 60;	//ボスの進む速さ

	bool MoveOK = false;							//目的地まで到達したかどうかtrue＝OK false＝NO
	int ShotCnt = 0;			//何回行動するか

	//HPが半分になった時に使う変数
	float BeforeX = 0;
	float BeforeY = 0;
	bool isOnce = false;

	private:
		//向いている方向に５発ずつ撃つ
		void LockOnShot(GameObject& gameObject, float deltaTime)
		{
			if (Timer == 0)
			{
				Angle = 0;
				intervalB = 0.5f;	//連連射間隔

			}
			Timer += deltaTime;

			if (Timer < 10)
			{
				if (timerB > intervalB)
				{
					speed = 300;
					vx = target->x - gameObject.x;
					vy = target->y - gameObject.y;

					float rad = atan2(vy, vx);
					Angle = rad * 180.0f / 3.14f;
					float AngleChild = Angle;


					//5方向に玉を打つ
					for (int i = -30; i < 60; i += 15)
					{
						auto bullet = gameObject.engine->Create<GameObject>(
							"enemy bullet", gameObject.x + x, gameObject.y + y);

						bullet->AddSprite(tc[0]);					//テクスチャ
						bullet->priority = prioEnemyBullet;		//優先順位
						bullet->AddCollision({ -10,-10,10,10 });	//当たり判定

						auto move = bullet->AddComponent<WallThroughMove>();	//移動
						Angle = i + AngleChild;

						rad = Angle * 3.14f / 180;
						move->vx = cos(rad) * speed;
						move->vy = sin(rad) * speed;
						move->Counter = 1;
						bullet->AddComponent<OutOfScreen>();	//画面外

						auto damageSource = bullet->AddComponent<DamageSource>();	//ダメージ
						damageSource->targetName = "player";	//プレイヤーを指定
					}
					timerB -= intervalB;
				}
				else
				{
					timerB += deltaTime;
				}
			}
			else if (Timer > 13)
			{
				//BulletPattern = rand() % 4;
				ChabgeAttack(0);
				Timer = 0;
			}

		}

		//クロススティンガーを撃つ
		void CrossStingger(GameObject& gameObject, float deltaTime)
		{
			if (Timer == 0)
			{
				angle = 0;
				numberOfFire = 20;
				intervalB = 0.1f;	//連連射間隔

				//移動前の座標
				BeforeY = gameObject.y;
			}

			Timer += deltaTime;
			if (numberOfFire > 0)
			{
				if (Timer < 1)
				{
					//移動処理（イージング）
					gameObject.y = BeforeY + Soft(Timer) * (650 - BeforeY);
				}
				else
				{
					if (timerB > intervalB)
					{
						speed = 600;

						auto bullet = gameObject.engine->Create<GameObject>(
							"enemy bullet", gameObject.x, gameObject.y);

						bullet->AddSprite(tc[0]);					//テクスチャ
						bullet->priority = prioEnemyBullet;			//優先順位
						bullet->AddCollision({ -10,-10,10,10 });	//当たり判定

						auto move = bullet->AddComponent<CrossMove>();	//移動
						move->Boss = boss;
						move->target = target;
						move->vx = speed;
						move->vy = speed;

						bullet->AddComponent<OutOfScreen>();		//画面外

						auto damageSource = bullet->AddComponent<DamageSource>();	//ダメージ
						damageSource->targetName = "player";	//プレイヤーを指定

						auto bullet2 = gameObject.engine->Create<GameObject>(
							"enemy bullet", gameObject.x, gameObject.y);

						bullet2->AddSprite(tc[1]);					//テクスチャ
						bullet2->priority = prioEnemyBullet;			//優先順位
						bullet2->AddCollision({ -10,-10,10,10 });	//当たり判定

						auto move2 = bullet2->AddComponent<CrossMove>();	//移動
						move2->Boss = boss;
						move2->target = target;
						move2->vx = -speed;
						move2->vy = speed;

						bullet2->AddComponent<OutOfScreen>();		//画面外

						auto damageSource2 = bullet2->AddComponent<DamageSource>();	//ダメージ
						damageSource2->targetName = "player";	//プレイヤーを指定

						timerB -= intervalB;
						numberOfFire--;

					}
					else
					{
						timerB += deltaTime;

					}
				}
			}
			else if (gameObject.y > CenterY)
			{
				gameObject.y -= MoveSpeed * deltaTime;
			}
			else
			{
				if (Timer > 3)
				{
					//BulletPattern = rand() % 4;
					ChabgeAttack(1);

					Timer = 0;
				}
			}

		}

		//円を描きながら攻撃する
		void CircleShot(GameObject& gameObject, float deltaTime)
		{
			if (Timer == 0)
			{
				Angle = 0;
				numberOfFire = 30;
				intervalB = 0.1f;	//連連射間隔

				//移動前の座標を代入
				BeforeY = gameObject.y;

			}

			Timer += deltaTime;
			if (numberOfFire > 0)
			{
				if (Timer < 1)
				{
					//移動処理（イージング）
					gameObject.y = BeforeY + Elastic(Timer) * (680 - BeforeY);

				}
				else
				{

					if (timerB > intervalB)
					{
						speed = 500;
						auto bullet = gameObject.engine->Create<GameObject>(
							"enemy bullet", gameObject.x, gameObject.y);

						bullet->AddSprite(tc[1]);					//テクスチャ
						bullet->priority = prioEnemyBullet;			//優先順位
						bullet->AddCollision({ -10,-10,10,10 });	//当たり判定

						angle = (Angle) * 3.14f / 180;
						auto move = bullet->AddComponent<CircleLerp>();	//移動
						//move->vx = speed;
						//move->vy = speed;
						move->target = target;

						auto damageSource = bullet->AddComponent<DamageSource>();	//ダメージ
						damageSource->targetName = "player";	//プレイヤーを指定

						numberOfFire--;
						timerB -= intervalB;
					}
					else
					{
						timerB += deltaTime;
					}
				}
			}
			else if (gameObject.y > CenterY)
			{
				gameObject.y -= MoveSpeed * deltaTime;
			}
			else
			{
				ChabgeAttack(2);
				Timer = 0;
			}

		}

		//ブレスを撃つ
		void BressShot(GameObject& gameObject, float deltaTime)
		{
			if (Timer == 0)
			{
				Angle = 0;
				numberOfFire = 200;
				intervalB = 0.01f;	//連連射間隔

			}

			Timer += deltaTime;
			if (numberOfFire > 0)
			{

				if (timerB > intervalB)
				{
					speed = 500 + static_cast<float>(sin(rand() % 360)) * 100;

					auto bullet = gameObject.engine->Create<GameObject>(
						"enemy bullet", gameObject.x, gameObject.y);

					bullet->AddSprite(tc[2]);					//テクスチャ
					bullet->priority = prioEnemyBullet;			//優先順位
					bullet->AddCollision({ -10,-10,10,10 });	//当たり判定

					float disx = target->x - gameObject.x;
					float disy = target->y - gameObject.y;
					angle = atan2(disy, disx);
					Angle = static_cast<float>(angle * 180) / 3.14f;
					Angle += static_cast<float>(sin(rand() % 360)) * 5;
					angle = Angle * 3.14f / 180;

					auto move = bullet->AddComponent<Move>();	//移動
					move->vx = speed * cos(angle);
					move->vy = speed * sin(angle);


					bullet->AddComponent<OutOfScreen>();		//画面外

					auto damageSource = bullet->AddComponent<DamageSource>();	//ダメージ
					damageSource->targetName = "player";	//プレイヤーを指定
					timerB -= intervalB;
					numberOfFire--;

				}
				else
				{
					timerB += deltaTime;

				}
			}
			else if (Timer > 5)
			{
				ChabgeAttack(3);
				Timer = 0;
			}
		}

		//最初の動き
		void MoveStart(GameObject& gameObject, float deltaTime)
		{
			if (Timer == 0)
			{
				//現在の位置を代入
				BeforeX = gameObject.x;
				BeforeY = gameObject.y;
			}
			Timer += deltaTime;

			if (Timer <= 1)
			{
				gameObject.y = BeforeY + Circle(Timer) * (500 - BeforeY);
			}
			else
			{
				MoveOK = true;
				vx = 1;
				vy = 1;
				Timer = 0;
				BulletPattern = 2;
			}

		}


};


#endif //COMPONENT_BOSSMOVE5_H_INCLUDED
