//@file GameClearScene.h

#ifndef GAMECLEARSCENE_H_INCLUDED
#define GAMECLEARSCENE_H_INCLUDED
#include "Scene.h"

//ゲームオーバーシーン

class GameClearScene :public Scene
{
public:
	GameClearScene() = default;				//コンストラクタ
	virtual ~GameClearScene() = default;		//デストラクタ

	virtual bool Initialize(Engine& engine)override;
	virtual void Update(Engine& engine, float deltaTime)override;

	GameObjectPtr uiPressSpace;
	GameObjectPtr uiGameClear;

	float Timer = 0;		//イージングに必要

	int Action = 0;			//画像の行動

	bool NextFlg = false;	//次のシーンに移行していいかどうか
};

#endif //GAMECLEARSCENE_H_INCLUDED
