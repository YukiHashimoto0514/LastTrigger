//@file TitleScene.h

#ifndef TITLESCENE_H_INCLUDED
#define TITLESCENE_H_INCLUDED
#include "Scene.h"
#include "../Component/Move/Lerp.h"
#include <time.h>
//タイトルシーン

class TitleScene :public Scene
{
public:
	TitleScene() = default;				//コンストラクタ
	virtual ~TitleScene() = default;	//デストラクタ

	virtual bool Initialize(Engine& engine)override;
	virtual void Update(Engine& engine, float deltaTime)override;


	GameObjectPtr uiPressEnter;	//PressEnterのゲームオブジェクト
	GameObjectPtr uiLast;		//タイトルのゲームオブジェクト
	GameObjectPtr uiTrigger;	//タイトルのゲームオブジェクト
	
	bool LastColor = false;		//タイトル文字の色を変えるためのもの
	bool TriggerColor = false;	//タイトル文字の色を変えるためのもの

	float Timer = 0;	//Lerpの関数を使うための変数

private:

	//テキストの色を変換させる
	void ChangeLastColor(float deltaTime);
	void ChangeTriggerColor(float deltaime);

	//色をadd分変更させる
	bool ChangeColorUp(GameObject& text, const int add, float deltaTIme);
	bool ChangeColorDown(GameObject& text, const int down, float deltaTime);

};

#endif //TITLESCENE_H_INCLUDED
