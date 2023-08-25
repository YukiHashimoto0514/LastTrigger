//@file MainGameScene.cpp

#include "MainGameScene.h"
#include "../Engine/Engine.h"
#include "Debug.h"
#include "../Component/Move/Move.h"
#include "../Component/Move/MoveAndStop.h"
#include "../Component/BulletMove/WaveMove.h"
#include "../Component/BulletMove/UMove.h"
#include "../Component/BulletMove/CrossMove.h"
#include "../Component/BulletMove/boundMove.h"
#include "../Component/BulletMove/WallThroughMove.h"
#include "../Component/BulletMove/CircleLerp.h"
#include "../Component/MeshRenderer.h"
#include "../Component/Damage/HitChangeColor.h"
#include "../Component/Move/UChaseMove.h"
#include "../Component/OutOfSreen.h"
#include "../Component/Damage/DamageSource.h"
#include "../Component/HP/Health.h"
#include "../Component/Explosion/Explosion.h"
#include "../Component/Animation/PowerUpAnim.h"
#include "../Component/Explosion/MegaExplosion.h"
#include "../Component/Explosion/BigExplosion.h"
#include "../Component/Explosion/anotherExplosion.h"
#include "../Component/HP/HPMove.h"

#include "../Component/BossMove/BossMove.h"
#include "../Component/BossMove/BossMove2.h"
#include "../Component/BossMove/BossMove3.h"
#include "../Component/BossMove/BossMove4.h"
#include "../Component/BossMove/BossMove5.h"

#include "../Component/HP/HPBar.h"

#include "../Component/BulletMove/FireBullet.h"
#include "../Component/BulletMove/DiffusionBullet.h"
#include "../Component/Text.h"
#include "../Component/Item.h"
#include "../Component/Score/ScoreManager.h"
#include "../Component/Score/Score.h"
#include "../Component/AudioEvent.h"
#include "EasyAudio.h"
#include "AudioSettings.h"
#include "../Engine/SpritePriority.h"
#include "GameObject.h"
#include "TitleScene.h"
#include "GameOverScene.h"
#include "GameClearScene.h"
#include "LoadScene.h"

using namespace VecMath;

//背景スクロール用コンポーネント
class BgScroll :public Component
{
public:
	BgScroll() = default;			//コンストラクタ
	virtual ~BgScroll() = default;	//デストラクタ

	virtual void Start(GameObject& go) override
	{
		//初期化座標を記録
		initialPosition = vec3(go.x, go.y, go.z);
	}

	virtual void Update(GameObject& go, float deltaTime) override
	{
		//座標を更新
		if (offset)
		{
			const vec3 v = initialPosition + *offset;

			go.x = v.x;
			go.y = v.y;
			go.z = v.z;
		}
	}


	vec3 initialPosition = vec3(0);	//初期化座標
	std::shared_ptr<vec3> offset;	//スクロール値
};

//テクスチャ座標のリスト
const Texcoord texcoordList[] = {
	MakeTexcoord(0,0,64,64),		//大型機
	MakeTexcoord(0,111,32,32),		//小型機
	MakeTexcoord(128,160,48,48),	//隕石
	MakeTexcoord(130,64,28,48),		//変なの
	MakeTexcoord(256,0,256,128),	//ボス1
	MakeTexcoord(512,0,150,150),	//ボス3
	MakeTexcoord(670,0,140,250),	//ボス2
	MakeTexcoord(830,0,176,118),	//ボス4
	MakeTexcoord(580,270,256,128),	//ラスボス
};

//敵ファイルリスト名
const char* texcoordList2[] = {
	"Res/Model/BigEnemy.obj",		//大型機
	"Res/Model/enemy_s_01.obj",		//小型機
	"Res/Model/StarEnemy.obj",	//隕石
	"Res/Model/StarEnemy.obj",		//変なの
};


//座標(x,y)が画面外かどうか
bool IsOutOfScreen(float x, float y)
{
	const float xMin = -32;		//画面内とみなす最小のx座標
	const float xMax = 18 * 32;	//画面内とみなす最大のx座標
	const float yMin = -32;		//画面内とみなす最小のx座標
	const float yMax = 23 * 32;	//画面内とみなす最大のx座標

	return x <= xMin || x >= xMax || y <= yMin || y >= yMax;
}

//シーンを初期化する
//		@retval true  初期化成功
//		@retval false 初期化失敗

bool MainGameScene::Initialize(Engine& engine)
{

	srand((unsigned int)time(NULL));

	GLFWwindow* window	= engine.GetWindow();
	std::mt19937& rg	= engine.GetRandomGenerator();
	

	MapLevel = engine.GetMapLevel();

	//ゲームオブジェクトを削除
	engine.ClearGameObjectList();
	engine.ClearUILayers();
	
	//カメラパラメータを設定
	const vec2 viewSize = engine.GetViewSize();
	auto cameraObject = engine.GetMainCameraObject();

	cameraObject->x = viewSize.x / 2;
	cameraObject->y = viewSize.y / 2;
	cameraObject->z = 500;
	cameraObject->rotation[0] = 0;

	//ビューポートを設定
	auto cameraComponent = engine.GetMainCamera();
	const vec2 windowSize = engine.GetWindowSize();

	cameraComponent->viewport = {
		0, 0,
		static_cast<int>(viewSize.x),
		static_cast<int>(viewSize.y),
	};
	//垂直視野角を計算
	const float tanTheta = (viewSize.y * 0.5f) / cameraObject->z;
	cameraComponent->fovY = VecMath::degrees(atan(tanTheta)) * 2;

	//アスペクト比を計算
	cameraComponent->aspect = viewSize.y / viewSize.x;

	//プルームエフェクトを設定
	engine.SetBloomThreshold(0.8f);	//閾値
	engine.SetBloomStrength(8);		//大きさ

	//プレイヤーの情報を初期化
	SetPlayerStatus(engine);
	
	//マップ情報等の読み取る
	LoadResFaile(engine);

	//ボスの情報を初期化
	SetBossStatus(engine);

	//UIの設定
	SetUI(engine);

	//フォント画像
	const size_t textLayer  = engine.AddUILayer("Res/UI/font_04_2.tga", GL_NEAREST, 10);

	//必殺技を使うためのサポートキー
	const char keycode[] = "Right Shift";
	Key = engine.CreateUI<GameObject>(textLayer, "T", 105, 450);
	auto KeyName = Key->AddComponent<Text>();
	KeyName->SetText(keycode, 1.5);

	//HPテキスト
	const char key[] = "H P";
	GameObjectPtr flaze = engine.CreateUI<GameObject>(textLayer, "T", 135, 85);
	auto Name = flaze->AddComponent<Text>();
	Name->SetText(key, 2);

	//スコアマネージャーを作成
	auto uiScore = engine.CreateUI<GameObject>(textLayer, "text", 940, 600);
	scoreManager = uiScore->AddComponent<ScoreManager>();
	scoreManager->textComponent = uiScore->AddComponent<Text>();

	//強化されたとき、どの弾が撃てるかを表示する
	BulletUILoad(engine);

	//BGMを再生
	Audio::Play(AudioPlayer::bgm, BGM::stage01, 1, true);//再生に使うプレイヤー番号、音声ファイル名、音量、ループフラグ

	//ゲームの速さを戻す
	engine.SlowSpeed = 1;


	return true;	//初期化成功
}

//メインゲーム画面の状態を更新する
void MainGameScene::Update(Engine& engine, float deltaTime)
{
	LastTrigger(engine, deltaTime);
	SpawnEnemy(engine);
	UpdateBoss(engine, BossSpawn, deltaTime);
	BulletUI(engine, deltaTime);
	UpdatePlayer(engine, deltaTime);

	//背景モデルのスクロール座標を更新
	if (bgOffset)
	{
		const float scrollSpeed = 32;	//スクロール速度（ピクセル毎秒）
		const float titleSize = 32;		//タイルマップのタイルサイズ

		//スクロール終端座標
		const float bgScrollYEnd = -static_cast<float>(engine.GetBgSize().sizeY) *
			titleSize + engine.GetViewSize().y;

		//終端まで移動する
		if (bgOffset->y > bgScrollYEnd)
		{
			bgOffset->y -= scrollSpeed * deltaTime;
		}
		else
		{
			bgOffset->y = bgScrollYEnd;
		}

		//真ん中を取得
		const float bgXSize = engine.GetViewSize().x;

		//プレイヤーの位置に合わせて左右に動く
		bgOffset->x = player->x / bgXSize * 100;

	}

	//ぼかしがかかっていたら、消す
	if (engine.GetblurStrength() > 0)
	{
		engine.SetblurStrength(engine.GetblurStrength() - deltaTime * 0.3f);
	}

	uiPlHPBar->HP = player->HP;
}

//自機の状態を更新
void MainGameScene::UpdatePlayer(Engine& engine, float deltaTime)
{
	//プレイヤーのHPバーの表示
	const float NowHP = player->HP;
	const float Bar = NowHP / 5;	//割合計算
	uiPlHPBar->x = HPBarX * Bar;	//減少量分ずらす

	//自機が破壊されたら再スタート
	if (player->isDead)
	{
		restartTimer += deltaTime;
		engine.SlowSpeed = 0.5f;
		if (restartTimer >= 1)	//２秒を超えたら
		{
			if (player->isDead)
			{
				engine.SetNextScene<GameOverScene>();//メインゲームシーンにする
			}
		}
		return;
	}

	 
	//無敵時間を減らす
	if (player->UnbeatableTime > 0)
	{
		player->alpha = 0.5f;
		player->UnbeatableTime -= deltaTime;

		//メッシュレンダラーを取得
		auto& color = static_cast<MeshRenderer&>(*player->componentList[0]);
		for (int i = 0; i < color.mesh->materials.size(); i++)
		{
			//若干透明にする
			color.mesh->materials[i]->baseColor.w = 0.5f;
		}

	}
	else
	{
		player->alpha = 1;

		//メッシュレンダラーを取得
		auto& ra = static_cast<MeshRenderer&>(*player->componentList[0]);
		for (int i = 0; i < ra.mesh->materials.size(); i++)
		{
			//色と透明度を元に戻す
			ra.mesh->materials[i]->baseColor = vec4(1);
		}
	}

	//キー入力を移動方向に変換
	const float vx = static_cast<float>(
		engine.GetKey(GLFW_KEY_D) - engine.GetKey(GLFW_KEY_A));

	const float vy = static_cast<float>(
		engine.GetKey(GLFW_KEY_W) - engine.GetKey(GLFW_KEY_S));


	//移動方向の長さが０以上なら、入力があったとみなす(０徐除算を回避するため)
	const float v = std::sqrt(vx * vx + vy * vy);//移動方向の長さｖを求める

	if (v > 0)
	{
		//自機の座標を更新
		const float playerSpeed = 300 * deltaTime / v;
		player->x += vx * playerSpeed * triggerplspeed;
		player->y += vy * playerSpeed * triggerplspeed;
	}

	//自機が画面外に行かないように値を制限する
	const float viewSizeX = 17 * 32;
	const float viewSizeY = 22 * 32;
	player->x = std::clamp(player->x, 16.0f, viewSizeX - 16.0f);
	player->y = std::clamp(player->y, 24.0f, viewSizeY - 24.0f);
    
	//自機を傾ける
	player->rotation[1] = vx * radians(30.0f);

	//弾の画像配列
	const Texcoord tcBullet[] = { MakeTexcoord(0, 365, 16, 16),//緑
								  MakeTexcoord(16, 365, 16, 16),//青
								  MakeTexcoord(32, 365, 16, 16),//赤
								  MakeTexcoord(48, 365, 16, 16),//黄色
								  MakeTexcoord(64, 365, 16, 16), //変なの
								  MakeTexcoord(112, 365, 16, 16), //紫
								  MakeTexcoord(128, 365, 16, 16), //ピンク
								};

	//弾を発射
	const bool shotKey = engine.GetKey(GLFW_KEY_SPACE);

	if (shotKey)
	{
		shotTimer -= deltaTime;				//キーが押されている間タイマーを減らす
		if(!TriggerFlg)TriggerGauge += deltaTime * 5;		//発動していなかったら、ゲージを加算する

		if (shotTimer <= 0)
		{
			if (ShotNumber == 0|| TriggerTime > 0)
			{
				//弾のゲームオブジェクトを作成
				GameObjectPtr bullet = engine.Create<GameObject>(
					"bullet", player->x, player->y);
				bullet->priority = prioPlayerBullet;	//表示優先順位
				bullet->AddSprite(tcBullet[1]);	//スプライトデータに弾を追加
				bullet->AddCollision({ -10,-10,10,10 });	//当たり判定

				auto move = bullet->AddComponent<Move>();	//弾の移動
				move->vy = 600;
				bullet->AddComponent<OutOfScreen>();		//画面外判定

				auto damageSource = bullet->AddComponent<DamageSource>();	//ダメージ源を追加
				damageSource->targetName = "enemy";	//enemyにダメージを与える

				
				//もしも強化アイテムを拾っていたら
				if (player->Power[0] > 1)
				{
					move->vy += player->Power[0] * 20;

					//必殺技が発動していない場合に限る
					if (TriggerTime <= 0)
					{
						shotTimer -= player->Power[0] * 0.1f;
					}
				}
			}

			if (ShotNumber == 1 || TriggerTime > 0)//二股に別れるやつ
			{
				if (player->Power[1] > 0)
				{
					GameObjectPtr horizon = engine.Create<GameObject>(
						"bullet", player->x, player->y);

					horizon->priority = prioPlayerBullet;			//表示優先順位
					horizon->AddSprite(tcBullet[3]);				//スプライトデータに弾を追加
					horizon->AddCollision({ -10,-10,10,10 });		//当たり判定
					auto move2 = horizon->AddComponent<UMove>();//弾の移動
					move2->vx = 200;
					move2->vy = 900;


					horizon->AddComponent<OutOfScreen>();		//画面外判定

					auto damageSource2 = horizon->AddComponent<DamageSource>();	//ダメージ源を追加
					damageSource2->targetName = "enemy";	//enemyにダメージを与える

					GameObjectPtr horizon2 = engine.Create<GameObject>(
						"bullet", player->x, player->y);

					horizon2->priority = prioPlayerBullet;			//表示優先順位
					horizon2->AddSprite(tcBullet[3]);				//スプライトデータに弾を追加
					horizon2->AddCollision({ -10,-10,10,10 });		//当たり判定
					auto move = horizon2->AddComponent<UMove>();//弾の移動
					move->vx = -200;
					move->vy = 900;


					horizon2->AddComponent<OutOfScreen>();		//画面外判定

					auto damageSource3 = horizon2->AddComponent<DamageSource>();	//ダメージ源を追加
					damageSource3->targetName = "enemy";	//enemyにダメージを与える

					//強化されるほど、弾が増える
					if (player->Power[1] > 1)
					{
						for (int i = 0; i < player->Power[1]; i++)
						{
							GameObjectPtr horizon4 = engine.Create<GameObject>(
								"bullet", player->x, player->y);

							horizon4->priority = prioPlayerBullet;			//表示優先順位
							horizon4->AddSprite(tcBullet[3]);				//スプライトデータに弾を追加
							horizon4->AddCollision({ -10,-10,10,10 });		//当たり判定
							auto move6 = horizon4->AddComponent<UMove>();//弾の移動
							move6->vx = 400.0f + i * 200.0f;
							move6->vy = 900;


							horizon4->AddComponent<OutOfScreen>();		//画面外判定

							auto damageSource7 = horizon4->AddComponent<DamageSource>();	//ダメージ源を追加
							damageSource7->targetName = "enemy";	//enemyにダメージを与える

							GameObjectPtr horizon9 = engine.Create<GameObject>(
								"bullet", player->x, player->y);

							horizon9->priority = prioPlayerBullet;			//表示優先順位
							horizon9->AddSprite(tcBullet[3]);				//スプライトデータに弾を追加
							horizon9->AddCollision({ -10,-10,10,10 });		//当たり判定
							auto move10 = horizon9->AddComponent<UMove>();//弾の移動
							move10->vx = -400.0f - i * 200.0f;
							move10->vy = 900;

							horizon9->AddComponent<OutOfScreen>();		//画面外判定

							auto damageSource4 = horizon9->AddComponent<DamageSource>();	//ダメージ源を追加
							damageSource4->targetName = "enemy";	//enemyにダメージを与える
						}
					}
				}
			}

			if (ShotNumber == 2 || TriggerTime > 0)//うねうねするやつ
			{
				if (player->Power[2] > 0)
				{
					GameObjectPtr bullet2 = engine.Create<GameObject>(
						"bullet", player->x, player->y);
					bullet2->priority = prioPlayerBullet;	//表示優先順位
					bullet2->AddSprite(tcBullet[2]);	//スプライトデータに弾を追加
					bullet2->AddCollision({ -10,-10,10,10 });	//当たり判定

					auto move4 = bullet2->AddComponent<WaveMove>();	//弾の移動
					move4->vx = 200;	//弾の速さ
					move4->vy = 700;	//弾の速さ

					bullet2->AddComponent<OutOfScreen>();		//画面外判定

					auto damageSource2 = bullet2->AddComponent<DamageSource>();	//ダメージ源を追加
					damageSource2->targetName = "enemy";	//enemyにダメージを与える


					GameObjectPtr bullet3 = engine.Create<GameObject>(
						"bullet", player->x, player->y);
					bullet3->priority = prioPlayerBullet;	//表示優先順位
					bullet3->AddSprite(tcBullet[2]);	//スプライトデータに弾を追加
					bullet3->AddCollision({ -10,-10,10,10 });	//当たり判定

					auto move5 = bullet3->AddComponent<WaveMove>();	//弾の移動
					move5->vx = -200;	//弾の速さ
					move5->vy = 700;	//弾の速さ

					bullet3->AddComponent<OutOfScreen>();		//画面外判定

					auto damageSource3 = bullet3->AddComponent<DamageSource>();	//ダメージ源を追加
					damageSource3->targetName = "enemy";	//enemyにダメージを与える

					//必殺技が発動していない場合に限る
					if (TriggerTime <= 0)
					{
						//強化すればするほど射撃間隔が狭まる
						if (player->Power[2] > 1)shotTimer -= player->Power[2] * 0.1f;
					}
				}
			}

			if (ShotNumber == 3|| TriggerTime > 0)//壁で跳ねるやつ
			{
				if (player->Power[3] > 0)
				{
					GameObjectPtr horizon = engine.Create<GameObject>(
						"bullet", player->x, player->y);

					horizon->priority = prioPlayerBullet;			//表示優先順位
					horizon->AddSprite(tcBullet[0]);				//スプライトデータに弾を追加
					horizon->AddCollision({ -10,-10,10,10 });		//当たり判定
					auto move2 = horizon->AddComponent<boundMove>();//弾の移動

					float angle = (static_cast<float>(rand() % 60) + 60.0f) * 3.14f / 180;
					move2->vx = 500 * cos(angle);
					move2->vy = -800;	//後ろから出る


					horizon->AddComponent<OutOfScreen>();		//画面外判定

					auto damageSource2 = horizon->AddComponent<DamageSource>();	//ダメージ源を追加
					damageSource2->targetName = "enemy";	//enemyにダメージを与える

					//強化されるほど、後ろから弾がでる
					if (player->Power[3] > 1)
					{
						for (int i = 0; i < player->Power[3]; i++)
						{
							GameObjectPtr horizon2 = engine.Create<GameObject>(
								"bullet", player->x, player->y);

							horizon2->priority = prioPlayerBullet;			//表示優先順位
							horizon2->AddSprite(tcBullet[0]);				//スプライトデータに弾を追加
							horizon2->AddCollision({ -10,-10,10,10 });		//当たり判定
							auto move9 = horizon2->AddComponent<boundMove>();//弾の移動
							float angle = (rand() % 180) * 3.14f / 180;
							move9->vx = 500 * cos(angle);
							move9->vy = -800;


							horizon2->AddComponent<OutOfScreen>();		//画面外判定

							auto damageSource2 = horizon2->AddComponent<DamageSource>();	//ダメージ源を追加
							damageSource2->targetName = "enemy";	//enemyにダメージを与える
						}
					}
				}
			}

			if (ShotNumber == 4 || TriggerTime > 0)//反対側から出てくるやつ
			{
				if (player->Power[4] > 0)
				{
					GameObjectPtr horizon = engine.Create<GameObject>(
						"bullet", player->x, player->y);

					horizon->priority = prioPlayerBullet;			//表示優先順位
					horizon->AddSprite(tcBullet[5]);				//スプライトデータに弾を追加
					horizon->AddCollision({ -10,-10,10,10 });		//当たり判定
					auto move2 = horizon->AddComponent<WallThroughMove>();//弾の移動
					float angle = (rand() % 180) * 3.14f / 180;
					move2->vx = 500;
					move2->vy = 900 * sin(angle);//横からでてくる


					horizon->AddComponent<OutOfScreen>();		//画面外判定

					auto damageSource2 = horizon->AddComponent<DamageSource>();	//ダメージ源を追加
					damageSource2->targetName = "enemy";	//enemyにダメージを与える

					//強化されると横からでる
					if (player->Power[4] > 1)
					{
						for (int i = 0; i < player->Power[4]; i++)
						{
							//偶数発目なら、右向きにでる
							if (i % 2 == 0)
							{
								GameObjectPtr horizon = engine.Create<GameObject>(
									"bullet", player->x, player->y);

								horizon->priority = prioPlayerBullet;			//表示優先順位
								horizon->AddSprite(tcBullet[5]);				//スプライトデータに弾を追加
								horizon->AddCollision({ -10,-10,10,10 });		//当たり判定
								auto move2 = horizon->AddComponent<WallThroughMove>();//弾の移動
								float angle = (rand() % 180) * 3.14f / 180;
								move2->vx = 500;
								move2->vy = 900 * sin(angle);

								horizon->AddComponent<OutOfScreen>();		//画面外判定

								auto damageSource2 = horizon->AddComponent<DamageSource>();	//ダメージ源を追加
								damageSource2->targetName = "enemy";	//enemyにダメージを与える
							}
							else//奇数目なら左から
							{
								GameObjectPtr horizon = engine.Create<GameObject>(
									"bullet", player->x, player->y);

								horizon->priority = prioPlayerBullet;			//表示優先順位
								horizon->AddSprite(tcBullet[5]);				//スプライトデータに弾を追加
								horizon->AddCollision({ -10,-10,10,10 });		//当たり判定
								auto move2 = horizon->AddComponent<WallThroughMove>();//弾の移動
								float angle = (rand() % 180) * 3.14f / 180;
								move2->vx = -500;
								move2->vy = 900 * sin(angle);

								horizon->AddComponent<OutOfScreen>();		//画面外判定

								auto damageSource2 = horizon->AddComponent<DamageSource>();	//ダメージ源を追加
								damageSource2->targetName = "enemy";	//enemyにダメージを与える

							}
						}
					}


				}
			}

			Audio::PlayOneShot(SE::playerShot, 0.1f);	//効果音を再生

			if (TriggerTime <= 0)
			{
				shotTimer += 0.46f;				//発射間隔を設定
			}
			else
			{
				shotTimer += 0.16f;				//発射間隔を設定
			}
		}
	}
	else
	{
		//次にキーが押されたら、すぐ弾が発射される(連打すると連射)
		shotTimer = 0;
	}
}

//敵を出現させる
void MainGameScene::SpawnEnemy(Engine& engine)
{
	//スクロール値を取得
	auto [sx, sy, bgBaseX, bgBaseY] = engine.GetBgSize();

	if (bgBaseY >= 0)
	{
		return;//配置データの範囲外なので何もしない
	}

	//画面上部（天井）ラインの位置を計算
	const int startY = static_cast<int>(enemyMap.sizeY) - 23 * 2;
	int ceilY = startY + static_cast<int>(bgBaseY / 32 * 2);

	if (ceilY < 0)
	{
		ceilY = 0;//範囲外アクセスを避けるため、０を最小値とする
	}

	//敵出現ライン～天井ライン間の配置データに従って敵を出現させる

	for (; spawningY > ceilY; --spawningY)
	{
		//横一列分の配置データをチェック
		for (size_t x = 0; x < enemyMap.sizeX; ++x)
		{
			//配置データから配置する敵番号を取得
			const size_t i = spawningY * enemyMap.sizeX + x;
			const int enemyId = enemyMap.data[i];

			//敵番号がテクスチャ座標配列の範囲内の場合、敵を出現させる
			if (enemyId >= 0 && enemyId < std::size(texcoordList))
			{
				//実際の敵の出現位置に敵を配置（配置データは画像右下を指している）
				//画像サイズ＝1024＊1024 タイルサイズ＝32*32
				const Texcoord& tc = texcoordList[enemyId];
				const char* tc2 = texcoordList2[enemyId];

				//ボスだったら
				if (enemyId == 4)//ボス１
				{ 
					auto Move = boss->AddComponent<BossMove2>();//弾を発射
					Move->target = player;


					BossSpawn = true;//ボスを出現させる
				}

				if (enemyId == 5)//ぼす３反射
				{
					auto Move = boss->AddComponent<BossMove4>();//弾を発射
					Move->target = player;
					
					BossSpawn = true;//ボスを出現させる

				}

				if (enemyId == 6)//ボス２ ランダム
				{

					auto Move = boss->AddComponent<BossMove3>();//弾を発射
					Move->target = player;



					BossSpawn = true;//ボスを出現させる

				}

				if (enemyId == 7)//ボス4 狙撃
				{

					auto Move = boss->AddComponent<BossMove5>();//弾を発射
					Move->target = player;
					Move->boss = boss;

					BossSpawn = true;//ボスを出現させる

				}

				if (enemyId == 8)//ラスボス
				{

					auto Move = boss->AddComponent<BossMove>();//弾を発射
					Move->target = player;
					Move->Boss = boss;

					BossSpawn = true;//ボスを出現させる

				}

				if(enemyId==0|| enemyId == 1|| enemyId == 2|| enemyId == 3)//雑魚の場合
				{
					//敵のゲームオブジェクトを作成
					GameObjectPtr enemy = engine.Create<GameObject>(
						"enemy",
						static_cast<float>(x * 32 + tc.sx * 1024 / 2),
						static_cast<float>(21 * 32 + tc.sy * 1024 / 2));
					enemy->priority = prioEnemy;	//表示優先順位
					//enemy->AddSprite(tc);	//スプライトデータに追加


					//３Ⅾモデル描画
					auto enemyrender = enemy->AddComponent<MeshRenderer>();
					enemyrender->mesh = engine.LoadOBJ(tc2);

					//個別に色を変えられるように、マテリアルのコピーを作る
					for (int j = 0; j < enemyrender->mesh->materials.size(); j++)
					{
						enemyrender->materials.push_back(
							std::make_shared<Mesh::Material>(*enemyrender->mesh->materials[j]));
					}

					const float collisionSize = 1024 * 0.4f;
					enemy->AddCollision({
						-tc.sx * collisionSize,-tc.sy * collisionSize,
						tc.sx * collisionSize,tc.sy * collisionSize });
					auto health = enemy->AddComponent<Health>();			//HP
					auto audio = enemy->AddComponent<AudioEvent>();			//音声再生コンポーネントを設定
					auto score = enemy->AddComponent<Score>();				//スコアコンポーネントを設定
					score->scoreManager = scoreManager;
					auto color = enemy->AddComponent<HitChangeColor>();

					if (enemyId == 0)		//大型種
					{
						if (MapLevel == 0)
						{
							auto move = enemy->AddComponent<MoveAndStop>();				//移動
							move->vy = -50;			//移動値
							enemy->HP = 5;		//HP
						}
						else if (MapLevel == 1)
						{
							auto move = enemy->AddComponent<MoveAndStop>();				//移動
							move->vy = -50;			//移動値
							enemy->HP = 5;		//HP
						}
						else if (MapLevel == 2)
						{
							auto move = enemy->AddComponent<MoveAndStop>();				//移動
							move->vy = -70;			//移動値
							enemy->HP = 7;		//HP
						}
						else if (MapLevel == 3)
						{
							auto move = enemy->AddComponent<MoveAndStop>();				//移動
							move->vy = -70;			//移動値
							enemy->HP = 7;		//HP
						}
						else if (MapLevel == 4)
						{
							auto move = enemy->AddComponent<MoveAndStop>();				//移動
							move->vy = -80;			//移動値
							enemy->HP = 8;		//HP
						}
						auto explosion = enemy->AddComponent<MegaExplosion>();		//爆発
						explosion->count = 3;	//爆発個数
						explosion->scale = 2;
						enemy->Point = 3;		//スコア倍率
						audio->files.onDestory = SE::enemyExplosionL;	//爆発効果音
						auto fireBullet2 = enemy->AddComponent<DiffusionBullet>();//弾を発射

						enemyrender->scale = vec3(48);			//3Dモデルのサイズ調整
						enemyrender->rotation.x = radians(90);	//向きを調整

					}
					else if (enemyId == 1)	//小型種
					{

						auto fireBullet = enemy->AddComponent<FireBullet>();//弾を発射
						fireBullet->target = player;	//プレイヤーに向けて撃つ
						fireBullet->intervalA = 1;		//最初の弾を出現後に発射させる

						enemyrender->scale = vec3(16);			//3Dモデルのサイズ調整
						enemyrender->rotation.x = radians(-90);	//向きを調整



						if (MapLevel == 0)
						{
							auto move = enemy->AddComponent<Move>();				//移動
							move->vy = -100;	//移動値
							fireBullet->numberOfFire = 2;
							fireBullet->intervalB = 0.1f;		//最初の弾を出現後に発射させる
						}
						else if (MapLevel == 1)
						{
							auto move = enemy->AddComponent<Move>();				//移動
							move->vy = -120;	//移動値

							fireBullet->numberOfFire = 3;
							fireBullet->intervalB = 0.15f;		//最初の弾を出現後に発射させる
						}
						else if (MapLevel == 2)
						{
							auto move = enemy->AddComponent<Move>();				//移動
							move->vy = -140;	//移動値
							fireBullet->numberOfFire = 3;
							fireBullet->intervalB = 0.15f;		//最初の弾を出現後に発射させる
						}
						else if (MapLevel == 3)
						{
							auto move = enemy->AddComponent<Move>();				//移動
							move->vy = -140;	//移動値
							fireBullet->numberOfFire = 4;
							fireBullet->intervalB = 0.19f;		//最初の弾を出現後に発射させる

						}
						else if (MapLevel == 4)
						{
							auto move = enemy->AddComponent<Move>();				//移動
							move->vy = -150;	//移動値

							fireBullet->numberOfFire = 4;
							fireBullet->intervalB = 0.2f;		//最初の弾を出現後に発射させる

						}

						auto explosion = enemy->AddComponent<Explosion>();		//爆発
						enemy->HP = 1;		//HP
						explosion->count = 5;	//爆発個数
						explosion->scale = 3;
						enemy->Point = 1;		//スコア倍率
						audio->files.onDestory = SE::enemyExplosionS;	//爆発効果音
					}
					else if (enemyId == 3)	//中型機
					{
					enemyrender->scale = vec3(5);			//3Dモデルのサイズ調整
					enemyrender->rotation.x = radians(90);	//向きを調整
					
						if (MapLevel == 0)
						{
							auto move = enemy->AddComponent<UChaseMove>();				//移動
							move->vy = -150;
							move->vx = 170;
							move->target = player;
							enemy->HP = 3;		//HP
						}
						else if (MapLevel == 1)
						{
							auto move = enemy->AddComponent<UChaseMove>();				//移動
							move->vy = -150;
							move->vx = 200;
							move->target = player;
							enemy->HP = 3;		//HP

						}
						else if (MapLevel == 2)
						{
							auto move = enemy->AddComponent<UChaseMove>();				//移動
							move->vy = -180;
							move->vx = 200;
							move->target = player;
							enemy->HP = 3;		//HP
						}
						else if (MapLevel == 3)
						{
							auto move = enemy->AddComponent<UChaseMove>();				//移動
							move->vy = -180;
							move->vx = 200;
							move->target = player;
							enemy->HP = 3;		//HP

						}
						else if (MapLevel == 4)
						{
							auto move = enemy->AddComponent<UChaseMove>();				//移動
							move->vy = -190;
							move->vx = 220;
							move->target = player;
							enemy->HP = 3;		//HP

						}
						auto explosion = enemy->AddComponent<BigExplosion>();		//爆発
						explosion->count = 3;	//爆発個数
						explosion->scale = 3;
						enemy->Point = 2;		//スコア倍率

						audio->files.onDestory = SE::enemyExplosionM;	//爆発効果音
					}

					auto damageSource = enemy->AddComponent<DamageSource>();//ダメージ
					damageSource->targetName = "player";	//名指し

					enemy->AddComponent<OutOfScreen>();//画面外判定


					int randam = 0;	//乱数
					int make = 0;	//乱数
					
					make = rand() % 100;
					//50/1の確率で発生
					if (make <= 20)
					{
						randam = rand() * rand() % 3;	//画像をランダムで決定

						//アイテムゲームオブジェトの生成
						GameObjectPtr ITEM = engine.Create<GameObject>(
							"item", enemy->x, enemy->y);

						ITEM->AddComponent<Item>();				//アイテム
						ITEM->AddCollision({ -15,-15,15,15 });	//当たり判定
						ITEM->priority = prioItem;				//優先順位
						ITEM->AddComponent<OutOfScreen>();		//画面外まで流れた場合
					}
				}
			}
		}
	}
}

void MainGameScene::UpdateBoss(Engine& engine, bool Spawnflg, float deltaTime)
{
	//メッシュレンダラーを取得
	auto& color = static_cast<MeshRenderer&>(*boss->componentList[0]);

	if (color.mesh->materials[0]->baseColor.x > 1)
	{
		for (int i = 0; i < color.mesh->materials.size(); i++)
		{
			//色をもとに戻す
			color.mesh->materials[i]->baseColor.x -= deltaTime * 100;
		}
	}
	else
	{
		for (int i = 0; i < color.mesh->materials.size(); i++)
		{
			//色をもとに戻す
			color.mesh->materials[i]->baseColor = vec4(1);
		}
	}

	//ボスが倒されたら次のレベルへ
	if (boss->isDead)
	{
		player->UnbeatableTime = 100;	//プレイヤーを無敵にしておく
		restartTimer += deltaTime;		//経過時間を計測
		engine.SlowSpeed = 0.4f;			//ゆっくりにして演出を楽しんでもらう
		if (restartTimer >= 1.5)		//２秒を超えたら
		{
			if (boss->isDead)
			{
				MapLevel++;
				if (MapLevel < 5)
				{
					engine.SetMapLevel(MapLevel);			//次のマップをセット
					engine.SetNextScene<LoadScene>();		//ロードシーンにする
				}
				else
				{
					engine.SetNextScene<GameClearScene>();	 //メインゲームシーンにする
				}
			}
		}
		return;
	}

}

void MainGameScene::LastTrigger(Engine &engine,float deltaTime)
{
	//ゲージのたまり具合によって、UIのゲージもためる
	uiTriggerGauge->y = TriggerGauge + 150;

	//Tキーを押したらラストトリガー発動
	TriggerFlg = engine.GetKey(GLFW_KEY_RIGHT_SHIFT);


	//ラストトリガーを発動させたら
	if (TriggerFlg)
	{
		//ゲージが少しでもたまっていたら発動可能
		if (TriggerGauge > 0) 
		{
			Audio::PlayOneShot(SE::lasttrigger);
			TriggerTime = TriggerGauge;		//効果時間を設定
			TriggerFlg = false;				//通常に戻す
		}
	}

	//効果時間があったら
	if (TriggerTime > 0)
	{
		engine.SlowSpeed = 0.4f;			//世界をゆっくりにする
		TriggerTime -= deltaTime * 55;	//効果時間を元の時間基準で減らす
		TriggerGauge = TriggerTime;		//ゲージを時間とともに減らす
		triggerplspeed = 1.1f;			//プレイヤーの移動速度を上げる

		//青光るようにする
		uiTriggerGauge->Red   = 0.3f;
		uiTriggerGauge->Green = 2;
		uiTriggerGauge->Blue  = static_cast<float>(rand() % 9);

		//画面をぼかす
		engine.SetblurStrength(0.5f);
		engine.SetBloomStrength(15);
	}
	else//いつも通りなら
	{
		TriggerTime = 0;	//効果時間を０にする
		engine.SlowSpeed = 1;		//いつも通りの時間にする
		triggerplspeed = 1; //いつもの速度に戻す

		//いつもの色に戻す
		uiTriggerGauge->Red = 1;
		uiTriggerGauge->Green = 1;
		uiTriggerGauge->Blue = 1;

		//いつもの色に戻す
		Key->Red = 1;
		Key->Green = 1;
		Key->Blue = 1;

		engine.SetBloomStrength(8);


		//ゲージが満タンじゃないなら
		if (TriggerGauge < 300)
		{
			if (!TriggerFlg)TriggerGauge += deltaTime * 4;//発動していないなら、ゲージをためる
		}
		else//満タンなら
		{
			TriggerGauge = 300;//ゲージを最大値で固定する

			//青っぽく点滅する
			uiTriggerGauge->Red   = static_cast<float>(rand() % 9);
			uiTriggerGauge->Green = static_cast<float>(rand() % 2);
			uiTriggerGauge->Blue  = static_cast<float>(rand() % 9);

			//虹色に光らせる
			Key->Red   = static_cast<float>(rand() % 9);
			Key->Green = static_cast<float>(rand() % 9);
			Key->Blue  = static_cast<float>(rand() % 9);
		}
	}
}

void MainGameScene::BulletUI(Engine& engine, float deltaTime)
{

	//矢印キーを押したら弾を切り替える
	if (coolTime > 0)
	{
		coolTime -= deltaTime * 3;
	}

	//↑矢印を押したら
	static float chagetime = 0.1f;	//切り替え時のクールタイムをリセットする
	const bool Uparrow = engine.GetKey(GLFW_KEY_UP);
	if (Uparrow)
	{
		shotTimer = chagetime;	//クールタイムをセットする
		if (coolTime <= 0)
		{
			//上まで行ったら下に行く
			if (ShotNumber != 0)
			{
				ShotNumber--;
				coolTime = 1;
				uiSelect->y += 100;
			}
			else
			{
				coolTime = 1;
				ShotNumber = 4;
				uiSelect->y = 200;
			}
		}
	}

	//下↓を押したら
	const bool Downarrow = engine.GetKey(GLFW_KEY_DOWN);
	if (Downarrow)
	{
		shotTimer = chagetime;	//クールタイムをセットする
		if (coolTime <= 0)
		{
			//一番下まで行ったら上に帰ってくる
			if (ShotNumber != 4)
			{
				ShotNumber++;
				coolTime = 1;
				uiSelect->y -= 100;
			}
			else
			{
				coolTime = 1;
				ShotNumber = 0;
				uiSelect->y = 600;
			}
		}
	}

	//非選択状態で半透明にする
	switch (ShotNumber)
	{
	case 0:
		uiBullet_[0]->alpha = 1.0f;
		uiBullet_[1]->alpha = 0.2f;
		uiBullet_[2]->alpha = 0.2f;
		uiBullet_[3]->alpha = 0.2f;
		uiBullet_[4]->alpha = 0.2f;
		break;
	case 1:
		uiBullet_[0]->alpha = 0.2f;
		uiBullet_[1]->alpha = 1.0f;
		uiBullet_[2]->alpha = 0.2f;
		uiBullet_[3]->alpha = 0.2f;
		uiBullet_[4]->alpha = 0.2f;
		break;
	case 2:
		uiBullet_[0]->alpha = 0.2f;
		uiBullet_[1]->alpha = 0.2f;
		uiBullet_[2]->alpha = 1.0f;
		uiBullet_[3]->alpha = 0.2f;
		uiBullet_[4]->alpha = 0.2f;
		break;
	case 3:
		uiBullet_[0]->alpha = 0.2f;
		uiBullet_[1]->alpha = 0.2f;
		uiBullet_[2]->alpha = 0.2f;
		uiBullet_[3]->alpha = 1.0f;
		uiBullet_[4]->alpha = 0.2f;
		break;
	case 4:
		uiBullet_[0]->alpha = 0.2f;
		uiBullet_[1]->alpha = 0.2f;
		uiBullet_[2]->alpha = 0.2f;
		uiBullet_[3]->alpha = 0.2f;
		uiBullet_[4]->alpha = 1.0f;
		break;
	default:
		break;
	}



	//獲得していなかったら非表示にする
	if (player->Power[1] <= 0)
	{
		uiBullet_[1]->alpha = 0;
	}

	if (player->Power[2] <= 0)
	{
		uiBullet_[2]->alpha = 0;
	}

	if (player->Power[3] <= 0)
	{
		uiBullet_[3]->alpha = 0;
	}

	if (player->Power[4] <= 0)
	{
		uiBullet_[4]->alpha = 0;
	}
}

void MainGameScene::BulletUILoad(Engine& engine)
{
	Bullet_[0] = engine.AddUILayer("Res/Bullet/Bullet_10.tga", GL_LINEAR, 10);
	uiBullet_[0] = engine.CreateUI<GameObject>(Bullet_[0], "Bullet_1", 50, 600);
	uiBullet_[0]->spriteList.clear();
	uiBullet_[0]->AddSprite({ 0,0,1,1 });

	Bullet_[1] = engine.AddUILayer("Res/Bullet/Bullet_3.tga", GL_LINEAR, 10);
	uiBullet_[1] = engine.CreateUI<GameObject>(Bullet_[1], "Bullet_1", 50, 500);
	uiBullet_[1]->spriteList.clear();
	uiBullet_[1]->AddSprite({ 0,0,1,1 });
	uiBullet_[1]->alpha = 0;

	Bullet_[2] = engine.AddUILayer("Res/Bullet/Bullet_2.tga", GL_LINEAR, 10);
	uiBullet_[2] = engine.CreateUI<GameObject>(Bullet_[2], "Bullet_1", 50, 400);
	uiBullet_[2]->spriteList.clear();
	uiBullet_[2]->AddSprite({ 0,0,1,1 });
	uiBullet_[2]->alpha = 0;

	Bullet_[3] = engine.AddUILayer("Res/Bullet/Bullet_1.tga", GL_LINEAR, 10);
	uiBullet_[3] = engine.CreateUI<GameObject>(Bullet_[3], "Bullet_1", 50, 300);
	uiBullet_[3]->spriteList.clear();
	uiBullet_[3]->AddSprite({ 0,0,1,1 });
	uiBullet_[3]->alpha = 0;

	Bullet_[4] = engine.AddUILayer("Res/Bullet/Bullet_4.tga", GL_LINEAR, 10);
	uiBullet_[4] = engine.CreateUI<GameObject>(Bullet_[4], "Bullet_1", 50, 200);
	uiBullet_[4]->spriteList.clear();
	uiBullet_[4]->AddSprite({ 0,0,1,1 });
	uiBullet_[4]->alpha = 0;

	SelectFrame= engine.AddUILayer("Res/UI/Selectframe.tga", GL_LINEAR, 10);
	uiSelect = engine.CreateUI<GameObject>(SelectFrame, "select", 50, 600);
	uiSelect->AddSprite({ 0,0,1,1 });

}

void MainGameScene::SetBossStatus(Engine& engine)
{
	//ボスの追加
	boss = engine.Create<GameObject>("enemy", 272, 1300);	//初期リス
	boss->priority = prioPlayer;			//表示優先順位

	auto render = boss->AddComponent<MeshRenderer>();//3Dモデル



	auto bosshealth = boss->AddComponent<Health>();			//HP
	boss->HP = 500;

	auto bossscore = boss->AddComponent<Score>();				//スコアコンポーネントを設定
	boss->Point = 6;
	bossscore->scoreManager = scoreManager;

	auto bossexplosion = boss->AddComponent<MegaExplosion>();	//爆発
	bossexplosion->count = 5;			//爆発個数を増やす
	bossexplosion->scale = 5;

	auto audio = boss->AddComponent<AudioEvent>();
	audio->files.onDestory = SE::BossExplosion;	//爆発効果音

	auto bossbar = boss->AddComponent<HPBar>();			//HPバーコンポーネント


	if (MapLevel == 0)//基本のステージ
	{
		boss->AddCollision({ -90,-20,95,20 });	//当たり判定
		render->mesh = engine.LoadOBJ("Res/Model/Boss_1.obj");
		render->scale = vec3(15);
		render->rotation.x = 90;

	}
	else if (MapLevel == 1)//ランダム
	{
		boss->AddCollision({ -70,-20,75,40 });	//当たり判定
		render->mesh = engine.LoadOBJ("Res/Model/Boss_2.obj");
		render->scale = vec3(4);
		boss->rotation[0] = 90;
	}
	else if (MapLevel == 2)//反射
	{
		//boss->HP = 200;

		render->mesh = engine.LoadOBJ("Res/Model/Boss_3.obj");
		render->scale = vec3(19);
		boss->rotation[0] = 90;

		boss->AddCollision({ -65,-20,75,30 });	//当たり判定
	}
	else if (MapLevel == 3)//狙い
	{
		boss->AddCollision({ -90,-30,90,30 });	//当たり判定

		render->mesh = engine.LoadOBJ("Res/Model/Boss_5.obj");
		render->scale = vec3(16);
		boss->rotation[0] = 90;

	}
	else if (MapLevel == 4)//ラスボス
	{
		boss->AddCollision({ -90,-20,95,20 });	//当たり判定
		render->mesh = engine.LoadOBJ("Res/Model/Boss_1.obj");
		render->scale = vec3(15);
		render->rotation.x = 90;

		boss->AddCollision({ -90,-20,95,20 });	//当たり判定

	}
	bosshealth->MaxHealth = boss->HP;
	bossbar->MaxHP = bosshealth->MaxHealth;		//最大HPを設定
	bossbar->NowHP = bosshealth->MaxHealth;		//現在のHPを設定

	BossSpawn = false;	//ボスを非表示に

}

void MainGameScene::SetPlayerStatus(Engine& engine)
{
	const auto [bgSizeX, bgSizeY, bgBaseX, bgBaseY] = engine.GetBgSize();

	//自機を追加
	player = engine.Create<GameObject>("player", static_cast<float>(bgSizeX) * 32 / 2, 64);	//初期リス
	player->priority = prioPlayer;						//表示優先順位
	player->AddCollision({ -3,-4,3,4 });				//当たり判定

	//3Dモデル描画
	auto renderer = player->AddComponent<MeshRenderer>();
	renderer->mesh = engine.LoadOBJ("Res/Model/player_fighter_01.obj");	//ファイルの読み込み
	renderer->scale = vec3(16);			//モデルの拡大
	renderer->rotation.x = radians(90);	//モデルを上から見るために回転



	auto health = player->AddComponent<Health>();			//HPコンポーネント
	player->HP = 5;	//HP量
	health->health = player->HP;	//今のHPを設定
	health->MaxHealth = player->HP;	//最大HPを設定

	auto explosion = player->AddComponent<MegaExplosion>();	//爆発コンポーネント
	explosion->count = 5;			//爆発個数を増やす
	explosion->scale = 2.5;			//爆発の大きさを変更
	player->Point = -1;				//スコア倍率

	auto powerup = player->AddComponent<PowerUpAnim>();	//パワーアップコンポーネント
	powerup->count = 1;				//アニメーションの個数
	player->priority = prioPlayer;	//表示優先順位


	//プレイヤーの音
	auto Playeraudio = player->AddComponent<AudioEvent>();
	Playeraudio->files.onDestory = SE::playerExplosion;	//爆発効果音


}

void MainGameScene::LoadResFaile(Engine& engine)
{
	//敵配置データを読み込む
	const char* enemyMapList[] = { "Res/Map/map001_OBJ.txt",
								   "Res/Map/map002_obj.txt",
								   "Res/Map/map003_obj.txt",
								   "Res/Map/map004_obj.txt",
								   "Res/Map/map005_obj.txt", };

	enemyMap = engine.LoadTileMap(enemyMapList[MapLevel]);
	spawningY = enemyMap.sizeY - 1;	//敵出現ラインを初期化

	//3Dマップリスト
	const char* DMapList[] = { "Res/Map/map02.json" ,
							   "Res/Map/map_02.json",
							   "Res/Map/map_04.json" ,
							   "Res/Map/map_03.json",
							   "Res/Map/map01.json" , };

	//3Dモデル配置ファイルを読み込む
	auto gameObjectList = engine.LoadGameObjectMap(DMapList[MapLevel],
		vec3(0, 0, -300), vec3(3.2f));

	//背景スクロールの設定
	bgOffset = std::make_shared<vec3>(0.0f);

	for (auto& e : gameObjectList)
	{
		auto c = e->AddComponent<BgScroll>();
		c->offset = bgOffset;
	}

	//スプライトを作成（タイルサイズ＝３２＊３２）
	const char* TileList[] = { "Res/Map/map001_BG.txt",
							   "Res/Map/map002_bg.txt",
							   "Res/Map/map003_bg.txt",
							   "Res/Map/map004_bg.txt",
							   "Res/Map/map006_bg.txt",
	};

	TileMap tileMap = engine.LoadTileMap(TileList[MapLevel]);
	engine.SetSpriteList(tileMap);
}

void MainGameScene::SetUI(Engine& engine)
{
	//外側の画像
	const size_t frameLayer = engine.AddUILayer("Res/UI/frame.tga", GL_LINEAR, 10);
	//ゲーム画面の外側
	auto uiFrame = engine.CreateUI<GameObject>(frameLayer, "frame", 640, 360);
	uiFrame->AddSprite({ 0,0,1,1 });

	//武器セレクト画像
	const size_t weaponframeLayer2 = engine.AddUILayer("Res/UI/weaponframe.tga", GL_LINEAR, 10);
	//装備している武器を表示するための枠の表示
	auto uiweapon0 = engine.CreateUI<GameObject>(weaponframeLayer2, "weaponframe", 50, 400);
	uiweapon0->AddSprite({ 0,0,1,5 });

	//必殺ゲージ
	SpecialGauge = engine.AddUILayer("Res/UI/TriggerGauge.tga", GL_NEAREST, 10);
	uiTriggerGauge = engine.CreateUI<GameObject>(SpecialGauge, "trigger", 200, 150);
	uiTriggerGauge->AddSprite({ 0,0,1,1 });

	//必殺ゲージのフレーム
	const size_t uiGaugeFrame = engine.AddUILayer("Res/UI/Gauge_Frama.tga", GL_NEAREST, 10);
	auto uigaugeFrame = engine.CreateUI<GameObject>(uiGaugeFrame, "gauge", 200, 300);
	uigaugeFrame->AddSprite({ 0,0,1,1 });

	//HPバーの下敷き
	const size_t uiBlack = engine.AddUILayer("Res/UI/Boarder.tga", GL_LINEAR, 10);
	auto uiblack = engine.CreateUI<GameObject>(uiBlack, "bkack", 167, 2);
	uiblack->AddSprite({ 0,0,1,1 });

	//プレイヤーのHPバー
	PlayerHPBar = engine.AddUILayer("Res/UI/PlayerHPBar.tga", GL_LINEAR, 10);
	uiPlHPBar = engine.CreateUI<GameObject>(PlayerHPBar, "HPFrame", 367, 50);
	uiPlHPBar->AddSprite({ 0,0,1,1 });
	uiPlHPBar->AddComponent<HPMove>();
	uiPlHPBar->HP = player->HP;

	//プレイヤーのHPフレーム
	const size_t PlayerHPFrame = engine.AddUILayer("Res/UI/PlayerHPFrame.tga", GL_LINEAR, 10);
	auto PlayerFrame = engine.CreateUI<GameObject>(PlayerHPFrame, "HPFrame", 185, 50);
	PlayerFrame->AddSprite({ 0,0,1,1 });

}