//@file DiffusionBullet.h

#ifndef COMPONENT_DIFFUSIONBULLET_H_INCLUDED
#define COMPONENT_DIFFUSIONBULLET_H_INCLUDED
#include "../../Application/Component.h"
#include "../../Engine/SpritePriority.h"
#include "../../Application/GameObject.h"
#include "../../Engine/Engine.h"
#include "../Move/Move.h"
#include "../OutOfSreen.h"
#include "../Damage/DamageSource.h"

//�g�U�e�R���|�[�l���g
class DiffusionBullet :public Component
{
public:
	DiffusionBullet() = default;				//�R���X�g���N�^
	virtual ~DiffusionBullet() = default;	//�f�X�g���N�^

	virtual void Update(GameObject& gameObject, float deltaTime)override
	{
		//���˗p�^�C�}�[�����ˊԊu�𒴂�����A���˕ϐ���������
		timerA += deltaTime;//�^�C�}�[�����Z

		if (timerA >= intervalA)	//�^�C�}�[���C���^�[�o���𒴂�����
		{
			isFiring = true;		//���˃t���Otrue
			timerA -= intervalA;	//�^�C�}�[�����炷
		}

		//���˃��[�h�łȂ���΁A�������Ȃ�
		if (!isFiring)
		{
			return;
		}


		//�e�𔭎�
		for (angle=0; angle <= 360;angle+=12)
		{
			auto bullet = gameObject.engine->Create<GameObject>(
				"enemy bullet", gameObject.x + x, gameObject.y + y);

			bullet->AddSprite(tc);						//�e�N�X�`��
			bullet->priority = prioEnemyBullet;			//�D�揇��
			bullet->AddCollision({ -10,-10,10,10 });	//�����蔻��

			const float rad = angle * 3.14f / 180;
			auto move = bullet->AddComponent<Move>();	//�ړ�
			move->vx = speed * cos(rad);
			move->vy = speed * sin(rad);

			bullet->AddComponent<OutOfScreen>();		//��ʊO

			auto damageSource = bullet->AddComponent<DamageSource>();	//�_���[�W
			damageSource->targetName = "player";	//�v���C���[���w��
		}

		isFiring = false;	//���˃t���O��false�ɂ���
	}

	//���������ɐݒ肷��ϐ�
	Texcoord tc = MakeTexcoord(144, 365, 16, 16);	//�e�N�X�`�����W

	float intervalA = 3;	//���ˊԊu
	float speed = 200;		//�e�̑��x
	float x = 0;			//���ˈʒu�̂w���W
	float y = 0;			//���ˈʒu�̂x���W

	//���s���ɕω�����ϐ�
	int angle = 0;			//���ˊp�x
	float timerA = 0;		//���˗p�^�C�}�[
	bool isFiring = false;	//���˃t���O

};



#endif // !COMPONENT_DIFFUSIONBULLET_H_INCLUDED
