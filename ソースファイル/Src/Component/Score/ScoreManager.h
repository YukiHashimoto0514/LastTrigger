//@file ScireManager.h

#ifndef COMPONENT_SCOREMANAGER_H_INCLUDED
#define COMPONENT_SCOREMANAGER_H_INCLUDED
#include "../../Application/Component.h"
#include "../Text.h"
#include <stdio.h>

//���_���Ǘ�����R���|�[�l���g

class ScoreManager :public Component
{
public:
	ScoreManager() = default;			//�R���X�g���N�^
	virtual ~ScoreManager() = default;	//�f�X�g���N�^
	
	virtual void Update(GameObject& gameObject, float deltaTime)override
	{
		if (textComponent)
		{
			char str[16];									//�\�����镶����
			snprintf(str, std::size(str), "%010d", score);	//�����\��
			textComponent->SetText(str, 2.0f);				//�e�L�X�g�ɓ����
		}
	}

	int score = 0;	//�X�R�A
	std::shared_ptr<Text> textComponent;	//������\������
};

using ScoreManagerPtr = std::shared_ptr<ScoreManager>;

#endif // !COMPONENT_SCOREMANAGER_H_INCLUDED
