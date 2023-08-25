//@file LoadScene.cpp

#include "LoadScene.h"
#include "MainGameScene.h"
#include "TitleScene.h"
#include "../Component/Text.h"
#include "EasyAudio.h"
#include "AudioSettings.h"
#include "GameObject.h"
#include "../Engine/Engine.h"
#include "../Component/Move/Lerp.h"
#include "../Component/MeshRenderer.h"


//ロード画面画面を初期化する

//@retval true  初期化成功
//@retval false 初期化失敗

bool LoadScene::Initialize(Engine& engine)
{
	//ゲームオブジェクトを削除
	engine.ClearGameObjectList();
	engine.ClearUILayers();

	//UIレイヤーを作成
	const size_t bgLayer   = engine.AddUILayer("Res/UI/gameover_bg.tga", GL_LINEAR, 10);
	const size_t LoadLayer = engine.AddUILayer("Res/UI/LoadPlanet.tga", GL_LINEAR, 10);
	const size_t textLayer = engine.AddUILayer("Res/UI/font_04_2.tga", GL_LINEAR, 10);

	////背景画像(真っ黒)
	auto uiBackground = engine.CreateUI<GameObject>(
		bgLayer, "gameover_bg", 640, 360);
	uiBackground->AddSprite({ 0,0,1,1 });
	uiBackground->Red = 0;
	uiBackground->Green = 0;
	uiBackground->Blue = 0;

	//ロード中に動く惑星
	uiPlanet = engine.CreateUI<GameObject>(
		LoadLayer, "LoadPlanet", 640, 360);
	uiPlanet->AddSprite({ 0,0,1,1 });

	//ロードテキスト
	if (engine.GetMapLevel() == 0)
	{
		const char load[] = "Now Loading...";
		const float fontSizeX2 = 15;
		const float loadx =
			640 - static_cast<float>(std::size(load) - 1) * fontSizeX2;
		Loadstr = engine.CreateUI<GameObject>(textLayer, "text", loadx, 600);
		auto textLoad2 = Loadstr->AddComponent<Text>();
		textLoad2->SetText(load, 2);
	}
	else
	{
		const char load[] = "Load Next Scene...";
		const float fontSizeX2 = 15;
		const float loadx =
			640 - static_cast<float>(std::size(load) - 1) * fontSizeX2;
		Loadstr = engine.CreateUI<GameObject>(textLayer, "text", loadx, 600);
		auto textLoad2 = Loadstr->AddComponent<Text>();
		textLoad2->SetText(load, 2);
	}

	//ゲームの開始方法を示すテキスト
	const char strToPlay[] = "Press Space To Start";
	const float fontSizeX = 15;
	const float x =
		640 - static_cast<float>(std::size(strToPlay) - 1) * fontSizeX;

	//スペースボタン
	uiPressSpace = engine.CreateUI<GameObject>(textLayer, "Press Space", x, 100);
	auto textGameOver = uiPressSpace->AddComponent<Text>();
	textGameOver->SetText(strToPlay, 2);
	uiPressSpace->alpha = 1;//最初は見えない

	//BGMを再生
	Audio::Play(AudioPlayer::bgm, BGM::load, 1, true);
	engine.SlowSpeed = 1;

	//ロードパーセントを表示
	const float fontSizeX2 = 15;
	const float loadx =
		640 - 2 * fontSizeX2;
	txtLoad = engine.CreateUI<GameObject>(textLayer, "Loadtxt", loadx, 500);

	char str[16];									//表示する文字列
	snprintf(str, std::size(str), "LoadPercent 0");		//文字表示
	auto txt = txtLoad->AddComponent<Text>();
	txt->SetText(str, 2);


	return true;	//初期化成功
}

//3Dモデル（随時追加）
const char* Loadlist[] = {
	"Res/Model/BigEnemy.obj",			//大型機
	"Res/Model/enemy_s_01.obj",			//小型機
	"Res/Model/StarEnemy.obj",			//中型機
	"Res/Model/Magu.obj",				//マグマ
	"Res/Model/Dirt.obj",				//地面
	"Res/Model/Big_Tyled_Floor.obj",	//地面
	"Res/Model/GraoundGrass2.obj",		//地面
	"Res/Model/GroundGrass.obj",		//地面
	"Res/Model/Ground.obj",				//床
	"Res/Model/Dirt.obj",				//土
	"Res/Model/Water.obj",				//水
	"Res/Model/tree_1.obj",				//木
	"Res/Model/ForestTreePineTall.obj",	//木
	"Res/Model/ForestTreePineShort.obj",//木
	"Res/Model/ForestTreeDShort.obj",	//木
	"Res/Model/ChaosTree.obj",			//変な木
	"Res/Model/crystal_17_2.obj",		//クリスタル
	"Res/Model/SF_Free-Fighter.obj",	//背景戦闘
	"Res/Model/BackFlyObj.obj",			//背景戦闘機
	"Res/Model/player_fighter_01.obj",	//プレイヤー
	"Res/Model/wrak_budova.obj",		//建物
	"Res/Model/Boss_1.obj",				//ボス
	"Res/Model/Boss_2.obj",				//ボス
	"Res/Model/Boss_3.obj",				//ボス
	"Res/Model/Boss_5.obj",				//ボス

};
//配列の長さを取得
const float LoadLength = sizeof(Loadlist) / sizeof(Loadlist[0]);


//タイトル画面の状態を更新する
void LoadScene::Update(Engine& engine, float deltaTime)
{
	Timer += deltaTime;			//イージングをするためのタイマー
	uiPlanet->radias += Timer;	//惑星を回す
	

	//上下移動する
	Loadstr->y = 600 + Smooth(Timer) * 20;

	//オブジェクトのロードが終ってたら
	if (Loadflg)
	{
		//透明と半透明を行き来する
		uiPressSpace->alpha = Smooth(Timer);
		uiPressSpace->radias = 0;

	    //スペースキーが押されたらゲーム開始
		if (engine.GetKey(GLFW_KEY_SPACE))
		{
			Audio::PlayOneShot(SE::Click2, 0.2f);		//効果音を再生
			engine.SetNextScene<MainGameScene>();		//メインゲームシーンをセット
		}
	}
	else//ロード中
	{
		//回してサイズを変える
		uiPressSpace->radias = Bound(Timer) * 2160;
		uiPlanet->Scale = 1 + Smooth(Timer);
	}

	//0.5秒後から読み込み開始
	if (Timer >= 0.5f)
	{
		if (!Loadflg)
		{
			if (i < LoadLength)
			{
				//読み込み具合を計算
				const float per = i / (LoadLength - 1) * 100;

				//読み込みパーセントを更新
				char str[16];									//表示する文字列
				snprintf(str, std::size(str), "LoadPercent %.f", per);		//文字表示
				auto txt = txtLoad->AddComponent<Text>();
				txt->SetText(str, 2);

				//３Ⅾモデルを先に読み込んでおく
				GameObjectPtr obj = engine.Create<GameObject>("enemy", 0, 0);

				const char* mod = Loadlist[i];			//読み込むファイル名
				auto objrender = obj->AddComponent<MeshRenderer>();
				objrender->mesh = engine.LoadOBJ(mod);	//ロード

				//次のモデルを読み込む
				i++;
			}
			else
			{
				//読み込み終了
				Loadflg = true;
			}
		}
	}
}

