//@file BossMove.h

#ifndef COMPONENT_BOSSMOVE_H_INCLUDED
#define COMPONENT_BOSSMOVE_H_INCLUDED

#include "../../Application/Component.h"
#include "../../Application/GameObject.h"
#include <time.h>
#include "../Move/Lerp.h"

//全てを踏襲したボス
class BossMove :public Component
{
public:
	BossMove() = default;			//コンストラクタ
	virtual ~BossMove() = default;	//デストラクタ

	virtual void Start(GameObject& gameObject)override
	{
		Pattern = 100;
		srand((unsigned int)time(NULL));
		CenterX = 272;
		CenterY = 500;

	}

	virtual void Update(GameObject& gameObject, float deltaTime)override
	{
		//HPが半分を切った時に一度だけ呼ばれる
		if (gameObject.PinchFlg && !isOnce)
		{
			ShotCnt = 2;

			Timer = 0;
			Pattern = 50;
			TakeTime = 0;
			isOnce = true;
		}

		switch (Pattern)
		{
		//プレイヤーの方向に拡散団を撃つ
		case 0:
			SideMoveAttack(gameObject, deltaTime);
		break;

		//風車みたいにくるくる回る
		case 1:
			EddyAttack(gameObject, deltaTime);
			break;

		//上からめちゃ撃つ
		case 2:
			BressShot(gameObject, deltaTime);
			break;

		//2か所からランダムな弾を放つ
		case 3:
			RandomShot(gameObject, deltaTime);
			break;

		//上方向に反射する弾を放つ
		case 4:
			UpBoundShot(gameObject, deltaTime);
			break;

		//クロススティンガー
		case 5:
			CrossStinger(gameObject, deltaTime);
			break;

		//HPが半分になったら真ん中に戻る
		case 50:
			HalfHP(gameObject, deltaTime);
			break;
		//登場
		case 100:
			MoveStart(gameObject, deltaTime);
			break;

		}
	}

	//攻撃パターンを変える関数
	void ChabgeAttack(int old)
	{
		while (old == Pattern)
		{
			Pattern = rand() % 6;
		}
	}


	//初期化時に設定する変数
	int Pattern = 100;//行動パターン
	int CenterX = 272;
	int CenterY = 500;
	int Right = 404;
	int Left = 180;
	//弾のテクスチャ座標
	Texcoord tc[2] = { MakeTexcoord(144, 365, 16, 16),
					   MakeTexcoord(144, 365, 16, 16) };

	float intervalA =6;		//発射間隔
	float intervalB = 0.1f;	//連連射間隔
	int numberOfFire = 26;	//連射数
	int ShotCnt = 1;
	float speed = 200;		//弾の速度
	float x = 0;			//発射位置のＸ座標
	float y = 0;			//発射位置のＹ座標

	GameObjectPtr target;	//標的
	GameObjectPtr Boss;		//ボス

	//実行中に変化する変数
	bool ShotFlg = false;	//true=撃つ　false＝待ち
	bool LRMove = true;		//左右どっちに移動するか true=左に行く　false＝右に行く

	float timerA = 0;		//発射用タイマー
	float timerB = 0;		//連射用タイマー
	int fireCounter = 0;	//射数カウンタ
	float vx = 0;			//発射ベクトルＸ
	float vy = 0;			//発射ベクトルＹ
	float Timer = 0;
	float Deg = 0;			//度数法の角度
	float Angle = 0;		//弧度法の角度
	float MoveSpeed = 80;

	bool isOnce = false;	//一度きりの処理で仕様

	//イージングに使う変数
	float BeforeX = 0;
	float BeforeY = 0;
	float TakeTime = 0;
	bool EasingFlg = false;

	private:
		//左右に移動して攻撃
		void SideMoveAttack(GameObject& gameObject, float deltaTime)
		{
			//２回右にったら
			if (Timer == 2)
			{
				if (gameObject.x < CenterX)
				{
					Timer = 0;
					ShotFlg = false;
					ChabgeAttack(0);
				}
				else
				{
					gameObject.x -= MoveSpeed * deltaTime;
				}
			}
			else
			{
				//左側に行ったら
				if (gameObject.x <= Left)
				{
					LRMove = false;
					ShotFlg = true;
				}
				else if (gameObject.x >= Right)
				{
					LRMove = true;
					ShotFlg = true;
					Timer++;
				}

				if (LRMove)	//左に行く
				{
					gameObject.x -= MoveSpeed * deltaTime;
				}
				else
				{
					gameObject.x += MoveSpeed * deltaTime;
				}

				//弾を発射
				if (ShotFlg)
				{
					//移動値を設定
					if (gameObject.PinchFlg)
					{
						speed = 600;
					}
					else
					{
						speed = 500;
					}

					//角度を算出
					vx = target->x - gameObject.x;
					vy = target->y - gameObject.y;
					Deg = atan2(vy, vx);
					Deg = Deg * 180 / 3.14f;	//度数法に変換

					for (Angle = Deg - 30; Angle <= Deg + 30; Angle += 7)
					{
						auto bullet = gameObject.engine->Create<GameObject>(
							"enemy bullet", gameObject.x, gameObject.y);

						bullet->AddSprite(tc[0]);					//テクスチャ
						bullet->priority = prioEnemyBullet;			//優先順位
						bullet->AddCollision({ -10,-10,10,10 });	//当たり判定

						const float rad = Angle * 3.14f / 180;		//弧度法に変換
						auto move = bullet->AddComponent<Move>();	//移動
						move->vx = speed * cos(rad);
						move->vy = speed * sin(rad);

						bullet->AddComponent<OutOfScreen>();		//画面外

						auto damageSource = bullet->AddComponent<DamageSource>();	//ダメージ
						damageSource->targetName = "player";	//プレイヤーを指定
					}
					ShotFlg = false;
				}
			}

		}

		//渦攻撃
		void EddyAttack(GameObject& gameObject, float deltaTime)
		{
			if (Timer == 0)
			{
				intervalB = 0.1f;	//連射間隔
				BeforeX = gameObject.x;
				BeforeY = gameObject.y;
				TakeTime = 0;
				Angle = 0;
				Timer++;
			}

			if (Timer < ShotCnt + 1)
			{
				TakeTime += deltaTime;
				//画面の中心に行く
				if (TakeTime <= 1)
				{
					//移動処理（イージング）
					gameObject.y = BeforeY + Bound(TakeTime) * (400 - BeforeY);
				}
				else
				{
					//一周させる
					if (Deg < 360)
					{
						//渦巻を巻きながら弾を発射させる
						timerB += deltaTime;//タイマーを加算
						if (timerB >= intervalB)	//タイマーがインターバルを超えたら
						{
							speed = 400;
							//弾を発射
							auto bullet = gameObject.engine->Create<GameObject>(
								"enemy bullet", gameObject.x + x, gameObject.y + y);

							bullet->AddSprite(tc[0]);				//テクスチャ
							bullet->priority = prioEnemyBullet;		//優先順位
							bullet->AddCollision({ -10,-10,10,10 });//当たり判定

							auto move = bullet->AddComponent<Move>();	//移動
							Angle = Deg * 3.14f / 180;	//弧度法に変換
							move->vx = cos(Angle) * speed;
							move->vy = sin(Angle) * speed;

							bullet->AddComponent<OutOfScreen>();	//画面外

							auto damageSource = bullet->AddComponent<DamageSource>();	//ダメージ
							damageSource->targetName = "player";	//プレイヤーを指定


							//弾を発射
							auto bullet3 = gameObject.engine->Create<GameObject>(
								"enemy bullet", gameObject.x + x, gameObject.y + y);

							bullet3->AddSprite(tc[0]);					//テクスチャ
							bullet3->priority = prioEnemyBullet;		//優先順位
							bullet3->AddCollision({ -10,-10,10,10 });	//当たり判定

							auto move3 = bullet3->AddComponent<Move>();	//移動
							Angle = (Deg + 90) * 3.14f / 180;			//弧度法に変換
							move3->vx = cos(Angle) * speed;
							move3->vy = sin(Angle) * speed;

							bullet3->AddComponent<OutOfScreen>();	//画面外

							auto damageSource3 = bullet3->AddComponent<DamageSource>();	//ダメージ
							damageSource3->targetName = "player";	//プレイヤーを指定


								//弾を発射
							auto bullet2 = gameObject.engine->Create<GameObject>(
								"enemy bullet", gameObject.x + x, gameObject.y + y);

							bullet2->AddSprite(tc[0]);					//テクスチャ
							bullet2->priority = prioEnemyBullet;		//優先順位
							bullet2->AddCollision({ -10,-10,10,10 });	//当たり判定

							auto move2 = bullet2->AddComponent<Move>();	//移動
							Angle = Deg * 3.14f / 180;					//弧度法に変換
							move2->vx = cos(Angle) * speed * (-1);		//反対方向に発射するため*-1
							move2->vy = sin(Angle) * speed * (-1);		//反対方向に発射するため*-1

							bullet2->AddComponent<OutOfScreen>();	//画面外

							auto damageSource2 = bullet2->AddComponent<DamageSource>();	//ダメージ
							damageSource2->targetName = "player";	//プレイヤーを指定


															//弾を発射
							auto bullet4 = gameObject.engine->Create<GameObject>(
								"enemy bullet", gameObject.x + x, gameObject.y + y);

							bullet4->AddSprite(tc[0]);					//テクスチャ
							bullet4->priority = prioEnemyBullet;		//優先順位
							bullet4->AddCollision({ -10,-10,10,10 });	//当たり判定

							auto move4 = bullet4->AddComponent<Move>();	//移動
							Angle = (Deg + 90) * 3.14f / 180;			//弧度法に変換(少し角度をずらして発射)
							move4->vx = cos(Angle) * speed * (-1);
							move4->vy = sin(Angle) * speed * (-1);

							bullet4->AddComponent<OutOfScreen>();	//画面外

							auto damageSource4 = bullet4->AddComponent<DamageSource>();	//ダメージ
							damageSource4->targetName = "player";	//プレイヤーを指定

							if (gameObject.PinchFlg)
							{
								//弾を発射
								auto bullet = gameObject.engine->Create<GameObject>(
									"enemy bullet", gameObject.x + x, gameObject.y + y);

								bullet->AddSprite(tc[1]);					//テクスチャ
								bullet->priority = prioEnemyBullet;		//優先順位
								bullet->AddCollision({ -10,-10,10,10 });	//当たり判定

								auto move = bullet->AddComponent<Move>();	//移動
								Angle = (Deg + 45) * 3.14f / 180;
								move->vx = cos(Angle) * speed;
								move->vy = sin(Angle) * speed;

								bullet->AddComponent<OutOfScreen>();	//画面外

								auto damageSource = bullet->AddComponent<DamageSource>();	//ダメージ
								damageSource->targetName = "player";	//プレイヤーを指定


								//弾を発射
								auto bullet3 = gameObject.engine->Create<GameObject>(
									"enemy bullet", gameObject.x + x, gameObject.y + y);

								bullet3->AddSprite(tc[1]);					//テクスチャ
								bullet3->priority = prioEnemyBullet;		//優先順位
								bullet3->AddCollision({ -10,-10,10,10 });	//当たり判定

								auto move3 = bullet3->AddComponent<Move>();	//移動
								Angle = (Deg + 135) * 3.14f / 180;
								move3->vx = cos(Angle) * speed;
								move3->vy = sin(Angle) * speed;

								bullet3->AddComponent<OutOfScreen>();	//画面外

								auto damageSource3 = bullet3->AddComponent<DamageSource>();	//ダメージ
								damageSource3->targetName = "player";	//プレイヤーを指定


									//弾を発射
								auto bullet2 = gameObject.engine->Create<GameObject>(
									"enemy bullet", gameObject.x + x, gameObject.y + y);

								bullet2->AddSprite(tc[1]);					//テクスチャ
								bullet2->priority = prioEnemyBullet;		//優先順位
								bullet2->AddCollision({ -10,-10,10,10 });	//当たり判定

								auto move2 = bullet2->AddComponent<Move>();	//移動
								Angle = (Deg + 45) * 3.14f / 180;
								move2->vx = cos(Angle) * speed * (-1);
								move2->vy = sin(Angle) * speed * (-1);

								bullet2->AddComponent<OutOfScreen>();	//画面外

								auto damageSource2 = bullet2->AddComponent<DamageSource>();	//ダメージ
								damageSource2->targetName = "player";	//プレイヤーを指定


																//弾を発射
								auto bullet4 = gameObject.engine->Create<GameObject>(
									"enemy bullet", gameObject.x + x, gameObject.y + y);

								bullet4->AddSprite(tc[1]);					//テクスチャ
								bullet4->priority = prioEnemyBullet;		//優先順位
								bullet4->AddCollision({ -10,-10,10,10 });	//当たり判定

								auto move4 = bullet4->AddComponent<Move>();	//移動
								Angle = (Deg + 135) * 3.14f / 180;
								move4->vx = cos(Angle) * speed * (-1);
								move4->vy = sin(Angle) * speed * (-1);

								bullet4->AddComponent<OutOfScreen>();	//画面外

								auto damageSource4 = bullet4->AddComponent<DamageSource>();	//ダメージ
								damageSource4->targetName = "player";	//プレイヤーを指定

							}
							timerB -= intervalB;	//インターバル分減らす
							Deg += 5;				//角度を足す
						}
					}
					else
					{
						Angle = 0;
						Timer++;//何周回ったかを加算する
					}
				}
			}
			else
			{
				//元に戻らないこのままじゃ
				if (gameObject.y <= CenterY)
				{
					gameObject.y += MoveSpeed * deltaTime;
				}
				else
				{
					ChabgeAttack(1);
					Timer = 0;
					TakeTime = 0;
				}

			}

		}

		//ブレス攻撃
		void BressShot(GameObject& gameObject, float deltaTime)
		{
			if (Timer == 0)
			{
				intervalB = 0.01f;
				BeforeY = gameObject.y;
				if (gameObject.PinchFlg)
				{
					numberOfFire = 30;
				}
				else
				{
					numberOfFire = 50;
				}
			}
			Timer += deltaTime;
			//上に行く
			if (numberOfFire > 0)
			{
				if (Timer < 1)
				{
					gameObject.y = BeforeY + Drop(Timer) * (700 - BeforeY);
				}
				else//連射をする
				{
					if (timerB > intervalB)
					{
						//3方向に玉を打つ
						for (int i = -20; i < 40; i += 20)
						{
							speed = 500 + static_cast<float>(sin(rand() % 360)) * 100;

							auto bullet = gameObject.engine->Create<GameObject>(
								"enemy bullet", gameObject.x, gameObject.y);

							bullet->AddSprite(tc[0]);					//テクスチャ
							bullet->priority = prioEnemyBullet;			//優先順位
							bullet->AddCollision({ -10,-10,10,10 });	//当たり判定

							float disx = target->x - gameObject.x;
							float disy = target->y - gameObject.y;
							Angle = atan2(disy, disx);
							Deg = (static_cast<float>(Angle * 180) / 3.14f) + i;
							Deg += static_cast<float>(sin(rand() % 360)) * 5;
							Angle = Deg * 3.14f / 180;

							auto move = bullet->AddComponent<Move>();	//移動
							move->vx = speed * cos(Angle);
							move->vy = speed * sin(Angle);


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

				//イージングのために今の座標を入れる
				BeforeY = gameObject.y;
				TakeTime = 0;
			}
			else    //連射が終ったら、定位置に戻る
			{
				TakeTime += deltaTime;
				if (TakeTime <= 1)
				{
					gameObject.y = BeforeY + Drop(TakeTime) * (CenterY - BeforeY);
				}
				else
				{
					Timer = 0;
					TakeTime = 0;
					ChabgeAttack(2);
				}
			}

		}

		//２か所からランダムに攻撃
		void RandomShot(GameObject& gameObject, float deltaTime)
		{
			if (Timer == 0)
			{
				Deg = 0;
				if (gameObject.PinchFlg)
				{
					intervalB = 0.05f;	//連連射間隔
				}
				else
				{
					intervalB = 0.1f;	//連連射間隔
				}
				numberOfFire = 80;
			}
			Timer += deltaTime;

			if (numberOfFire > 0)
			{
				if (timerB > intervalB)
				{
					speed = 400;

					//弾を発射
					x = 100;
					y = -50;
					auto bullet = gameObject.engine->Create<GameObject>(
						"enemy bullet", gameObject.x + x, gameObject.y + y);

					bullet->AddSprite(tc[0]);					//テクスチャ
					bullet->priority = prioPlayer;		//優先順位
					bullet->AddCollision({ -10,-10,10,10 });	//当たり判定

					int angle = rand() % 360;
					angle = static_cast<int>(angle * 3.14f / 180);
					auto move = bullet->AddComponent<WallThroughMove>();	//移動
					move->vx = speed * static_cast<float>(cos(angle)) - rand() % 100;
					move->vy = speed * static_cast<float>(sin(angle)) - rand() % 100;
					move->Counter = 1;

					bullet->AddComponent<OutOfScreen>();	//画面外

					auto damageSource = bullet->AddComponent<DamageSource>();	//ダメージ
					damageSource->targetName = "player";	//プレイヤーを指定


					//弾を発射
					auto bullet2 = gameObject.engine->Create<GameObject>(
						"enemy bullet2", gameObject.x - x, gameObject.y + y);

					bullet2->AddSprite(tc[1]);					//テクスチャ
					bullet2->priority = prioPlayer;		//優先順位
					bullet2->AddCollision({ -10,-10,10,10 });	//当たり判定

					angle = rand() % 360;
					angle = static_cast<int>(angle * 3.14f / 180);
					auto move2 = bullet2->AddComponent<WallThroughMove>();	//移動
					move2->vx = speed * static_cast<float>(cos(angle)) + rand() % 100;
					move2->vy = speed * static_cast<float>(sin(angle)) + rand() % 100;
					move2->Counter = 1;

					bullet2->AddComponent<OutOfScreen>();	//画面外

					auto damageSource2 = bullet2->AddComponent<DamageSource>();	//ダメージ
					damageSource2->targetName = "player";	//プレイヤーを指定

					//連射カウンタを減らし、０以上になったら発射モード解除
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
				ChabgeAttack(3);
				Timer = 0;
				x = 0;
				y = 0;
			}

		}

		//クロススティンガー
		void CrossStinger(GameObject& gameObject, float deltaTime)
		{
			if (Timer == 0)
			{
				Angle = 0;
				BeforeX = gameObject.x;
				BeforeY = gameObject.y;
				TakeTime = 0;

				if (gameObject.PinchFlg)
				{
					numberOfFire = 25;
				}
				else
				{
					numberOfFire = 20;
				}
				intervalB = 0.1f;	//連連射間隔
			}

			Timer += deltaTime;
			if (numberOfFire > 0)
			{
				if (Timer < 1)
				{
					gameObject.y = BeforeY + Bound(Timer) * (600 - BeforeY);
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
						move->Boss = Boss;
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
						move2->Boss = Boss;
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
				BeforeY = gameObject.y;

			}
			else
			{
				TakeTime += deltaTime;
				if (TakeTime <= 1)
				{
					gameObject.y = BeforeY + Bound(TakeTime) * (CenterY - BeforeY);
				}
				else
				{
					ChabgeAttack(5);
					Timer = 0;
				}
			}

		}

		//上方向に反射弾を撃つ
		void UpBoundShot(GameObject& gameObject, float deltaTime)
		{
			if (Timer == 0)
			{
				Deg = 0;
				numberOfFire = 30;
				intervalB = 0.2f;	//連連射間隔

			}
			Timer += deltaTime;

			if (numberOfFire > 0)
			{
				if (timerB > intervalB)
				{
					if (gameObject.PinchFlg)
					{
						speed = 600;
					}
					else
					{
						speed = 400;
					}
					if (numberOfFire % 2 == 0)
					{
						auto bullet = gameObject.engine->Create<GameObject>(
							"enemy bullet", gameObject.x, gameObject.y);

						bullet->AddSprite(tc[0]);					//テクスチャ
						bullet->priority = prioEnemyBullet;			//優先順位
						bullet->AddCollision({ -10,-10,10,10 });	//当たり判定

						Angle = (static_cast<int>(rand()) % 40 + 30) * 3.14f / 180;
						auto move = bullet->AddComponent<boundMove>();	//移動
						move->vx = speed * cos(Angle);
						move->vy = speed * sin(Angle);


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

						Angle = (static_cast<int>(rand()) % 30 + 120) * 3.14f / 180;
						auto move = bullet->AddComponent<boundMove>();	//移動
						move->vx = speed * cos(Angle);
						move->vy = speed * sin(Angle);

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
				ChabgeAttack(4);
				Timer = 0;
			}


		}

		//HPが半分以下になった時の行動
		void HalfHP(GameObject& gameObject, float deltaTime)
		{
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
				Pattern = rand() % 6;
				Timer = 0;
				TakeTime = 0;
			}


		}

		//行動開始
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
				Pattern = rand() % 6;
			}

		}
};

#endif //COMPONENT_BOSSMOVE_H_INCLUDED
