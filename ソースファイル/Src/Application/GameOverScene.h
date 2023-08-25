//@file GameOverScene.h

#ifndef GAMEOVERSCENE_H_INCLUDED
#define GAMEOVERSCENE_H_INCLUDED
#include "Scene.h"

//ゲームオーバーシーン

class GameOverScene :public Scene
{
public:
	GameOverScene() = default;				//コンストラクタ
	virtual ~GameOverScene() = default;		//デストラクタ

	virtual bool Initialize(Engine& engine)override;
	virtual void Update(Engine& engine, float deltaTime)override;


	GameObjectPtr uiGameOver;	//タイトルのゲームオブジェクト
	bool GameOveralpha = false;	//true=アルファを下げる　false=アルファを揚げる

	GameObjectPtr uiPressSpace;	//タイトルのゲームオブジェクト
	bool Spacealpha = false;	//true=アルファを下げる　false=アルファを揚げる

	float Timer = 0;	//Lerpの関数を使うための変数
};

#endif //GAMEOVERSCENE_H_INCLUDED
