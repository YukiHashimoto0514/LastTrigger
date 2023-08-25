//@file GameOverScene.cpp

#include "GameOverScene.h"
#include "MainGameScene.h"
#include "TitleScene.h"
#include "../Component/Text.h"
#include "EasyAudio.h"
#include "AudioSettings.h"
#include "GameObject.h"
#include "../Engine/Engine.h"

//ゲームオーバー画面を初期化する

//@retval true  初期化成功
//@retval false 初期化失敗

bool GameOverScene::Initialize(Engine& engine)
{
	//ゲームオブジェクトを削除
	engine.ClearGameObjectList();
	engine.ClearUILayers();

	//UIレイヤーを作成
	const size_t bgLayer   = engine.AddUILayer("Res/UI/gameover_bg.tga", GL_LINEAR, 10);
	const size_t logoLayer = engine.AddUILayer("Res/UI/gameover_logo.tga", GL_LINEAR, 10);
	const size_t textLayer = engine.AddUILayer("Res/UI/font_04_2.tga", GL_LINEAR, 10);

	//背景画像
	auto uiBackground = engine.CreateUI<GameObject>(
		bgLayer, "gameover_bg", 640, 360);
	uiBackground->AddSprite({ 0,0,1,1 });

	//ゲームオーバーロゴ画像
	uiGameOver = engine.CreateUI<GameObject>(
		logoLayer, "gameover_logo", 640, 1000);
	uiGameOver->AddSprite({ 0,0,1,1 }, 0, 0, 1.25f);

	auto move = uiGameOver->AddComponent<Lerp>();
	move->TargetX = 640;
	move->TargetY = 600;
	move->MaxTime = 1;
	move->SetCalc = 0;


	//ゲームの開始方法を示すテキスト
	const char strGameOver[] = "Press Space To RePlay";
	const float fontSizeX = 15;
	const float x =
		640 - static_cast<float>(std::size(strGameOver) - 1) * fontSizeX;

	uiPressSpace = engine.CreateUI<GameObject>(textLayer, "Press Space To Title", x, 100);
	auto textGameOver = uiPressSpace->AddComponent<Text>();
	textGameOver->SetText(strGameOver, 2);
	
	//BGMを再生
	Audio::Play(AudioPlayer::bgm, BGM::gameover, 1, true);
	engine.SlowSpeed = 1;
	return true;	//初期化成功
}

//タイトル画面の状態を更新する
void GameOverScene::Update(Engine& engine, float deltaTime)
{
	//経過時間を計測
	Timer += deltaTime;
	//スペースキーが押されたらゲーム開始
	if (engine.GetKey(GLFW_KEY_SPACE))
	{
		Audio::PlayOneShot(SE::Click, 0.2f);		//効果音を再生
		engine.SetNextScene<MainGameScene>();	//タイトルシーンに（仮）
	}

	//PressEnterの文字をフェードさせまくる
	uiPressSpace->alpha = Smooth(Timer);

	//ゲームオーバー画像を拡縮する
	if (Timer < 2)
	{
		uiGameOver->Scale = 1 + Smooth(Timer) / 5;
	}
	else if (Timer > 3)
	{
		Timer = 0;
	}
	
}