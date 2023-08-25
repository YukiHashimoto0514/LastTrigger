//@file BossMove3.h

#ifndef COMPONENT_BOSSMOVE3_H_INCLUDED
#define COMPONENT_BOSSMOVE3_H_INCLUDED

#include "../../Application/Component.h"
#include "../../Application/GameObject.h"
#include <time.h>

//ランダムを基にしたボス
class BossMove3 :public Component
{
public:
	BossMove3() = default;			//コンストラクタ
	virtual ~BossMove3() = default;	//デストラクタ

	virtual void Start(GameObject& gameObject)override
	{
		MovePattern = 100;
		CenterX = 272;
		CenterY = 500;
		srand((unsigned int)time(NULL));
	}

	virtual void Update(GameObject& gameObject, float deltaTime)override
	{
		if (gameObject.PinchFlg)
		{
			speed = 300;

			if (!isOnce)
			{
				MovePattern = 50;
				Timer = 0;
				isOnce = true;
			}
		}

		switch (MovePattern)
		{
		//中央からの動き
		case 0:

			if (Timer == 0)
			{
				rndMove = rand() % 5;
				Timer++;
				MoveSpeed = static_cast <float>(60 + (rand() % 40 - 11));//ボスの速さを－10～30の値で変化させる
				BulletPattern = rand() % 2;
			}


			//どの方向に進むか
			if (rndMove == 0)		//上に行く
			{
				if (gameObject.y < 700)
				{
					gameObject.y += MoveSpeed * deltaTime;
				}
				else
				{
					MoveOK = true;
					MovePattern = 5;
				}
			}
			else if (rndMove == 1)	//左上に行く
			{
				if (gameObject.x > LeftUpX)
				{
					gameObject.x -= MoveSpeed * deltaTime;
				}
				else
				{
					MoveOK = true;
					MovePattern = 1;
				}


				if (gameObject.y < UpY)
				{
					gameObject.y += MoveSpeed * deltaTime;
				}
			}
			else if (rndMove == 2) //左下
			{
				if (gameObject.x > LeftDownX)
				{
					gameObject.x -= MoveSpeed * deltaTime;
				}

				if (gameObject.y > DownY)
				{
					gameObject.y -= MoveSpeed * deltaTime;
				}
				else
				{
					MoveOK = true;
					MovePattern = 2;

				}

			}
			else if (rndMove == 3) //右下
			{
				if (gameObject.x < RightDownX)
				{
					gameObject.x += MoveSpeed * deltaTime;
				}

				if (gameObject.y > DownY)
				{
					gameObject.y -= MoveSpeed * deltaTime;
				}
				else
				{
					MoveOK = true;
					MovePattern = 3;

				}

			}
			else if (rndMove == 4) //右上
			{
				if (gameObject.x < RightUpX)
				{
					gameObject.x += MoveSpeed * deltaTime;
				}
				else
				{
					MoveOK = true;
					MovePattern = 4;

				}

				if (gameObject.y < UpY)
				{
					gameObject.y += MoveSpeed * deltaTime;
				}

			}

			if (MoveOK)
			{
				Timer = 0;
				MoveOK = false;
			}

			break;

		//左上からの動き
		case 1:

			if (Timer == 0)
			{
				rndMove = rand() % 3;
				Timer++;
				MoveSpeed = static_cast <float>(60 + (rand() % 40 - 11));//ボスの速さを－10～30の値で変化させる
				BulletPattern = rand() % 2;

			}


			//どの方向に進むか
			if (rndMove == 0)		//上に行く
			{
				if (gameObject.x < CenterX)
				{
					gameObject.x += MoveSpeed * deltaTime;
				}
				else
				{
					MoveOK = true;
					MovePattern = 5;

				}

				if (gameObject.y < 700)
				{
					gameObject.y += MoveSpeed * deltaTime;
				}


			}
			else if (rndMove == 1)	//左下に行く
			{
				if (gameObject.x > LeftDownX)
				{
					gameObject.x -= MoveSpeed * deltaTime;
				}

				if (gameObject.y > DownY)
				{
					gameObject.y -= MoveSpeed * deltaTime;
				}
				else
				{
					MoveOK = true;
					MovePattern = 2;

				}

			}
			else if (rndMove == 2) //中央に行く
			{
				if (gameObject.x < CenterX)
				{
					gameObject.x += MoveSpeed * deltaTime;
				}
				else
				{
					MoveOK = true;
					MovePattern = 0;

				}


				if (gameObject.y > CenterY)
				{
					gameObject.y -= MoveSpeed * deltaTime;
				}
			}

			//次の動きに移行する
			if (MoveOK)
			{
				Timer = 0;
				MoveOK = false;
			}


			break;

		//左下からの動き
		case 2:

			if (Timer == 0)
			{
				rndMove = rand() % 3;
				Timer++;
				BulletPattern = rand() % 2;

				MoveSpeed = static_cast <float>(60 + (rand() % 40 - 11));//ボスの速さを－10～30の値で変化させる
			}


			//どの方向に進むか
			if (rndMove == 0)		//左上に行く
			{
				if (gameObject.x > LeftUpX)
				{
					gameObject.x -= MoveSpeed * deltaTime;
				}

				if (gameObject.y < UpY)
				{
					gameObject.y += MoveSpeed * deltaTime;
				}
				else
				{
					MoveOK = true;
					MovePattern = 1;

				}


			}
			else if (rndMove == 1)	//右下に行く
			{
				if (gameObject.x < RightDownX)
				{
					gameObject.x += MoveSpeed * deltaTime;
				}
				else
				{
					MoveOK = true;
					MovePattern = 3;
				}
			}
			else if (rndMove == 2) //中央に行く
			{
				if (gameObject.x < CenterX)
				{
					gameObject.x += MoveSpeed * deltaTime;
				}


				if (gameObject.y < CenterY)
				{
					gameObject.y += MoveSpeed * deltaTime;
				}
				else
				{
					MoveOK = true;
					MovePattern = 0;

				}

			}

			//次の動きに移行する
			if (MoveOK)
			{
				MovePattern = 3;
				Timer = 0;
				MoveOK = false;
			}


			break;

		//右下からの動き
		case 3:

			if (Timer == 0)
			{
				rndMove = rand() % 3;
				Timer++;
				MoveSpeed = static_cast <float>(60 + (rand() % 40 - 11));//ボスの速さを－10～30の値で変化させる
				BulletPattern = rand() % 2;

			}


			//どの方向に進むか
			if (rndMove == 0)		//左下に行く
			{
				if (gameObject.x > LeftDownX)
				{
					gameObject.x -= MoveSpeed * deltaTime;
				}
				else
				{
					MoveOK = true;
					MovePattern = 2;

				}

			}
			else if (rndMove == 1)	//右上に行く
			{
				if (gameObject.x < RightUpX)
				{
					gameObject.x += MoveSpeed * deltaTime;
				}

				if (gameObject.y < UpY)
				{
					gameObject.y += MoveSpeed * deltaTime;
				}
				else
				{
					MoveOK = true;
					MovePattern = 4;

				}



			}
			else if (rndMove == 2) //中央に行く
			{
				if (gameObject.x > CenterX)
				{
					gameObject.x -= MoveSpeed * deltaTime;
				}


				if (gameObject.y < CenterY)
				{
					gameObject.y += MoveSpeed * deltaTime;
				}
				else
				{
					MoveOK = true;
					MovePattern = 0;

				}

			}

			//次の動きに移行する
			if (MoveOK)
			{
				MovePattern = 4;
				Timer = 0;
				MoveOK = false;
			}



			break;

		//右上からの動き
		case 4:

			if (Timer == 0)
			{
				rndMove = rand() % 3;
				Timer++;
				MoveSpeed = static_cast <float>(60 + (rand() % 40 - 11));//ボスの速さを－10～30の値で変化させる
				BulletPattern = rand() % 2;
			}

			//どの方向に進むか
			if (rndMove == 0)		//右下に行く
			{
				if (gameObject.x > RightDownX)
				{
					gameObject.x -= MoveSpeed * deltaTime;
				}

				if (gameObject.y > DownY)
				{
					gameObject.y -= MoveSpeed * deltaTime;
				}
				else
				{
					MoveOK = true;
					MovePattern = 3;

				}

			}
			else if (rndMove == 1)	//上に行く
			{
				if (gameObject.x > CenterX)
				{
					gameObject.x -= MoveSpeed * deltaTime;
				}
				else
				{
					MoveOK = true;
					MovePattern = 5;

				}

				if (gameObject.y < 700)
				{
					gameObject.y += MoveSpeed * deltaTime;
				}



			}
			else if (rndMove == 2) //中央に行く
			{
				if (gameObject.x > CenterX)
				{
					gameObject.x -= MoveSpeed * deltaTime;
				}
				else
				{
					MoveOK = true;
					MovePattern = 0;
				}


				if (gameObject.y > CenterY)
				{
					gameObject.y -= MoveSpeed * deltaTime;
				}

			}

			//次の動きに移行する
			if (MoveOK)
			{
				Timer = 0;
				MoveOK = false;
			}
			break;

		//上からの動き
		case 5:
			if (Timer == 0)
			{
				rndMove = rand() % 3;
				Timer++;
				MoveSpeed = static_cast <float>(60 + (rand() % 40 - 11));//ボスの速さを－10～30の値で変化させる
				BulletPattern = 2;
			}

			//どの方向に進むか
			if (rndMove == 0)	//右上に行く
			{
				if (gameObject.x < RightUpX)
				{
					gameObject.x += MoveSpeed * deltaTime;
				}
				else
				{
					MoveOK = true;
					MovePattern = 4;

				}

				if (gameObject.y > UpY)
				{
					gameObject.y -= MoveSpeed * deltaTime;
				}

			}
			else if (rndMove == 1)	//左上に行く
			{
				if (gameObject.x > LeftUpX)
				{
					gameObject.x -= MoveSpeed * deltaTime;
				}
				else
				{
					MoveOK = true;
					MovePattern = 1;

				}

				if (gameObject.y > UpY)
				{
					gameObject.y -= MoveSpeed * deltaTime;
				}
			}
			else if (rndMove == 2) //中央に行く
			{
				if (gameObject.y > CenterY)
				{
					gameObject.y -= MoveSpeed * deltaTime;
				}
				else
				{
					MoveOK = true;
					MovePattern = 0;
				}


			}

			//次の動きに移行する
			if (MoveOK)
			{
				Timer = 0;
				MoveOK = false;
			}

			break;

		//真ん中に戻る
		case 50:

			if (Timer == 0)
			{
				//現在の位置を代入
				BeforeX = gameObject.x;
				BeforeY = gameObject.y;
			}

			if (Timer < 1)
			{
				Timer += deltaTime;
			}
			else
			{
				Timer = 1;
			}

			//移動処理（イージング）
			gameObject.x = BeforeX + Elastic(Timer) * (CenterX - BeforeX);
			gameObject.y = BeforeY + Elastic(Timer) * (CenterY - BeforeY);

			if (Timer == 1)
			{
				MovePattern = 0;
				Timer = 0;
			}


			break;


			//最初の一回のみ呼ばれる
		case 100:
			//ゆっくり降りてくる
			if (Timer == 0)
			{
				BeforeY = gameObject.y;
			}
			Timer += deltaTime;

			if (Timer <= 1)
			{
				gameObject.y = BeforeY + Circle(Timer) * (500 - BeforeY);
			}
			else
			{
				MovePattern = 0;
				Timer = 0;
			}
			break;
		}

		//やんわりプレイヤー方向にランダム50発
		if (BulletPattern==0)
		{
			RandomShot(gameObject, deltaTime);
		}

		//左右から全方向に発射
		if (BulletPattern==1)
		{
			AllRangeShot(gameObject, deltaTime);
		}

		//プレイヤーのいる方向に弾を撃つ
		if (BulletPattern == 2)
		{
			LockPlayerShot(gameObject, deltaTime);
		}


	}

	//初期化時に設定する変数
	int MovePattern = 100;	//行動パターン
	int rndMove = 0;		//行動する方向

	//弾のテクスチャ座標
	Texcoord tc[2] = { MakeTexcoord(144, 365, 16, 16),
	};

	float intervalA = 1;	//発射間隔
	float intervalB = 0.1f;	//連連射間隔
	int numberOfFire = 26;	//連射数
	float speed = 200;		//弾の速度
	float x = 0;			//発射位置のＸ座標
	float y = 0;			//発射位置のＹ座標
	float MaxHP;
	GameObjectPtr target;	//標的

	//実行中に変化する変数
	int BulletPattern = 2;
	float angle = 0;			//発射角度
	bool ShotFlg;//true=撃つ　false＝待ち

	bool isFiring;			//発射フラグ
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

	float RightUpX = 440;
	float RightDownX = 380;
	float LeftDownX = 200;
	float LeftUpX = 180;;	
	float UpY = 500;
	float DownY = 400;
	bool MoveOK = false;							//目的地まで到達したかどうかtrue＝OK false＝NO
	int ShotCnt;			//何回行動するか

	//HPが半分になった時に使う変数
	float BeforeX = 0;
	float BeforeY = 0;
	bool isOnce = false;

	private:

		//プレイヤーのいる方向にランダムで弾を５０発発射
		void RandomShot(GameObject& gameObject, float deltaTime)
		{
			x = 0;
			y = -80;

			numberOfFire = 50;
			ShotFlg = true;
			if (ShotFlg)
			{
				//発射用タイマーが発射間隔を超えたら、発射変数を初期化
				timerA += deltaTime;//タイマーを加算

				if (timerA >= intervalA)	//タイマーがインターバルを超えたら
				{
					isFiring = true;		//発射フラグtrue
					timerA -= intervalA;	//タイマーを減らす
					timerB = intervalB;		//一発目が即座に発射される
					fireCounter = numberOfFire;

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


				//弾を発射
				auto bullet = gameObject.engine->Create<GameObject>(
					"enemy bullet", gameObject.x + x, gameObject.y);

				bullet->priority = prioPlayer;		//優先順位
				bullet->AddCollision({ -10,-10,10,10 });	//当たり判定
				bullet->AddSprite(tc[0]);					//テクスチャ
				float disX = target->x - gameObject.x;
				float disY = target->y - gameObject.y;

				Angle = atan2(disY, disX);
				angle = Angle * 180.0f / 3.14f;//度に直す
				angle += rand() % 60 - 30;//-30~29
				Angle = angle * 3.14f / 180;//ラジアンに

				if (fireCounter % 2 == 0)
				{
					auto move = bullet->AddComponent<MoveAndStop>();	//移動
					move->vx = speed * cos(Angle);
					move->vy = speed * sin(Angle);
				}
				else
				{
					auto move = bullet->AddComponent<MoveAndStop>();	//移動
					move->vx = speed * cos(Angle);
					move->vy = speed * sin(Angle);
					move->Stop = 2;
				}
				bullet->AddComponent<OutOfScreen>();	//画面外

				auto damageSource = bullet->AddComponent<DamageSource>();	//ダメージ
				damageSource->targetName = "player";	//プレイヤーを指定


				//連射カウンタを減らし、０以上になったら発射モード解除
				--fireCounter;

				if (fireCounter <= 0)
				{
					isFiring = false;	//発射フラグをfalseにする
					ShotFlg = false;
				}
			}

		}

		//２か所から全方向に弾をうつ
		void AllRangeShot(GameObject& gameObject, float deltaTime)
		{
			numberOfFire = 200;
			ShotFlg = true;
			if (ShotFlg)
			{
				//発射用タイマーが発射間隔を超えたら、発射変数を初期化
				timerA += deltaTime;//タイマーを加算

				if (timerA >= intervalA)	//タイマーがインターバルを超えたら
				{
					isFiring = true;		//発射フラグtrue
					timerA -= intervalA;	//タイマーを減らす
					timerB = intervalB;		//一発目が即座に発射される
					fireCounter = numberOfFire;

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


				//弾を発射
				x = 25;
				y = -100;
				auto bullet = gameObject.engine->Create<GameObject>(
					"enemy bullet", gameObject.x + x, gameObject.y);

				bullet->AddSprite(tc[0]);					//テクスチャ
				bullet->priority = prioPlayer;		//優先順位
				bullet->AddCollision({ -10,-10,10,10 });	//当たり判定

				float angle = static_cast<float>(rand() % 360);
				angle = angle * 3.14f / 180;
				auto move = bullet->AddComponent<MoveAndStop>();	//移動
				move->vx = speed * cos(angle) - rand() % 100;
				move->vy = speed * sin(angle) - rand() % 100;

				bullet->AddComponent<OutOfScreen>();	//画面外

				auto damageSource = bullet->AddComponent<DamageSource>();	//ダメージ
				damageSource->targetName = "player";	//プレイヤーを指定


				//弾を発射
				auto bullet2 = gameObject.engine->Create<GameObject>(
					"enemy bullet2", gameObject.x - x, gameObject.y);

				bullet2->AddSprite(tc[0]);					//テクスチャ
				bullet2->priority = prioPlayer;		//優先順位
				bullet2->AddCollision({ -10,-10,10,10 });	//当たり判定

				angle = static_cast<float>(rand() % 360);
				angle = angle * 3.14f / 180;
				auto move2 = bullet2->AddComponent<MoveAndStop>();	//移動
				move2->vx = speed * cos(angle) + rand() % 100;
				move2->vy = speed * sin(angle) + rand() % 100;
				move2->Stop = 2;
				bullet2->AddComponent<OutOfScreen>();	//画面外

				auto damageSource2 = bullet2->AddComponent<DamageSource>();	//ダメージ
				damageSource2->targetName = "player";	//プレイヤーを指定

				//連射カウンタを減らし、０以上になったら発射モード解除
				--fireCounter;

				if (fireCounter <= 0)
				{
					isFiring = false;	//発射フラグをfalseにする
					ShotFlg = false;
				}
			}

		}

		void LockPlayerShot(GameObject& gameObject, float deltaTime)
		{
			x = 0;
			y = -80;
			vx = target->x - gameObject.x;
			vy = target->y - gameObject.y;

			float rad = atan2(vy, vx);
			Angle = rad * 180 / 3.14f;
			float AngleChild = Angle;


			//11方向に玉を打つ
			if (timerB > intervalB)
			{
				for (int i = -15; i < 20; i += 5)
				{
					auto bullet = gameObject.engine->Create<GameObject>(
						"enemy bullet", gameObject.x + x, gameObject.y);

					bullet->AddSprite(tc[0]);					//テクスチャ
					bullet->priority = prioEnemyBullet;		//優先順位
					bullet->AddCollision({ -10,-10,10,10 });	//当たり判定

					auto move = bullet->AddComponent<MoveAndStop>();	//移動
					Angle = i + AngleChild;

					rad = Angle * 3.14f / 180;
					move->vx = cos(rad) * speed;
					move->vy = sin(rad) * speed;

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
};




#endif //COMPONENT_BOSSMOVE3_H_INCLUDED
