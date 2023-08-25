//@file ScireManager.h

#ifndef COMPONENT_SCOREMANAGER_H_INCLUDED
#define COMPONENT_SCOREMANAGER_H_INCLUDED
#include "../../Application/Component.h"
#include "../Text.h"
#include <stdio.h>

//得点を管理するコンポーネント

class ScoreManager :public Component
{
public:
	ScoreManager() = default;			//コンストラクタ
	virtual ~ScoreManager() = default;	//デストラクタ
	
	virtual void Update(GameObject& gameObject, float deltaTime)override
	{
		if (textComponent)
		{
			char str[16];									//表示する文字列
			snprintf(str, std::size(str), "%010d", score);	//文字表示
			textComponent->SetText(str, 2.0f);				//テキストに入れる
		}
	}

	int score = 0;	//スコア
	std::shared_ptr<Text> textComponent;	//文字を表示する
};

using ScoreManagerPtr = std::shared_ptr<ScoreManager>;

#endif // !COMPONENT_SCOREMANAGER_H_INCLUDED
