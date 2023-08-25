//@file TitleScene.cpp

#include "TitleScene.h"
#include "MainGameScene.h"
#include "LoadScene.h"
#include "../Component/Text.h"
#include "EasyAudio.h"
#include "AudioSettings.h"
#include "GameObject.h"
#include "../Engine/Engine.h"
#include "../Component/HP/Health.h"
#include "../Component/Explosion/MegaExplosion.h"


//�^�C�g����ʂ�����������

//@retval true  ����������
//@retval false ���������s

bool TitleScene::Initialize(Engine& engine)
{
	//�Q�[���I�u�W�F�N�g���폜
	engine.ClearGameObjectList();
	engine.ClearUILayers();

	srand((unsigned int)time(NULL));


	//UI���C���[���쐬
	const size_t bgLayer          = engine.AddUILayer("Res/UI/title_bg.tga", GL_LINEAR, 10);
	const size_t LastlogoLayer	  = engine.AddUILayer("Res/UI/Last.tga", GL_LINEAR, 10);
	const size_t TriggerlogoLayer = engine.AddUILayer("Res/UI/Trigger.tga", GL_LINEAR, 10);
	const size_t textLayer        = engine.AddUILayer("Res/UI/font_04_2.tga", GL_LINEAR, 10);

	//�w�i�摜
	auto uiBackground = engine.CreateUI<GameObject>(
		bgLayer, "title_bg", 640, 360);
	uiBackground->AddSprite({ 0,0,1,1 });


	//�^�C�g�����S�摜
	uiLast = engine.CreateUI<GameObject>(
		LastlogoLayer, "title_logo", 0, 1000);
	uiLast->AddSprite({ 0,0,1,1 });
	auto move = uiLast->AddComponent<Lerp>();
	move->SetCalc = 2;
	move->TargetX = 470;

	uiTrigger = engine.CreateUI<GameObject>(
		TriggerlogoLayer, "title_logo", 810, -100);
	uiTrigger->AddSprite({ 0,0,1,1 });
	auto move2 = uiTrigger->AddComponent<Lerp>();
	move2->TargetX = 810;
	move2->MaxTime = 2;
	move2->SetCalc = 0;

	const char strLastTrigger[] = "Last Trigger";
	const float fontSizeX2 = 32;
	const float x2 =
		640 - static_cast<float>(std::size(strLastTrigger) - 1) * fontSizeX2;

	//�Q�[���̊J�n���@�������e�L�X�g
	const char strPressEnter[] = "Press  Enter";

	uiPressEnter = engine.CreateUI<GameObject>(textLayer, "press enter", 450, 100);
	auto textPressEnter = uiPressEnter->AddComponent<Text>();
	textPressEnter->SetText(strPressEnter, 2);

	uiPressEnter->alpha = 0;

	//BGM���Đ�
	Audio::Play(AudioPlayer::bgm, BGM::title, 0.5, true);
	engine.SlowSpeed = 1;
	return true;	//����������
}

//�^�C�g����ʂ̏�Ԃ��X�V����
void TitleScene::Update(Engine& engine, float deltaTime)
{
	//�C�[�W���O�p�^�C�}�[���v��
	Timer += deltaTime;

	//�e�L�X�g�̐F��ϊ�
	ChangeLastColor(deltaTime);
	ChangeTriggerColor(deltaTime);

	//�^�C�g�����������Ă���
	if (Timer > 1)
	{
		//�G���^�[�L�[�������ꂽ��Q�[���J�n
		if (engine.GetKey(GLFW_KEY_ENTER))
		{
			Audio::PlayOneShot(SE::Click, 0.2f);		//���ʉ����Đ�
			engine.SetNextScene<LoadScene>();	//���C���Q�[���V�[�����Z�b�g
		}

		//PressEnter�̕������t�F�[�h�����܂���
		uiPressEnter->alpha = Smooth(Timer / 1.5f);
	}
}

//Last�̕����̐F���ω�����
void TitleScene::ChangeLastColor(float deltaTime)
{
	if (LastColor)
	{
		if (ChangeColorDown(*uiLast, 3, deltaTime))
		{
			LastColor = true;
		}
		else
		{
			LastColor = false;
		}
	}
	else
	{
		if (ChangeColorUp(*uiLast, 3, deltaTime))
		{
			LastColor = false;
		}
		else
		{
			LastColor = true;
		}
	}

}

//Triger�̐F��ω�������
void TitleScene::ChangeTriggerColor(float deltaTime)
{	
	if (TriggerColor)
	{

		if (ChangeColorDown(*uiTrigger, 5, deltaTime))
		{
			TriggerColor = true;
		}
		else
		{
			TriggerColor = false;
		}
	}
	else
	{
		if (ChangeColorUp(*uiTrigger, 5, deltaTime))
		{
			TriggerColor = false;
		}
		else
		{
			TriggerColor = true;
		}
	}
}


//�F�𖾂邭������
bool TitleScene::ChangeColorUp(GameObject& text, const int add, float deltaTime)
{
	//��\���ĐԐF���T��菬����������
	if (text.Red < 5)
	{
		text.Blue  += add * deltaTime;
		text.Red   += add * deltaTime;
		text.Green += add * deltaTime;

		return true;
	}
	else
	{
		return false;
	}
}

//�F���Â�������
bool TitleScene::ChangeColorDown(GameObject& text, const int down, float deltaTime)
{
	//��\���ĐԐF���P���傫��������
	if (text.Red > 1)
	{
		text.Blue  -= down * deltaTime;
		text.Red   -= down * deltaTime;
		text.Green -= down * deltaTime;

		return true;
	}
	else
	{
		return false;
	}
}