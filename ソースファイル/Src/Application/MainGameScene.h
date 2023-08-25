//@file MainGameScene

#ifndef MAINGAMESCENE_H_INCLUDED
#define MAINGAMESCENE_H_INCLUDED
#include "Scene.h"
#include "../ENgine/TileMap.h"
#include "VecMath.h"

//��s�錾
class ScoreManager;
using ScoreManagerPtr = std::shared_ptr<ScoreManager>;

//���C���Q�[�����

class MainGameScene :public Scene
{
public:
	MainGameScene() = default;			//�R���X�g���N�^
	virtual ~MainGameScene() = default;	//�f�X�g���N�^
	virtual bool Initialize(Engine& engine)override;				//������
	virtual void Update(Engine& engin, float deltaTime)override;	//�X�V����

	float TriggerGauge = 0;				//�K�E�Z���g�����߂ɂ��߂鐔

private:
	void UpdateBoss(Engine& engine,bool Spawnflg, float deltaTime);		//�{�X�̎��񂾂Ƃ��Ƃ�
	void UpdatePlayer(Engine& engine, float deltaTime);	//�v���C���[�̈ړ��Ƃ�
	void LastTrigger(Engine& engin, float deltaTime);	//�K�E�Z����
	void BulletUI(Engine& engine,float deltaTime);		//�e��UI�֌W
	void SetPlayerStatus(Engine& engine);				//�v���C���[�̃X�e�[�^�X���Z�b�g����
	void SetBossStatus(Engine& engine);					//�{�X�̃X�e�[�^�X���Z�b�g����
	void BulletUILoad(Engine& engine);					//�eUI�̏�����
	void LoadResFaile(Engine& engine);					//Res�t�@�C���̏���ǂݎ��
	void SpawnEnemy(Engine& engine);					//�G�̏o��
	void SetUI(Engine& engine);							//UI�̓ǂݍ���

	float restartTimer = 0;			//�ăX�^�[�g�܂ł̎��Ԃ𑪂�^�C�}�[
	float shotTimer = 0;			//�e�̔��ˊԊu�^�C�}�[
	float coolTime = 0;				//�e��؂�ւ���N�[���^�C��

	int ShotNumber = 0;				//�v���C���[���ǂ̒e������
	int MapLevel = 0;				//���݂̃X�e�[�W
	bool BossSpawn = false;			//�{�X���o��������itrue=�o���@false=�T���j
	bool TriggerFlg = false;		//�K�E�Z�@�I���̏��Y�l�F���X�g�g���K�[
	float TriggerTime = 0;			//�K�E�Z���g���鎞��
	float triggerplspeed = 1;	    //�g���K�[���̃v���C���[���x��g����
	float HPBarX = 367;				//�v���C���[��HP�o�[�̍��W

	TileMap enemyMap;				//�G�o���f�[�^
	size_t spawningY = 0;			//�G�o�����C��
	GameObjectPtr player;			//�v���C���[
	GameObjectPtr boss;				//�{�X
	ScoreManagerPtr scoreManager;	//�X�R�A�Ǘ��I�u�W�F�N�g

	size_t Bullet_[5];				//�ǂ̒e�������Ă��邩��\�����邽�߂̉摜
	GameObjectPtr uiBullet_[5];     //UI���Q�[���I�u�W�F�N�g�ŃL���X�g���ē�����

	size_t SelectFrame;				//�ǂ̒e��I�����Ă��邩�̃J�[�\����\�����邽�߂̉摜
	GameObjectPtr uiSelect;			//UI���Q�[���I�u�W�F�N�g�ŃL���X�g���ē�����

	size_t SpecialGauge;			//�X�y�V�����Q�[�W��\��
	GameObjectPtr uiTriggerGauge;	//UI���Q�[���I�u�W�F�N�g�ŃL���X�g���ē�����

	size_t PlayerHPBar;				//�v���C���[��HP�o�[��\��
	GameObjectPtr uiPlHPBar;		//UI���Q�[���I�u�W�F�N�g�ŃL���X�g���ē�����

	GameObjectPtr Key;	//�K�E�Z���g�����߂̃L�[

	std::shared_ptr<VecMath::vec3> bgOffset;	//�w�i�X�N���[�����W
};

#endif // !MAINGAMESCENE_H_INCLUDED

