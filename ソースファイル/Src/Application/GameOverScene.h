//@file GameOverScene.h

#ifndef GAMEOVERSCENE_H_INCLUDED
#define GAMEOVERSCENE_H_INCLUDED
#include "Scene.h"

//�Q�[���I�[�o�[�V�[��

class GameOverScene :public Scene
{
public:
	GameOverScene() = default;				//�R���X�g���N�^
	virtual ~GameOverScene() = default;		//�f�X�g���N�^

	virtual bool Initialize(Engine& engine)override;
	virtual void Update(Engine& engine, float deltaTime)override;


	GameObjectPtr uiGameOver;	//�^�C�g���̃Q�[���I�u�W�F�N�g
	bool GameOveralpha = false;	//true=�A���t�@��������@false=�A���t�@��g����

	GameObjectPtr uiPressSpace;	//�^�C�g���̃Q�[���I�u�W�F�N�g
	bool Spacealpha = false;	//true=�A���t�@��������@false=�A���t�@��g����

	float Timer = 0;	//Lerp�̊֐����g�����߂̕ϐ�
};

#endif //GAMEOVERSCENE_H_INCLUDED
