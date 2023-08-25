//@file BossMove4.h

#ifndef COMPONENT_BOSSMOVE4_H_INCLUDED
#define COMPONENT_BOSSMOVE4_H_INCLUDED

#include "../../Application/Component.h"
#include "../../Application/GameObject.h"
#include <time.h>

//���˂���{�X
class BossMove4 :public Component
{
public:
	BossMove4() = default;			//�R���X�g���N�^
	virtual ~BossMove4() = default;	//�f�X�g���N�^

	virtual void Start(GameObject& gameObject)override
	{
		MovePattern = 100;
		CenterX = 272;
		CenterY = 500;
		srand((unsigned int)time(NULL));
	}

	virtual void Update(GameObject& gameObject, float deltaTime)override
	{
		auto& ra = static_cast<MeshRenderer&>(*gameObject.componentList[0]);
		//�G��HP�����������ʂ��c�����s�ɓ������
		if (gameObject.PinchFlg && !IsOnce)
		{
			vy = 1;
			IsOnce = true;
			BulletPattern = 10;
			Timer = 0;
			angle = 0;
			Angle = 0;
		}
		else if (!gameObject.PinchFlg)
		{
			vy = 0;
		}

		//�o�ꂵ����
		if (MoveOK)
		{
			//�{�̂���]������
			ra.rotation[1] += deltaTime * 5;

			//��ʂ̒[�Œ��˕Ԃ��Ă���
			if (gameObject.x <= 72)
			{
				vx = vx * (-1);
			}
			if (gameObject.x >= 472)
			{
				vx = vx * (-1);
			}
			if (gameObject.y >= 632)
			{
				vy = vy * (-1);
			}
			if (gameObject.y <= 72)
			{
				vy = vy * (-1);
			}

			//�ړ�����
			gameObject.x += vx * deltaTime * MoveSpeed;
			gameObject.y += vy * deltaTime * MoveSpeed;


			switch (BulletPattern)
			{

				//������ɂł���߂ɔ��˂���e�𕡐��o��
			case 0:
				RandomUpShot(gameObject, deltaTime);
				break;

				//�������ɏ��ԂɌ���
			case 1:
				DownShot(gameObject, deltaTime);
				break;

				//������̑�����������
			case 2:
				DescentShot(gameObject, deltaTime);
				break;

				//������ɏ��ԂɌ���
			case 3:
				UpShot(gameObject, deltaTime);
				break;

				//�{�X�������Ă�������Ɍ���������
			case 10:
				if (Timer == 0)
				{
					Angle = 0;
					intervalB = 0.6f;	//�A�A�ˊԊu
				}
				Timer += deltaTime;

				if (timerB > intervalB)
				{
					speed = 300;
					auto bullet = gameObject.engine->Create<GameObject>(
						"enemy bullet", gameObject.x, gameObject.y);

					bullet->AddSprite(tc[0]);					//�e�N�X�`��
					bullet->priority = prioEnemyBullet;			//�D�揇��
					bullet->AddCollision({ -10,-10,10,10 });	//�����蔻��
					Angle = ra.rotation[1];
					angle = Angle * 3.14f / 180;
					auto move = bullet->AddComponent<boundMove>();	//�ړ�
					move->vx = speed * cos(angle);
					move->vy = speed * sin(angle);


					bullet->AddComponent<OutOfScreen>();		//��ʊO

					auto damageSource = bullet->AddComponent<DamageSource>();	//�_���[�W
					damageSource->targetName = "player";	//�v���C���[���w��

					speed = 380;
					auto bullet2 = gameObject.engine->Create<GameObject>(
						"enemy bullet", gameObject.x, gameObject.y);

					bullet2->AddSprite(tc[1]);					//�e�N�X�`��
					bullet2->priority = prioEnemyBullet;			//�D�揇��
					bullet2->AddCollision({ -10,-10,10,10 });	//�����蔻��
					Angle = gameObject.radias;
					angle = (Angle + 180.0f) * 3.14f / 180;
					auto move2 = bullet2->AddComponent<boundMove>();	//�ړ�
					move2->vx = speed * cos(angle);
					move2->vy = speed * sin(angle);


					bullet2->AddComponent<OutOfScreen>();		//��ʊO

					auto damageSource2 = bullet2->AddComponent<DamageSource>();	//�_���[�W
					damageSource2->targetName = "player";	//�v���C���[���w��

					timerB -= intervalB;

				}
				else
				{
					timerB += deltaTime;

				}

				break;
			}

		}
		else
		{
		MoveStart(gameObject, deltaTime);
		}

	}

	//���������ɐݒ肷��ϐ�
	int MovePattern = 100;	//�s���p�^�[��
	int rndMove = 0;		//�s���������
	bool IsOnce = false;

	//�e�̃e�N�X�`�����W
	Texcoord tc[3] = { MakeTexcoord(144, 365, 16, 16),
					   MakeTexcoord(144, 365, 16, 16),
					   MakeTexcoord(144, 365, 16, 16),
	};

	float intervalA = 1;	//���ˊԊu
	float intervalB = 0.5f;	//�A�A�ˊԊu
	int numberOfFire = 28;	//�A�ː�
	float speed = 200;		//�e�̑��x
	float x = 0;			//���ˈʒu�̂w���W
	float y = 0;			//���ˈʒu�̂x���W
	GameObjectPtr target;	//�W�I

	//���s���ɕω�����ϐ�
	int BulletPattern = 10;
	float angle = 0;		//���ˊp�x

	bool LRMove = true;		//���E�ǂ����Ɉړ����邩 true=���ɍs���@false���E�ɍs��
	float timerA = 0;		//���˗p�^�C�}�[
	float timerB = 0;		//�A�˗p�^�C�}�[
	int fireCounter = 0;	//�ː��J�E���^
	float vx = 0;			//���˃x�N�g���w
	float vy = 0;			//���˃x�N�g���x
	float Timer = 0;		//�ړ��Ȃ�Ɏg��
	float Angle = 0;
	float CenterX = 272;	//�{�X��x��ʒu
	float CenterY = 500;	//�{�X��y��ʒu
	float MoveSpeed = 60;	//�{�X�̐i�ޑ���

	bool MoveOK = false;	//�ړI�n�܂œ��B�������ǂ���true��OK false��NO
	int ShotCnt = 0;		//����s�����邩


private:

	//������ɂł���߂ɔ��˂���e�𕡐��o��
	void RandomUpShot(GameObject& gameObject, float deltaTime)
	{
		if (Timer == 0)
		{
			Angle = 0;
			numberOfFire = 20;
			intervalB = 0.4f;	//�A�A�ˊԊu

		}
		Timer += deltaTime;

		if (numberOfFire > 0)
		{
			if (timerB > intervalB)
			{
				speed = 200;
				if (numberOfFire % 2 == 0)
				{
					auto bullet = gameObject.engine->Create<GameObject>(
						"enemy bullet", gameObject.x, gameObject.y);

					bullet->AddSprite(tc[0]);					//�e�N�X�`��
					bullet->priority = prioEnemyBullet;			//�D�揇��
					bullet->AddCollision({ -10,-10,10,10 });	//�����蔻��

					angle = (static_cast<float>(rand() % 40) + 30) * 3.14f / 180;
					auto move = bullet->AddComponent<boundMove>();	//�ړ�
					move->vx = speed * cos(angle);
					move->vy = speed * sin(angle);


					bullet->AddComponent<OutOfScreen>();		//��ʊO

					auto damageSource = bullet->AddComponent<DamageSource>();	//�_���[�W
					damageSource->targetName = "player";	//�v���C���[���w��
				}
				else
				{
					auto bullet = gameObject.engine->Create<GameObject>(
						"enemy bullet", gameObject.x, gameObject.y);

					bullet->AddSprite(tc[0]);					//�e�N�X�`��
					bullet->priority = prioEnemyBullet;			//�D�揇��
					bullet->AddCollision({ -10,-10,10,10 });	//�����蔻��

					angle = (static_cast<int>(rand()) % 30 + 120) * 3.14f / 180;
					auto move = bullet->AddComponent<boundMove>();	//�ړ�
					move->vx = speed * cos(angle);
					move->vy = speed * sin(angle);

					bullet->AddComponent<OutOfScreen>();		//��ʊO

					auto damageSource = bullet->AddComponent<DamageSource>();	//�_���[�W
					damageSource->targetName = "player";	//�v���C���[���w��

				}
				timerB -= intervalB;
				numberOfFire--;
			}
			else
			{
				timerB += deltaTime;

			}
		}
		else
		{
			BulletPattern = rand() % 4;
			Timer = 0;
		}
	}

	//�������Ɍ������Ēe������
	void DownShot(GameObject& gameObject, float deltaTime)
	{
		if (Timer == 0)
		{
			Angle = 0;
			numberOfFire = 28;
			intervalB = 0.2f;	//�A�A�ˊԊu

		}

		Timer += deltaTime;
		if (numberOfFire > 0)
		{
			if (timerB > intervalB)
			{
				speed = 200;
				if (numberOfFire > 14)
				{
					Angle += 10;
				}
				else
				{
					Angle -= 10;
				}

				auto bullet = gameObject.engine->Create<GameObject>(
					"enemy bullet", gameObject.x, gameObject.y);

				bullet->AddSprite(tc[0]);					//�e�N�X�`��
				bullet->priority = prioEnemyBullet;			//�D�揇��
				bullet->AddCollision({ -10,-10,10,10 });	//�����蔻��

				angle = (200 + Angle) * 3.14f / 180;
				auto move = bullet->AddComponent<boundMove>();	//�ړ�
				move->vx = speed * cos(angle);
				move->vy = speed * sin(angle);


				bullet->AddComponent<OutOfScreen>();		//��ʊO

				auto damageSource = bullet->AddComponent<DamageSource>();	//�_���[�W
				damageSource->targetName = "player";	//�v���C���[���w��
				timerB -= intervalB;
				numberOfFire--;

			}
			else
			{
				timerB += deltaTime;

			}
		}
		else
		{
			BulletPattern = rand() % 4;
			Timer = 0;
		}

	}

	//����Ȃ���~��Ă���e
	void DescentShot(GameObject& gameObject, float deltaTime)
	{
		if (Timer == 0)
		{
			Angle = 0;
			numberOfFire = 6;
			intervalB = 0.1f;	//�A�A�ˊԊu

		}

		Timer += deltaTime;
		if (numberOfFire > 0)
		{
			if (timerB > intervalB)
			{

				if (numberOfFire > 3)
				{
					Angle = 190;
				}
				else
				{
					Angle = 350;
				}

				speed = 500;
				auto bullet = gameObject.engine->Create<GameObject>(
					"enemy bullet", gameObject.x, gameObject.y);

				bullet->AddSprite(tc[1]);					//�e�N�X�`��
				bullet->priority = prioEnemyBullet;			//�D�揇��
				bullet->AddCollision({ -10,-10,10,10 });	//�����蔻��

				angle = (Angle) * 3.14f / 180;
				auto move = bullet->AddComponent<boundMove>();	//�ړ�
				move->Counter = -4;
				move->vx = speed * cos(angle);
				move->vy = speed * sin(angle);


				bullet->AddComponent<OutOfScreen>();		//��ʊO

				auto damageSource = bullet->AddComponent<DamageSource>();	//�_���[�W
				damageSource->targetName = "player";	//�v���C���[���w��

				numberOfFire--;
				timerB -= intervalB;
			}
			else
			{
				timerB += deltaTime;
			}
		}
		else
		{
			BulletPattern = rand() % 4;
			Timer = 0;
			numberOfFire = 20;
		}

	}

	//������ɒe��ł�
	void UpShot(GameObject& gameObject, float deltaTime)
	{
		if (Timer == 0)
		{
			Angle = 0;
			numberOfFire = 28;
			intervalB = 0.2f;	//�A�A�ˊԊu

		}

		Timer += deltaTime;
		if (numberOfFire > 0)
		{
			if (timerB > intervalB)
			{
				speed = 600;
				if (numberOfFire > 14)
				{
					Angle += 10;
				}
				else
				{
					Angle -= 10;
				}

				auto bullet = gameObject.engine->Create<GameObject>(
					"enemy bullet", gameObject.x, gameObject.y);

				bullet->AddSprite(tc[2]);					//�e�N�X�`��
				bullet->priority = prioEnemyBullet;			//�D�揇��
				bullet->AddCollision({ -10,-10,10,10 });	//�����蔻��

				angle = (20 + Angle) * 3.14f / 180;
				auto move = bullet->AddComponent<boundMove>();	//�ړ�
				move->Counter = 1;
				move->vx = speed * cos(angle);
				move->vy = speed * sin(angle);


				bullet->AddComponent<OutOfScreen>();		//��ʊO

				auto damageSource = bullet->AddComponent<DamageSource>();	//�_���[�W
				damageSource->targetName = "player";	//�v���C���[���w��
				timerB -= intervalB;
				numberOfFire--;

			}
			else
			{
				timerB += deltaTime;

			}
		}
		else
		{
			BulletPattern = rand() % 4;
			Timer = 0;
		}

	}

	//�s���J�n
	void MoveStart(GameObject& gameObject, float deltaTime)
	{
		//��ʂ̐^�񒆂܂ō~��Ă��鏈��
		if (gameObject.y > 570)
		{
			gameObject.y += -MoveSpeed * deltaTime * 10;
		}
		else
		{
			MoveOK = true;
			vx = 1;
			vy = 1;
			BulletPattern = rand() % 4;
		}
	}
};




#endif //COMPONENT_BOSSMOVE4_H_INCLUDED
