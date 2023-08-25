//@file Score.h

#ifndef COMPONENT_SCORE_H_INCLUDED
#define COMPONENT_SCORE_H_INCLUDED
#include "../../Application/Component.h"
#include "ScoreManager.h"

///���_�R���|�[�l���g

class Score :public Component
{
public:
	Score() = default;			//�R���X�g���N�^
	virtual ~Score() = default;	//�f�X�g���N�^

	virtual void OnDestroy(GameObject& gameObject)override
	{
		if (scoreManager)
		{
			scoreManager->score += score*gameObject.Point;//���_�����Z�����
		}
	}

	ScoreManagerPtr scoreManager;

	int score = 100;	//���_


};


#endif // !COMPONENT_SCORE_H_INCLUDED
