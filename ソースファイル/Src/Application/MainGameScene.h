//@file MainGameScene

#ifndef MAINGAMESCENE_H_INCLUDED
#define MAINGAMESCENE_H_INCLUDED
#include "Scene.h"
#include "../ENgine/TileMap.h"
#include "VecMath.h"

//先行宣言
class ScoreManager;
using ScoreManagerPtr = std::shared_ptr<ScoreManager>;

//メインゲーム画面

class MainGameScene :public Scene
{
public:
	MainGameScene() = default;			//コンストラクタ
	virtual ~MainGameScene() = default;	//デストラクタ
	virtual bool Initialize(Engine& engine)override;				//初期化
	virtual void Update(Engine& engin, float deltaTime)override;	//更新処理

	float TriggerGauge = 0;				//必殺技を使うためにためる数

private:
	void UpdateBoss(Engine& engine,bool Spawnflg, float deltaTime);		//ボスの死んだときとか
	void UpdatePlayer(Engine& engine, float deltaTime);	//プレイヤーの移動とか
	void LastTrigger(Engine& engin, float deltaTime);	//必殺技処理
	void BulletUI(Engine& engine,float deltaTime);		//弾のUI関係
	void SetPlayerStatus(Engine& engine);				//プレイヤーのステータスをセットする
	void SetBossStatus(Engine& engine);					//ボスのステータスをセットする
	void BulletUILoad(Engine& engine);					//弾UIの初期化
	void LoadResFaile(Engine& engine);					//Resファイルの情報を読み取る
	void SpawnEnemy(Engine& engine);					//敵の出現
	void SetUI(Engine& engine);							//UIの読み込み

	float restartTimer = 0;			//再スタートまでの時間を測るタイマー
	float shotTimer = 0;			//弾の発射間隔タイマー
	float coolTime = 0;				//弾を切り替えるクールタイム

	int ShotNumber = 0;				//プレイヤーがどの弾を撃つか
	int MapLevel = 0;				//現在のステージ
	bool BossSpawn = false;			//ボスを出現させる（true=出現　false=控室）
	bool TriggerFlg = false;		//必殺技　終末の処刑人：ラストトリガー
	float TriggerTime = 0;			//必殺技を使える時間
	float triggerplspeed = 1;	    //トリガー中のプレイヤー速度を揚げる
	float HPBarX = 367;				//プレイヤーのHPバーの座標

	TileMap enemyMap;				//敵出現データ
	size_t spawningY = 0;			//敵出現ライン
	GameObjectPtr player;			//プレイヤー
	GameObjectPtr boss;				//ボス
	ScoreManagerPtr scoreManager;	//スコア管理オブジェクト

	size_t Bullet_[5];				//どの弾を持っているかを表示するための画像
	GameObjectPtr uiBullet_[5];     //UIをゲームオブジェクトでキャストして動かす

	size_t SelectFrame;				//どの弾を選択しているかのカーソルを表示するための画像
	GameObjectPtr uiSelect;			//UIをゲームオブジェクトでキャストして動かす

	size_t SpecialGauge;			//スペシャルゲージを表示
	GameObjectPtr uiTriggerGauge;	//UIをゲームオブジェクトでキャストして動かす

	size_t PlayerHPBar;				//プレイヤーのHPバーを表示
	GameObjectPtr uiPlHPBar;		//UIをゲームオブジェクトでキャストして動かす

	GameObjectPtr Key;	//必殺技を使うためのキー

	std::shared_ptr<VecMath::vec3> bgOffset;	//背景スクロール座標
};

#endif // !MAINGAMESCENE_H_INCLUDED

