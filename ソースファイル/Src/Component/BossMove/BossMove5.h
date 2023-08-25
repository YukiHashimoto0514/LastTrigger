//@file BossMove5.h

#ifndef COMPONENT_BOSSMOVE5_H_INCLUDED
#define COMPONENT_BOSSMOVE5_H_INCLUDED

#include "../../Application/Component.h"
#include "../../Application/GameObject.h"
#include <time.h>

//�ђʂ��C���[�W�����{�X
class BossMove5 :public Component
{
public:
	BossMove5() = default;			//�R���X�g���N�^
	virtual ~BossMove5() = default;	//�f�X�g���N�^

	virtual void Start(GameObject& gameObject)override
	{
		MovePattern = 100;
		CenterX = 272;
		CenterY = 500;
		srand((unsigned int)time(NULL));
	}

	virtual void Update(GameObject& gameObject, float deltaTime)override
	{

		//�o�ꂵ����
		if (MoveOK)
		{
			switch (BulletPattern)
			{

			//�������Ɉ��̊Ԋu��5�������b�N�I�����Ȃ������
			case 0:
				LockOnShot(gameObject, deltaTime);
				break;

			//�N���X�X�e�B���K�[
			case 1:
				CrossStingger(gameObject, deltaTime);
				break;

			//�~��`���Ȃ���U��
			case 2:
				CircleShot(gameObject, deltaTime);
				break;

			//�v���C���[�̕����Ƀu���X�̂悤�ɒe��f��
			case 3:
				BressShot(gameObject, deltaTime);
				break;
			}

		}
		else
		{
			//�s���J�n
			MoveStart(gameObject, deltaTime);
		}
	}

	//�U���p�^�[����ς���֐�
	void ChabgeAttack(int old)
	{
		while (old == BulletPattern)
		{
			BulletPattern = rand() % 4;
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
	GameObjectPtr boss;		//�U���X�N���v�g�ɕK�v

	//���s���ɕω�����ϐ�
	int BulletPattern = 0;	//�s���p�^�[��
	float angle = 0;		//���ˊp�x

	bool LRMove = true;		//���E�ǂ����Ɉړ����邩 true=���ɍs���@false���E�ɍs��
	float timerA = 0;		//���˗p�^�C�}�[
	float timerB = 0;		//�A�˗p�^�C�}�[
	int fireCounter = 0;	//�ː��J�E���^
	float vx = 0;			//���˃x�N�g���w
	float vy = 0;			//���˃x�N�g���x
	float Timer = 0;		//�ړ��Ȃ�Ɏg��
	float Angle = 0;		//�p�x
	float CenterX = 272;	//�{�X��x��ʒu
	float CenterY = 570;	//�{�X��y��ʒu
	float MoveSpeed = 60;	//�{�X�̐i�ޑ���

	bool MoveOK = false;							//�ړI�n�܂œ��B�������ǂ���true��OK false��NO
	int ShotCnt = 0;			//����s�����邩

	//HP�������ɂȂ������Ɏg���ϐ�
	float BeforeX = 0;
	float BeforeY = 0;
	bool isOnce = false;

	private:
		//�����Ă�������ɂT��������
		void LockOnShot(GameObject& gameObject, float deltaTime)
		{
			if (Timer == 0)
			{
				Angle = 0;
				intervalB = 0.5f;	//�A�A�ˊԊu

			}
			Timer += deltaTime;

			if (Timer < 10)
			{
				if (timerB > intervalB)
				{
					speed = 300;
					vx = target->x - gameObject.x;
					vy = target->y - gameObject.y;

					float rad = atan2(vy, vx);
					Angle = rad * 180.0f / 3.14f;
					float AngleChild = Angle;


					//5�����ɋʂ�ł�
					for (int i = -30; i < 60; i += 15)
					{
						auto bullet = gameObject.engine->Create<GameObject>(
							"enemy bullet", gameObject.x + x, gameObject.y + y);

						bullet->AddSprite(tc[0]);					//�e�N�X�`��
						bullet->priority = prioEnemyBullet;		//�D�揇��
						bullet->AddCollision({ -10,-10,10,10 });	//�����蔻��

						auto move = bullet->AddComponent<WallThroughMove>();	//�ړ�
						Angle = i + AngleChild;

						rad = Angle * 3.14f / 180;
						move->vx = cos(rad) * speed;
						move->vy = sin(rad) * speed;
						move->Counter = 1;
						bullet->AddComponent<OutOfScreen>();	//��ʊO

						auto damageSource = bullet->AddComponent<DamageSource>();	//�_���[�W
						damageSource->targetName = "player";	//�v���C���[���w��
					}
					timerB -= intervalB;
				}
				else
				{
					timerB += deltaTime;
				}
			}
			else if (Timer > 13)
			{
				//BulletPattern = rand() % 4;
				ChabgeAttack(0);
				Timer = 0;
			}

		}

		//�N���X�X�e�B���K�[������
		void CrossStingger(GameObject& gameObject, float deltaTime)
		{
			if (Timer == 0)
			{
				angle = 0;
				numberOfFire = 20;
				intervalB = 0.1f;	//�A�A�ˊԊu

				//�ړ��O�̍��W
				BeforeY = gameObject.y;
			}

			Timer += deltaTime;
			if (numberOfFire > 0)
			{
				if (Timer < 1)
				{
					//�ړ������i�C�[�W���O�j
					gameObject.y = BeforeY + Soft(Timer) * (650 - BeforeY);
				}
				else
				{
					if (timerB > intervalB)
					{
						speed = 600;

						auto bullet = gameObject.engine->Create<GameObject>(
							"enemy bullet", gameObject.x, gameObject.y);

						bullet->AddSprite(tc[0]);					//�e�N�X�`��
						bullet->priority = prioEnemyBullet;			//�D�揇��
						bullet->AddCollision({ -10,-10,10,10 });	//�����蔻��

						auto move = bullet->AddComponent<CrossMove>();	//�ړ�
						move->Boss = boss;
						move->target = target;
						move->vx = speed;
						move->vy = speed;

						bullet->AddComponent<OutOfScreen>();		//��ʊO

						auto damageSource = bullet->AddComponent<DamageSource>();	//�_���[�W
						damageSource->targetName = "player";	//�v���C���[���w��

						auto bullet2 = gameObject.engine->Create<GameObject>(
							"enemy bullet", gameObject.x, gameObject.y);

						bullet2->AddSprite(tc[1]);					//�e�N�X�`��
						bullet2->priority = prioEnemyBullet;			//�D�揇��
						bullet2->AddCollision({ -10,-10,10,10 });	//�����蔻��

						auto move2 = bullet2->AddComponent<CrossMove>();	//�ړ�
						move2->Boss = boss;
						move2->target = target;
						move2->vx = -speed;
						move2->vy = speed;

						bullet2->AddComponent<OutOfScreen>();		//��ʊO

						auto damageSource2 = bullet2->AddComponent<DamageSource>();	//�_���[�W
						damageSource2->targetName = "player";	//�v���C���[���w��

						timerB -= intervalB;
						numberOfFire--;

					}
					else
					{
						timerB += deltaTime;

					}
				}
			}
			else if (gameObject.y > CenterY)
			{
				gameObject.y -= MoveSpeed * deltaTime;
			}
			else
			{
				if (Timer > 3)
				{
					//BulletPattern = rand() % 4;
					ChabgeAttack(1);

					Timer = 0;
				}
			}

		}

		//�~��`���Ȃ���U������
		void CircleShot(GameObject& gameObject, float deltaTime)
		{
			if (Timer == 0)
			{
				Angle = 0;
				numberOfFire = 30;
				intervalB = 0.1f;	//�A�A�ˊԊu

				//�ړ��O�̍��W����
				BeforeY = gameObject.y;

			}

			Timer += deltaTime;
			if (numberOfFire > 0)
			{
				if (Timer < 1)
				{
					//�ړ������i�C�[�W���O�j
					gameObject.y = BeforeY + Elastic(Timer) * (680 - BeforeY);

				}
				else
				{

					if (timerB > intervalB)
					{
						speed = 500;
						auto bullet = gameObject.engine->Create<GameObject>(
							"enemy bullet", gameObject.x, gameObject.y);

						bullet->AddSprite(tc[1]);					//�e�N�X�`��
						bullet->priority = prioEnemyBullet;			//�D�揇��
						bullet->AddCollision({ -10,-10,10,10 });	//�����蔻��

						angle = (Angle) * 3.14f / 180;
						auto move = bullet->AddComponent<CircleLerp>();	//�ړ�
						//move->vx = speed;
						//move->vy = speed;
						move->target = target;

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
			}
			else if (gameObject.y > CenterY)
			{
				gameObject.y -= MoveSpeed * deltaTime;
			}
			else
			{
				ChabgeAttack(2);
				Timer = 0;
			}

		}

		//�u���X������
		void BressShot(GameObject& gameObject, float deltaTime)
		{
			if (Timer == 0)
			{
				Angle = 0;
				numberOfFire = 200;
				intervalB = 0.01f;	//�A�A�ˊԊu

			}

			Timer += deltaTime;
			if (numberOfFire > 0)
			{

				if (timerB > intervalB)
				{
					speed = 500 + static_cast<float>(sin(rand() % 360)) * 100;

					auto bullet = gameObject.engine->Create<GameObject>(
						"enemy bullet", gameObject.x, gameObject.y);

					bullet->AddSprite(tc[2]);					//�e�N�X�`��
					bullet->priority = prioEnemyBullet;			//�D�揇��
					bullet->AddCollision({ -10,-10,10,10 });	//�����蔻��

					float disx = target->x - gameObject.x;
					float disy = target->y - gameObject.y;
					angle = atan2(disy, disx);
					Angle = static_cast<float>(angle * 180) / 3.14f;
					Angle += static_cast<float>(sin(rand() % 360)) * 5;
					angle = Angle * 3.14f / 180;

					auto move = bullet->AddComponent<Move>();	//�ړ�
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
			else if (Timer > 5)
			{
				ChabgeAttack(3);
				Timer = 0;
			}
		}

		//�ŏ��̓���
		void MoveStart(GameObject& gameObject, float deltaTime)
		{
			if (Timer == 0)
			{
				//���݂̈ʒu����
				BeforeX = gameObject.x;
				BeforeY = gameObject.y;
			}
			Timer += deltaTime;

			if (Timer <= 1)
			{
				gameObject.y = BeforeY + Circle(Timer) * (500 - BeforeY);
			}
			else
			{
				MoveOK = true;
				vx = 1;
				vy = 1;
				Timer = 0;
				BulletPattern = 2;
			}

		}


};


#endif //COMPONENT_BOSSMOVE5_H_INCLUDED
