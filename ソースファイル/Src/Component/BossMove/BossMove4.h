//@file BossMove4.h

#ifndef COMPONENT_BOSSMOVE4_H_INCLUDED
#define COMPONENT_BOSSMOVE4_H_INCLUDED

#include "../../Application/Component.h"
#include "../../Application/GameObject.h"
#include <time.h>

//反射するボス
class BossMove4 :public Component
{
public:
	BossMove4() = default;			//コンストラクタ
	virtual ~BossMove4() = default;	//デストラクタ

	virtual void Start(GameObject& gameObject)override
	{
		MovePattern = 100;
		CenterX = 272;
		CenterY = 500;
		srand((unsigned int)time(NULL));
	}

	virtual void Update(GameObject& gameObject, float deltaTime)override
	{
		auto& ra = static_cast<MeshRenderer&>(*gameObject.componentList[0]);
		//敵のHPが減ったら画面を縦横無尽に動き回る
		if (gameObject.PinchFlg && !IsOnce)
		{
			vy = 1;
			IsOnce = true;
			BulletPattern = 10;
			Timer = 0;
			angle = 0;
			Angle = 0;
		}
		else if (!gameObject.PinchFlg)
		{
			vy = 0;
		}

		//登場したら
		if (MoveOK)
		{
			//本体を回転させる
			ra.rotation[1] += deltaTime * 5;

			//画面の端で跳ね返ってくる
			if (gameObject.x <= 72)
			{
				vx = vx * (-1);
			}
			if (gameObject.x >= 472)
			{
				vx = vx * (-1);
			}
			if (gameObject.y >= 632)
			{
				vy = vy * (-1);
			}
			if (gameObject.y <= 72)
			{
				vy = vy * (-1);
			}

			//移動処理
			gameObject.x += vx * deltaTime * MoveSpeed;
			gameObject.y += vy * deltaTime * MoveSpeed;


			switch (BulletPattern)
			{

				//上方向にでたらめに反射する弾を複数出す
			case 0:
				RandomUpShot(gameObject, deltaTime);
				break;

				//下方向に順番に撃つ
			case 1:
				DownShot(gameObject, deltaTime);
				break;

				//横下りの速い球を撃つ
			case 2:
				DescentShot(gameObject, deltaTime);
				break;

				//上方向に順番に撃つ
			case 3:
				UpShot(gameObject, deltaTime);
				break;

				//ボスが向いている方向に撃ち続ける
			case 10:
				if (Timer == 0)
				{
					Angle = 0;
					intervalB = 0.6f;	//連連射間隔
				}
				Timer += deltaTime;

				if (timerB > intervalB)
				{
					speed = 300;
					auto bullet = gameObject.engine->Create<GameObject>(
						"enemy bullet", gameObject.x, gameObject.y);

					bullet->AddSprite(tc[0]);					//テクスチャ
					bullet->priority = prioEnemyBullet;			//優先順位
					bullet->AddCollision({ -10,-10,10,10 });	//当たり判定
					Angle = ra.rotation[1];
					angle = Angle * 3.14f / 180;
					auto move = bullet->AddComponent<boundMove>();	//移動
					move->vx = speed * cos(angle);
					move->vy = speed * sin(angle);


					bullet->AddComponent<OutOfScreen>();		//画面外

					auto damageSource = bullet->AddComponent<DamageSource>();	//ダメージ
					damageSource->targetName = "player";	//プレイヤーを指定

					speed = 380;
					auto bullet2 = gameObject.engine->Create<GameObject>(
						"enemy bullet", gameObject.x, gameObject.y);

					bullet2->AddSprite(tc[1]);					//テクスチャ
					bullet2->priority = prioEnemyBullet;			//優先順位
					bullet2->AddCollision({ -10,-10,10,10 });	//当たり判定
					Angle = gameObject.radias;
					angle = (Angle + 180.0f) * 3.14f / 180;
					auto move2 = bullet2->AddComponent<boundMove>();	//移動
					move2->vx = speed * cos(angle);
					move2->vy = speed * sin(angle);


					bullet2->AddComponent<OutOfScreen>();		//画面外

					auto damageSource2 = bullet2->AddComponent<DamageSource>();	//ダメージ
					damageSource2->targetName = "player";	//プレイヤーを指定

					timerB -= intervalB;

				}
				else
				{
					timerB += deltaTime;

				}

				break;
			}

		}
		else
		{
		MoveStart(gameObject, deltaTime);
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

	//実行中に変化する変数
	int BulletPattern = 10;
	float angle = 0;		//発射角度

	bool LRMove = true;		//左右どっちに移動するか true=左に行く　false＝右に行く
	float timerA = 0;		//発射用タイマー
	float timerB = 0;		//連射用タイマー
	int fireCounter = 0;	//射数カウンタ
	float vx = 0;			//発射ベクトルＸ
	float vy = 0;			//発射ベクトルＹ
	float Timer = 0;		//移動なりに使う
	float Angle = 0;
	float CenterX = 272;	//ボスのx定位置
	float CenterY = 500;	//ボスのy定位置
	float MoveSpeed = 60;	//ボスの進む速さ

	bool MoveOK = false;	//目的地まで到達したかどうかtrue＝OK false＝NO
	int ShotCnt = 0;		//何回行動するか


private:

	//上方向にでたらめに反射する弾を複数出す
	void RandomUpShot(GameObject& gameObject, float deltaTime)
	{
		if (Timer == 0)
		{
			Angle = 0;
			numberOfFire = 20;
			intervalB = 0.4f;	//連連射間隔

		}
		Timer += deltaTime;

		if (numberOfFire > 0)
		{
			if (timerB > intervalB)
			{
				speed = 200;
				if (numberOfFire % 2 == 0)
				{
					auto bullet = gameObject.engine->Create<GameObject>(
						"enemy bullet", gameObject.x, gameObject.y);

					bullet->AddSprite(tc[0]);					//テクスチャ
					bullet->priority = prioEnemyBullet;			//優先順位
					bullet->AddCollision({ -10,-10,10,10 });	//当たり判定

					angle = (static_cast<float>(rand() % 40) + 30) * 3.14f / 180;
					auto move = bullet->AddComponent<boundMove>();	//移動
					move->vx = speed * cos(angle);
					move->vy = speed * sin(angle);


					bullet->AddComponent<OutOfScreen>();		//画面外

					auto damageSource = bullet->AddComponent<DamageSource>();	//ダメージ
					damageSource->targetName = "player";	//プレイヤーを指定
				}
				else
				{
					auto bullet = gameObject.engine->Create<GameObject>(
						"enemy bullet", gameObject.x, gameObject.y);

					bullet->AddSprite(tc[0]);					//テクスチャ
					bullet->priority = prioEnemyBullet;			//優先順位
					bullet->AddCollision({ -10,-10,10,10 });	//当たり判定

					angle = (static_cast<int>(rand()) % 30 + 120) * 3.14f / 180;
					auto move = bullet->AddComponent<boundMove>();	//移動
					move->vx = speed * cos(angle);
					move->vy = speed * sin(angle);

					bullet->AddComponent<OutOfScreen>();		//画面外

					auto damageSource = bullet->AddComponent<DamageSource>();	//ダメージ
					damageSource->targetName = "player";	//プレイヤーを指定

				}
				timerB -= intervalB;
				numberOfFire--;
			}
			else
			{
				timerB += deltaTime;

			}
		}
		else
		{
			BulletPattern = rand() % 4;
			Timer = 0;
		}
	}

	//下方向に向かって弾を撃つ
	void DownShot(GameObject& gameObject, float deltaTime)
	{
		if (Timer == 0)
		{
			Angle = 0;
			numberOfFire = 28;
			intervalB = 0.2f;	//連連射間隔

		}

		Timer += deltaTime;
		if (numberOfFire > 0)
		{
			if (timerB > intervalB)
			{
				speed = 200;
				if (numberOfFire > 14)
				{
					Angle += 10;
				}
				else
				{
					Angle -= 10;
				}

				auto bullet = gameObject.engine->Create<GameObject>(
					"enemy bullet", gameObject.x, gameObject.y);

				bullet->AddSprite(tc[0]);					//テクスチャ
				bullet->priority = prioEnemyBullet;			//優先順位
				bullet->AddCollision({ -10,-10,10,10 });	//当たり判定

				angle = (200 + Angle) * 3.14f / 180;
				auto move = bullet->AddComponent<boundMove>();	//移動
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
		else
		{
			BulletPattern = rand() % 4;
			Timer = 0;
		}

	}

	//下りながら降りてくる弾
	void DescentShot(GameObject& gameObject, float deltaTime)
	{
		if (Timer == 0)
		{
			Angle = 0;
			numberOfFire = 6;
			intervalB = 0.1f;	//連連射間隔

		}

		Timer += deltaTime;
		if (numberOfFire > 0)
		{
			if (timerB > intervalB)
			{

				if (numberOfFire > 3)
				{
					Angle = 190;
				}
				else
				{
					Angle = 350;
				}

				speed = 500;
				auto bullet = gameObject.engine->Create<GameObject>(
					"enemy bullet", gameObject.x, gameObject.y);

				bullet->AddSprite(tc[1]);					//テクスチャ
				bullet->priority = prioEnemyBullet;			//優先順位
				bullet->AddCollision({ -10,-10,10,10 });	//当たり判定

				angle = (Angle) * 3.14f / 180;
				auto move = bullet->AddComponent<boundMove>();	//移動
				move->Counter = -4;
				move->vx = speed * cos(angle);
				move->vy = speed * sin(angle);


				bullet->AddComponent<OutOfScreen>();		//画面外

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
		else
		{
			BulletPattern = rand() % 4;
			Timer = 0;
			numberOfFire = 20;
		}

	}

	//上方向に弾を打つ
	void UpShot(GameObject& gameObject, float deltaTime)
	{
		if (Timer == 0)
		{
			Angle = 0;
			numberOfFire = 28;
			intervalB = 0.2f;	//連連射間隔

		}

		Timer += deltaTime;
		if (numberOfFire > 0)
		{
			if (timerB > intervalB)
			{
				speed = 600;
				if (numberOfFire > 14)
				{
					Angle += 10;
				}
				else
				{
					Angle -= 10;
				}

				auto bullet = gameObject.engine->Create<GameObject>(
					"enemy bullet", gameObject.x, gameObject.y);

				bullet->AddSprite(tc[2]);					//テクスチャ
				bullet->priority = prioEnemyBullet;			//優先順位
				bullet->AddCollision({ -10,-10,10,10 });	//当たり判定

				angle = (20 + Angle) * 3.14f / 180;
				auto move = bullet->AddComponent<boundMove>();	//移動
				move->Counter = 1;
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
		else
		{
			BulletPattern = rand() % 4;
			Timer = 0;
		}

	}

	//行動開始
	void MoveStart(GameObject& gameObject, float deltaTime)
	{
		//画面の真ん中まで降りてくる処理
		if (gameObject.y > 570)
		{
			gameObject.y += -MoveSpeed * deltaTime * 10;
		}
		else
		{
			MoveOK = true;
			vx = 1;
			vy = 1;
			BulletPattern = rand() % 4;
		}
	}
};




#endif //COMPONENT_BOSSMOVE4_H_INCLUDED
