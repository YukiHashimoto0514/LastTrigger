//@file GameClearScene.cpp

#include "GameClearScene.h"
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

bool GameClearScene::Initialize(Engine& engine)
{
	//ゲームオブジェクトを削除
	engine.ClearGameObjectList();
	engine.ClearUILayers();

	//UIレイヤーを作成
	const size_t bgLayer   = engine.AddUILayer("Res/UI/title_bg.tga", GL_LINEAR, 10);
	const size_t logoLayer = engine.AddUILayer("Res/UI/gameclear_logo.tga", GL_LINEAR, 10);
	const size_t textLayer = engine.AddUILayer("Res/UI/font_04_2.tga", GL_LINEAR, 10);

	//背景画像
	auto uiBackground = engine.CreateUI<GameObject>(
		bgLayer, "gameclear_bg", 640, 360);
	uiBackground->AddSprite({ 0,0,1,1 });

	//ゲームクリアロゴ画像
	uiGameClear = engine.CreateUI<GameObject>(
		logoLayer, "clear_logo", 660, -600);
	uiGameClear->AddSprite({ 0,0,1,1 }, 0, 0, 1.25f);

	auto move = uiGameClear->AddComponent<Lerp>();
	move->TargetX = 660;
	move->TargetY = 600;
	move->MaxTime = 1;
	move->SetCalc = 0;



	//ゲームの開始方法を示すテキスト
	const char strGameClear[] = "Press Space";
	const float fontSizeX = 15;
	const float x =
		640 - static_cast<float>(std::size(strGameClear) - 1) * fontSizeX;

	//スペースボタン
	uiPressSpace = engine.CreateUI<GameObject>(textLayer, "Press Space", x, 100);
	auto textGameClear = uiPressSpace->AddComponent<Text>();
	textGameClear->SetText(strGameClear, 2);


	//BGMを再生
	Audio::Play(AudioPlayer::bgm, BGM::stageclear, 1, true);

	engine.SlowSpeed = 1;
	NextFlg = false;
	uiPressSpace->alpha = 0;
	return true;	//初期化成功
}

//タイトル画面の状態を更新する
void GameClearScene::Update(Engine& engine, float deltaTime)
{
	//経過時間を計測
	Timer += deltaTime;

	//エンターキーが押されたらゲーム開始
	if (NextFlg)
	{
		//点滅させる
		uiPressSpace->alpha = Smooth(Timer / 1.5f);

		if (engine.GetKey(GLFW_KEY_SPACE))
		{
			Audio::PlayOneShot(SE::Click2, 0.2f);	//効果音を再生
			engine.SetNextScene<TitleScene>();		//タイトルシーンに（仮）
		}
	}

	if (Timer < 2)
	{
		switch (Action)
		{
		case 0://サイズを変える
			uiGameClear->Scale = 1 + Smooth(Timer);
			break;

		case 1://回転させる
			uiGameClear->radias = Smooth(Timer) * 720;
			break;

		case 2://回転しながらサイズを変える
			uiGameClear->Scale = 1 + Smooth(Timer);
			uiGameClear->radias = Smooth(Timer) * 360;
			break;

		case 3://発光させる
			uiGameClear->Red = 1 + Smooth(Timer);
			uiGameClear->Green = 1 + Smooth(Timer);
			uiGameClear->Blue = 1 + Smooth(Timer);
			break;
		}
	}
	else if (Timer > 3)
	{
		NextFlg = true;			//次のシーンに移行できるようにする
		Timer = 0;				//時間をリセットする
		Action = rand() % 4;	//ランダムで動きをきめる
	}


}