//@file LoadScene.h

#ifndef LOADSCENE_H_INCLUDED
#define LOADSCENE_H_INCLUDED
#include "Scene.h"
#include "../Component/Text.h"

//�Q�[���I�[�o�[�V�[��

class LoadScene :public Scene
{
public:
	LoadScene() = default;				//�R���X�g���N�^
	virtual ~LoadScene() = default;		//�f�X�g���N�^

	virtual bool Initialize(Engine& engine)override;
	virtual void Update(Engine& engine, float deltaTime)override;


	GameObjectPtr uiGameOver;	//�^�C�g���̃Q�[���I�u�W�F�N�g
	bool GameOveralpha = false;	//true=�A���t�@��������@false=�A���t�@��g����

	GameObjectPtr uiPressSpace;	//�^�C�g���̃Q�[���I�u�W�F�N�g
	bool Spacealpha = false;	//true=�A���t�@��������@false=�A���t�@��g����

	GameObjectPtr uiPlanet;		//�f���̃Q�[���I�u�W�F�N�g

	GameObjectPtr Loadstr;		//�����̃Q�[���I�u�W�F�N�g

	float Timer = 0;	//Lerp�̊֐����g�����߂̕ϐ�

	int i = 0;
	GameObjectPtr txtLoad;		//�����̃Q�[���I�u�W�F�N�g



	bool Loadflg = false;
};

#endif //LOADSCENE_H_INCLUDED
