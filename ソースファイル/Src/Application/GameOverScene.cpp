//@file GameOverScene.cpp

#include "GameOverScene.h"
#include "MainGameScene.h"
#include "TitleScene.h"
#include "../Component/Text.h"
#include "EasyAudio.h"
#include "AudioSettings.h"
#include "GameObject.h"
#include "../Engine/Engine.h"

//�Q�[���I�[�o�[��ʂ�����������

//@retval true  ����������
//@retval false ���������s

bool GameOverScene::Initialize(Engine& engine)
{
	//�Q�[���I�u�W�F�N�g���폜
	engine.ClearGameObjectList();
	engine.ClearUILayers();

	//UI���C���[���쐬
	const size_t bgLayer   = engine.AddUILayer("Res/UI/gameover_bg.tga", GL_LINEAR, 10);
	const size_t logoLayer = engine.AddUILayer("Res/UI/gameover_logo.tga", GL_LINEAR, 10);
	const size_t textLayer = engine.AddUILayer("Res/UI/font_04_2.tga", GL_LINEAR, 10);

	//�w�i�摜
	auto uiBackground = engine.CreateUI<GameObject>(
		bgLayer, "gameover_bg", 640, 360);
	uiBackground->AddSprite({ 0,0,1,1 });

	//�Q�[���I�[�o�[���S�摜
	uiGameOver = engine.CreateUI<GameObject>(
		logoLayer, "gameover_logo", 640, 1000);
	uiGameOver->AddSprite({ 0,0,1,1 }, 0, 0, 1.25f);

	auto move = uiGameOver->AddComponent<Lerp>();
	move->TargetX = 640;
	move->TargetY = 600;
	move->MaxTime = 1;
	move->SetCalc = 0;


	//�Q�[���̊J�n���@�������e�L�X�g
	const char strGameOver[] = "Press Space To RePlay";
	const float fontSizeX = 15;
	const float x =
		640 - static_cast<float>(std::size(strGameOver) - 1) * fontSizeX;

	uiPressSpace = engine.CreateUI<GameObject>(textLayer, "Press Space To Title", x, 100);
	auto textGameOver = uiPressSpace->AddComponent<Text>();
	textGameOver->SetText(strGameOver, 2);
	
	//BGM���Đ�
	Audio::Play(AudioPlayer::bgm, BGM::gameover, 1, true);
	engine.SlowSpeed = 1;
	return true;	//����������
}

//�^�C�g����ʂ̏�Ԃ��X�V����
void GameOverScene::Update(Engine& engine, float deltaTime)
{
	//�o�ߎ��Ԃ��v��
	Timer += deltaTime;
	//�X�y�[�X�L�[�������ꂽ��Q�[���J�n
	if (engine.GetKey(GLFW_KEY_SPACE))
	{
		Audio::PlayOneShot(SE::Click, 0.2f);		//���ʉ����Đ�
		engine.SetNextScene<MainGameScene>();	//�^�C�g���V�[���Ɂi���j
	}

	//PressEnter�̕������t�F�[�h�����܂���
	uiPressSpace->alpha = Smooth(Timer);

	//�Q�[���I�[�o�[�摜���g�k����
	if (Timer < 2)
	{
		uiGameOver->Scale = 1 + Smooth(Timer) / 5;
	}
	else if (Timer > 3)
	{
		Timer = 0;
	}
	
}