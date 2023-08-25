//@file TitleScene.cpp

#include "TitleScene.h"
#include "MainGameScene.h"
#include "LoadScene.h"
#include "../Component/Text.h"
#include "EasyAudio.h"
#include "AudioSettings.h"
#include "GameObject.h"
#include "../Engine/Engine.h"
#include "../Component/HP/Health.h"
#include "../Component/Explosion/MegaExplosion.h"


//タイトル画面を初期化する

//@retval true  初期化成功
//@retval false 初期化失敗

bool TitleScene::Initialize(Engine& engine)
{
	//ゲームオブジェクトを削除
	engine.ClearGameObjectList();
	engine.ClearUILayers();

	srand((unsigned int)time(NULL));


	//UIレイヤーを作成
	const size_t bgLayer          = engine.AddUILayer("Res/UI/title_bg.tga", GL_LINEAR, 10);
	const size_t LastlogoLayer	  = engine.AddUILayer("Res/UI/Last.tga", GL_LINEAR, 10);
	const size_t TriggerlogoLayer = engine.AddUILayer("Res/UI/Trigger.tga", GL_LINEAR, 10);
	const size_t textLayer        = engine.AddUILayer("Res/UI/font_04_2.tga", GL_LINEAR, 10);

	//背景画像
	auto uiBackground = engine.CreateUI<GameObject>(
		bgLayer, "title_bg", 640, 360);
	uiBackground->AddSprite({ 0,0,1,1 });


	//タイトルロゴ画像
	uiLast = engine.CreateUI<GameObject>(
		LastlogoLayer, "title_logo", 0, 1000);
	uiLast->AddSprite({ 0,0,1,1 });
	auto move = uiLast->AddComponent<Lerp>();
	move->SetCalc = 2;
	move->TargetX = 470;

	uiTrigger = engine.CreateUI<GameObject>(
		TriggerlogoLayer, "title_logo", 810, -100);
	uiTrigger->AddSprite({ 0,0,1,1 });
	auto move2 = uiTrigger->AddComponent<Lerp>();
	move2->TargetX = 810;
	move2->MaxTime = 2;
	move2->SetCalc = 0;

	const char strLastTrigger[] = "Last Trigger";
	const float fontSizeX2 = 32;
	const float x2 =
		640 - static_cast<float>(std::size(strLastTrigger) - 1) * fontSizeX2;

	//ゲームの開始方法を示すテキスト
	const char strPressEnter[] = "Press  Enter";

	uiPressEnter = engine.CreateUI<GameObject>(textLayer, "press enter", 450, 100);
	auto textPressEnter = uiPressEnter->AddComponent<Text>();
	textPressEnter->SetText(strPressEnter, 2);

	uiPressEnter->alpha = 0;

	//BGMを再生
	Audio::Play(AudioPlayer::bgm, BGM::title, 0.5, true);
	engine.SlowSpeed = 1;
	return true;	//初期化成功
}

//タイトル画面の状態を更新する
void TitleScene::Update(Engine& engine, float deltaTime)
{
	//イージング用タイマーを計測
	Timer += deltaTime;

	//テキストの色を変換
	ChangeLastColor(deltaTime);
	ChangeTriggerColor(deltaTime);

	//タイトルが到着してから
	if (Timer > 1)
	{
		//エンターキーが押されたらゲーム開始
		if (engine.GetKey(GLFW_KEY_ENTER))
		{
			Audio::PlayOneShot(SE::Click, 0.2f);		//効果音を再生
			engine.SetNextScene<LoadScene>();	//メインゲームシーンをセット
		}

		//PressEnterの文字をフェードさせまくる
		uiPressEnter->alpha = Smooth(Timer / 1.5f);
	}
}

//Lastの文字の色が変化する
void TitleScene::ChangeLastColor(float deltaTime)
{
	if (LastColor)
	{
		if (ChangeColorDown(*uiLast, 3, deltaTime))
		{
			LastColor = true;
		}
		else
		{
			LastColor = false;
		}
	}
	else
	{
		if (ChangeColorUp(*uiLast, 3, deltaTime))
		{
			LastColor = false;
		}
		else
		{
			LastColor = true;
		}
	}

}

//Trigerの色を変化させる
void TitleScene::ChangeTriggerColor(float deltaTime)
{	
	if (TriggerColor)
	{

		if (ChangeColorDown(*uiTrigger, 5, deltaTime))
		{
			TriggerColor = true;
		}
		else
		{
			TriggerColor = false;
		}
	}
	else
	{
		if (ChangeColorUp(*uiTrigger, 5, deltaTime))
		{
			TriggerColor = false;
		}
		else
		{
			TriggerColor = true;
		}
	}
}


//色を明るくさせる
bool TitleScene::ChangeColorUp(GameObject& text, const int add, float deltaTime)
{
	//代表して赤色が５より小さかったら
	if (text.Red < 5)
	{
		text.Blue  += add * deltaTime;
		text.Red   += add * deltaTime;
		text.Green += add * deltaTime;

		return true;
	}
	else
	{
		return false;
	}
}

//色を暗くさせる
bool TitleScene::ChangeColorDown(GameObject& text, const int down, float deltaTime)
{
	//代表して赤色が１より大きかったら
	if (text.Red > 1)
	{
		text.Blue  -= down * deltaTime;
		text.Red   -= down * deltaTime;
		text.Green -= down * deltaTime;

		return true;
	}
	else
	{
		return false;
	}
}