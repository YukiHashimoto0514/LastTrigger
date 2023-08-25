//@file BossMove2.h

#ifndef COMPONENT_BOSSMOVE2_H_INCLUDED
#define COMPONENT_BOSSMOVE2_H_INCLUDED

#include "../../Application/Component.h"
#include "../../Application/GameObject.h"
#include <time.h>
#include "../Move/Lerp.h"

//基本を基にした一般的なボス
class BossMove2 :public Component
{
public:
	BossMove2() = default;			//コンストラクタ
	virtual ~BossMove2() = default;	//デストラクタ

	virtual void Start(GameObject& gameObject)override
	{
		Pattern = 100;
		srand((unsigned int)time(NULL));
		CenterX = 272;
		CenterY = 500;
	}

	virtual void Update(GameObject& gameObject, float deltaTime)override
	{
		if (gameObject.PinchFlg)
		{
			MoveSpeed = 100;
			speed = 300;
			if (!isOnce)
			{
				Timer = 0;
				Pattern = 50;
				isOnce = true;
			}
		}
		else
		{
			MoveSpeed = 60;
		}

		switch (Pattern)
		{
	    //横に移動する
		case 0:
			SideMoveAttack(gameObject, deltaTime);
			break;

		//上下に移動する
		case 1:
			WaveMoveAttack(gameObject, deltaTime);
			break;

		//上から3方向攻撃
		case 2:
			ThreeBurst(gameObject, deltaTime);
			break;

			//中心から渦巻攻撃
		case 3:
			EddyAttack(gameObject, deltaTime);
			break;

			
		//真ん中に戻る
		case 50:
			HalfHP(gameObject, deltaTime);

			break;

		//最初の一回のみ呼ばれる
		case 100:
			MoveStart(gameObject, deltaTime);
			break;

		}
	}



	//初期化時に設定する変数
	int Pattern = 100;//行動パターン

	//弾のテクスチャ座標
	Texcoord tc[2] = { MakeTexcoord(144, 365, 16, 16),
					   MakeTexcoord(144, 365, 16, 16)
	                 };

	float intervalA = 6;	//発射間隔
	float intervalB = 0.1f;	//連連射間隔
	int numberOfFire = 26;	//連射数
	float speed = 200;		//弾の速度
	float x = 0;			//発射位置のＸ座標
	float y = 0;			//発射位置のＹ座標
	GameObjectPtr target;	//標的

	//実行中に変化する変数
	float angle = 0;		//発射角度
	bool ShotFlg = false;	//true=撃つ　false＝待ち

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

	int ShotCnt = 0;		//何回行動するか

	//HPが半分になった時に使う変数
	float BeforeX = 0;
	float BeforeY = 0;
	bool isOnce = false;

	private:

		//攻撃パターンを変える関数
		void ChabgeAttack(int old)
		{
			while (old == Pattern)
			{
				Pattern = rand() % 4;
			}
		}

		//左右に揺れながら攻撃する
		void SideMoveAttack(GameObject& gameObject, float deltaTime)
		{
			//1回右にったら
			if (Timer == 1)
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
				if (gameObject.x <= 180)
				{
					LRMove = false;
					ShotFlg = true;
				}
				else if (gameObject.x >= 404)
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
					for (angle = 240; angle <= 300; angle += 4)
					{
						auto bullet = gameObject.engine->Create<GameObject>(
							"enemy bullet", gameObject.x, gameObject.y);

						bullet->AddSprite(tc[0]);					//テクスチャ
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
					ShotFlg = false;
				}
			}

		}

		//波を描きながら攻撃する
		void WaveMoveAttack(GameObject& gameObject, float deltaTime)
		{
			Timer += deltaTime;

			if (Timer >= 6 && abs(gameObject.x - CenterX) <= 5 && abs(gameObject.y - CenterY) <= 5)
			{
				ChabgeAttack(1);
				Timer = 0;

			}
			else
			{
				Angle += 1.0f * 3.14f / 180.0f;
				gameObject.y = gameObject.y + sin(Angle) * 0.9f;

				if (gameObject.y == 600)
				{
					ShotFlg = true;
				}

				if (gameObject.x <= 200)
				{
					LRMove = false;
					ShotFlg = true;
				}
				else if (gameObject.x >= 404)
				{
					LRMove = true;
					ShotFlg = true;
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
					for (angle = 240; angle <= 300; angle += 2)
					{
						auto bullet = gameObject.engine->Create<GameObject>(
							"enemy bullet", gameObject.x, gameObject.y);

						bullet->AddSprite(tc[0]);					//テクスチャ
						bullet->priority = prioEnemyBullet;			//優先順位
						bullet->AddCollision({ -10,-10,10,10 });	//当たり判定

						const float rad = angle * 3.14f / 180;
						auto move = bullet->AddComponent<MoveAndStop>();	//移動
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

		//上から3方向に攻撃する
		void ThreeBurst(GameObject& gameObject, float deltaTime)
		{

			//上に行く
			if (Timer < 30)
			{
				if (gameObject.y < 700)
				{
					gameObject.y += MoveSpeed * deltaTime;
				}
				else//連射をする
				{
					if (timerB > intervalB)
					{
						vx = target->x - gameObject.x;
						vy = target->y - gameObject.y;

						float rad = atan2(vy, vx);
						Angle = rad * 180 / 3.14f;
						float AngleChild = Angle;


						if (!gameObject.PinchFlg)
						{
							//3方向に玉を打つ
							for (int i = -30; i < 60; i += 30)
							{
								auto bullet = gameObject.engine->Create<GameObject>(
									"enemy bullet", gameObject.x + x, gameObject.y + y);

								bullet->AddSprite(tc[0]);					//テクスチャ
								bullet->priority = prioEnemyBullet;		//優先順位
								bullet->AddCollision({ -10,-10,10,10 });	//当たり判定

								auto move = bullet->AddComponent<Move>();	//移動
								Angle = i + AngleChild;

								rad = Angle * 3.14f / 180;
								move->vx = cos(rad) * speed;
								move->vy = sin(rad) * speed;

								bullet->AddComponent<OutOfScreen>();	//画面外

								auto damageSource = bullet->AddComponent<DamageSource>();	//ダメージ
								damageSource->targetName = "player";	//プレイヤーを指定
							}
						}
						else
						{
							//5方向に玉を打つ
							for (int i = -30; i < 45; i += 15)
							{
								auto bullet = gameObject.engine->Create<GameObject>(
									"enemy bullet", gameObject.x + x, gameObject.y + y);

								if (abs(i) == 15)
								{
									bullet->AddSprite(tc[1]);					//テクスチャ
								}
								else
								{
									bullet->AddSprite(tc[0]);				//テクスチャ
								}
								bullet->priority = prioEnemyBullet;		//優先順位
								bullet->AddCollision({ -10,-10,10,10 });	//当たり判定

								auto move = bullet->AddComponent<Move>();	//移動
								Angle = i + AngleChild;

								rad = Angle * 3.14f / 180;
								move->vx = cos(rad) * speed;
								move->vy = sin(rad) * speed;

								bullet->AddComponent<OutOfScreen>();	//画面外

								auto damageSource = bullet->AddComponent<DamageSource>();	//ダメージ
								damageSource->targetName = "player";	//プレイヤーを指定
							}

						}
						Timer++;
						timerB -= intervalB;
					}
					else
					{
						timerB += deltaTime;
					}
				}
			}

			//連射が終ったら、定位置に戻る
			if (Timer >= 30)
			{

				if (gameObject.y > CenterY)
				{
					gameObject.y -= MoveSpeed * deltaTime;
				}
				else
				{
					ChabgeAttack(2);
					Timer = 0;
				}
			}

		}
		//渦攻撃
		void EddyAttack(GameObject& gameObject, float deltaTime)
		{
			if (gameObject.PinchFlg)
			{
				ShotCnt = 2;
			}
			else
			{
				ShotCnt = 1;
			}
			if (Timer < ShotCnt)
			{
				//画面の中心に行く
				if (gameObject.y > 400)
				{
					gameObject.y -= MoveSpeed * deltaTime;
					Angle = 0;
				}
				else
				{
					if (Angle < 360)
					{
						//渦巻を巻きながら弾を発射させる
						timerB += deltaTime;//タイマーを加算
						if (timerB >= intervalB)	//タイマーがインターバルを超えたら
						{
							//弾を発射
							auto bullet = gameObject.engine->Create<GameObject>(
								"enemy bullet", gameObject.x + x, gameObject.y + y);

							bullet->AddSprite(tc[0]);					//テクスチャ
							bullet->priority = prioEnemyBullet;		//優先順位
							bullet->AddCollision({ -10,-10,10,10 });	//当たり判定

							auto move = bullet->AddComponent<Move>();	//移動
							angle = Angle * 3.14f / 180;
							move->vx = cos(angle) * speed;
							move->vy = sin(angle) * speed;

							bullet->AddComponent<OutOfScreen>();	//画面外

							auto damageSource = bullet->AddComponent<DamageSource>();	//ダメージ
							damageSource->targetName = "player";	//プレイヤーを指定

							if (gameObject.PinchFlg)
							{
								//弾を発射
								auto bullet = gameObject.engine->Create<GameObject>(
									"enemy bullet", gameObject.x + x, gameObject.y + y);

								bullet->AddSprite(tc[1]);					//テクスチャ
								bullet->priority = prioEnemyBullet;		//優先順位
								bullet->AddCollision({ -10,-10,10,10 });	//当たり判定

								auto move = bullet->AddComponent<Move>();	//移動
								angle = Angle * 3.14f / 180;
								move->vx = cos(angle) * speed * (-1);
								move->vy = sin(angle) * speed * (-1);

								bullet->AddComponent<OutOfScreen>();	//画面外

								auto damageSource = bullet->AddComponent<DamageSource>();	//ダメージ
								damageSource->targetName = "player";	//プレイヤーを指定

							}
							timerB -= intervalB;	//インターバル分減らす
							Angle += 5;				//角度を足す

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
				if (gameObject.y < CenterY)
				{
					gameObject.y += MoveSpeed * deltaTime;
				}
				else
				{
					ChabgeAttack(3);
					Timer = 0;
				}

			}

		}

		//行動開始
		void MoveStart(GameObject& gameObject, float deltaTime)
		{
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
				Pattern = 2;
			}

		}

		//HPが半分になったら
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
				Pattern = rand() % 4;
				Timer = 0;
			}

		}
};




#endif //COMPONENT_BOSSMOVE_H_INCLUDED
