//@file Score.h

#ifndef COMPONENT_SCORE_H_INCLUDED
#define COMPONENT_SCORE_H_INCLUDED
#include "../../Application/Component.h"
#include "ScoreManager.h"

///得点コンポーネント

class Score :public Component
{
public:
	Score() = default;			//コンストラクタ
	virtual ~Score() = default;	//デストラクタ

	virtual void OnDestroy(GameObject& gameObject)override
	{
		if (scoreManager)
		{
			scoreManager->score += score*gameObject.Point;//得点が加算される
		}
	}

	ScoreManagerPtr scoreManager;

	int score = 100;	//得点


};


#endif // !COMPONENT_SCORE_H_INCLUDED
