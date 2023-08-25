//@file FireBullet.h

#ifndef COMPONENT_FIREBULLET_H_INCLUDED
#define COMPONENT_FIREBULLET_H_INCLUDED
#include "../../Application/Component.h"
#include "../../Engine/SpritePriority.h"
#include "../../Application/GameObject.h"
#include "../../Engine/Engine.h"
#include "../Move/Move.h"
#include "../OutOfSreen.h"
#include "../Damage/DamageSource.h"

//�G�e���˃R���|�[�l���g

class FireBullet :public Component
{
public:
	FireBullet() = default;				//�R���X�g���N�^
	virtual ~FireBullet() = default;	//�f�X�g���N�^

	virtual void Update(GameObject& gameObject, float deltaTime)override
	{
		//���˗p�^�C�}�[�����ˊԊu�𒴂�����A���˕ϐ���������
		timerA += deltaTime;//�^�C�}�[�����Z

		if (timerA >= intervalA)	//�^�C�}�[���C���^�[�o���𒴂�����
		{
			isFiring = true;		//���˃t���Otrue
			timerA -= intervalA;	//�^�C�}�[�����炷
			timerB = intervalB;		//�ꔭ�ڂ������ɔ��˂����
			fireCounter = numberOfFire;

			//�^�[�Q�b�g�w�肪����΁A���˃x�N�g�����^�[�Q�b�g�ɂނ���
			if (target)
			{
				//�O����
				vx = target->x - gameObject.x;
				vy = target->y - gameObject.y;
				const float v = sqrt(vx * vx + vy * vy);
				vx = vx / v * speed;
				vy = vy / v * speed;
			}
			else//�^�[�Q�b�g�w�肪�Ȃ���΁A�^���ɑł�
			{
				vx = 0;
				vy = -speed;
			}

		}
			//���˃��[�h�łȂ���΁A�������Ȃ�
		if (!isFiring)
		{
			return;
		}

		//�A�˗p�^�C�}�[���A�ˊԊu�𒴂�����A�e�𔭎�
		timerB += deltaTime;
		if (timerB < intervalB)
		{
			return;
		}
		timerB -= intervalB;
		//�A�˒��̒e�̕��������������Ȃ炱����

		
		//�e�𔭎�
		auto bullet = gameObject.engine->Create<GameObject>(
			"enemy bullet", gameObject.x + x, gameObject.y + y);

		bullet->AddSprite(tc);					//�e�N�X�`��
		bullet->priority = prioEnemyBullet;		//�D�揇��
		bullet->AddCollision({ -10,-10,10,10 });	//�����蔻��

		auto move = bullet->AddComponent<Move>();	//�ړ�
		move->vx = vx;
		move->vy = vy;

		bullet->AddComponent<OutOfScreen>();	//��ʊO

		auto damageSource = bullet->AddComponent<DamageSource>();	//�_���[�W
		damageSource->targetName = "player";	//�v���C���[���w��
		
		//�A�˃J�E���^�����炵�A�O�ȏ�ɂȂ����甭�˃��[�h����
		--fireCounter;

		if (fireCounter <= 0)
		{
			isFiring = false;	//���˃t���O��false�ɂ���
		}
	}

	//���������ɐݒ肷��ϐ�
	GameObjectPtr target;	//�W�I
	Texcoord tc = MakeTexcoord(144, 365, 16, 16);	//�e�N�X�`�����W
	float intervalA = 1;	//���ˊԊu
	float intervalB = 0.1f;	//�A�A�ˊԊu
	int numberOfFire = 3;	//�A�ː�
	float speed = 200;		//�e�̑��x
	float x = 0;			//���ˈʒu�̂w���W
	float y = 0;			//���ˈʒu�̂x���W

	//���s���ɕω�����ϐ�
	bool isFiring = false;	//���˃t���O
	float timerA = 0;		//���˗p�^�C�}�[
	float timerB = 0;		//�A�˗p�^�C�}�[
	int fireCounter = 0;	//�ː��J�E���^
	float vx = 0;			//���˃x�N�g���w
	float vy = 0;			//���˃x�N�g���x

};



#endif // !COMPONENT_FIREBULLET_H_INCLUDED

