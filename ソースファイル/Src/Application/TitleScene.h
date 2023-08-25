//@file TitleScene.h

#ifndef TITLESCENE_H_INCLUDED
#define TITLESCENE_H_INCLUDED
#include "Scene.h"
#include "../Component/Move/Lerp.h"
#include <time.h>
//�^�C�g���V�[��

class TitleScene :public Scene
{
public:
	TitleScene() = default;				//�R���X�g���N�^
	virtual ~TitleScene() = default;	//�f�X�g���N�^

	virtual bool Initialize(Engine& engine)override;
	virtual void Update(Engine& engine, float deltaTime)override;


	GameObjectPtr uiPressEnter;	//PressEnter�̃Q�[���I�u�W�F�N�g
	GameObjectPtr uiLast;		//�^�C�g���̃Q�[���I�u�W�F�N�g
	GameObjectPtr uiTrigger;	//�^�C�g���̃Q�[���I�u�W�F�N�g
	
	bool LastColor = false;		//�^�C�g�������̐F��ς��邽�߂̂���
	bool TriggerColor = false;	//�^�C�g�������̐F��ς��邽�߂̂���

	float Timer = 0;	//Lerp�̊֐����g�����߂̕ϐ�

private:

	//�e�L�X�g�̐F��ϊ�������
	void ChangeLastColor(float deltaTime);
	void ChangeTriggerColor(float deltaime);

	//�F��add���ύX������
	bool ChangeColorUp(GameObject& text, const int add, float deltaTIme);
	bool ChangeColorDown(GameObject& text, const int down, float deltaTime);

};

#endif //TITLESCENE_H_INCLUDED
