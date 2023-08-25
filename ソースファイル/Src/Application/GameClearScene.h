//@file GameClearScene.h

#ifndef GAMECLEARSCENE_H_INCLUDED
#define GAMECLEARSCENE_H_INCLUDED
#include "Scene.h"

//�Q�[���I�[�o�[�V�[��

class GameClearScene :public Scene
{
public:
	GameClearScene() = default;				//�R���X�g���N�^
	virtual ~GameClearScene() = default;		//�f�X�g���N�^

	virtual bool Initialize(Engine& engine)override;
	virtual void Update(Engine& engine, float deltaTime)override;

	GameObjectPtr uiPressSpace;
	GameObjectPtr uiGameClear;

	float Timer = 0;		//�C�[�W���O�ɕK�v

	int Action = 0;			//�摜�̍s��

	bool NextFlg = false;	//���̃V�[���Ɉڍs���Ă������ǂ���
};

#endif //GAMECLEARSCENE_H_INCLUDED
