//@file BossMove.h

#ifndef COMPONENT_BOSSMOVE_H_INCLUDED
#define COMPONENT_BOSSMOVE_H_INCLUDED

#include "../../Application/Component.h"
#include "../../Application/GameObject.h"
#include <time.h>
#include "../Move/Lerp.h"

//�S�Ă𓥏P�����{�X
class BossMove :public Component
{
public:
	BossMove() = default;			//�R���X�g���N�^
	virtual ~BossMove() = default;	//�f�X�g���N�^

	virtual void Start(GameObject& gameObject)override
	{
		Pattern = 100;
		srand((unsigned int)time(NULL));
		CenterX = 272;
		CenterY = 500;

	}

	virtual void Update(GameObject& gameObject, float deltaTime)override
	{
		//HP��������؂������Ɉ�x�����Ă΂��
		if (gameObject.PinchFlg && !isOnce)
		{
			ShotCnt = 2;

			Timer = 0;
			Pattern = 50;
			TakeTime = 0;
			isOnce = true;
		}

		switch (Pattern)
		{
		//�v���C���[�̕����Ɋg�U�c������
		case 0:
			SideMoveAttack(gameObject, deltaTime);
		break;

		//���Ԃ݂����ɂ��邭����
		case 1:
			EddyAttack(gameObject, deltaTime);
			break;

		//�ォ��߂��ጂ��
		case 2:
			BressShot(gameObject, deltaTime);
			break;

		//2�������烉���_���Ȓe�����
		case 3:
			RandomShot(gameObject, deltaTime);
			break;

		//������ɔ��˂���e�����
		case 4:
			UpBoundShot(gameObject, deltaTime);
			break;

		//�N���X�X�e�B���K�[
		case 5:
			CrossStinger(gameObject, deltaTime);
			break;

		//HP�������ɂȂ�����^�񒆂ɖ߂�
		case 50:
			HalfHP(gameObject, deltaTime);
			break;
		//�o��
		case 100:
			MoveStart(gameObject, deltaTime);
			break;

		}
	}

	//�U���p�^�[����ς���֐�
	void ChabgeAttack(int old)
	{
		while (old == Pattern)
		{
			Pattern = rand() % 6;
		}
	}


	//���������ɐݒ肷��ϐ�
	int Pattern = 100;//�s���p�^�[��
	int CenterX = 272;
	int CenterY = 500;
	int Right = 404;
	int Left = 180;
	//�e�̃e�N�X�`�����W
	Texcoord tc[2] = { MakeTexcoord(144, 365, 16, 16),
					   MakeTexcoord(144, 365, 16, 16) };

	float intervalA =6;		//���ˊԊu
	float intervalB = 0.1f;	//�A�A�ˊԊu
	int numberOfFire = 26;	//�A�ː�
	int ShotCnt = 1;
	float speed = 200;		//�e�̑��x
	float x = 0;			//���ˈʒu�̂w���W
	float y = 0;			//���ˈʒu�̂x���W

	GameObjectPtr target;	//�W�I
	GameObjectPtr Boss;		//�{�X

	//���s���ɕω�����ϐ�
	bool ShotFlg = false;	//true=���@false���҂�
	bool LRMove = true;		//���E�ǂ����Ɉړ����邩 true=���ɍs���@false���E�ɍs��

	float timerA = 0;		//���˗p�^�C�}�[
	float timerB = 0;		//�A�˗p�^�C�}�[
	int fireCounter = 0;	//�ː��J�E���^
	float vx = 0;			//���˃x�N�g���w
	float vy = 0;			//���˃x�N�g���x
	float Timer = 0;
	float Deg = 0;			//�x���@�̊p�x
	float Angle = 0;		//�ʓx�@�̊p�x
	float MoveSpeed = 80;

	bool isOnce = false;	//��x����̏����Ŏd�l

	//�C�[�W���O�Ɏg���ϐ�
	float BeforeX = 0;
	float BeforeY = 0;
	float TakeTime = 0;
	bool EasingFlg = false;

	private:
		//���E�Ɉړ����čU��
		void SideMoveAttack(GameObject& gameObject, float deltaTime)
		{
			//�Q��E�ɂ�����
			if (Timer == 2)
			{
				if (gameObject.x < CenterX)
				{
					Timer = 0;
					ShotFlg = false;
					ChabgeAttack(0);
				}
				else
				{
					gameObject.x -= MoveSpeed * deltaTime;
				}
			}
			else
			{
				//�����ɍs������
				if (gameObject.x <= Left)
				{
					LRMove = false;
					ShotFlg = true;
				}
				else if (gameObject.x >= Right)
				{
					LRMove = true;
					ShotFlg = true;
					Timer++;
				}

				if (LRMove)	//���ɍs��
				{
					gameObject.x -= MoveSpeed * deltaTime;
				}
				else
				{
					gameObject.x += MoveSpeed * deltaTime;
				}

				//�e�𔭎�
				if (ShotFlg)
				{
					//�ړ��l��ݒ�
					if (gameObject.PinchFlg)
					{
						speed = 600;
					}
					else
					{
						speed = 500;
					}

					//�p�x���Z�o
					vx = target->x - gameObject.x;
					vy = target->y - gameObject.y;
					Deg = atan2(vy, vx);
					Deg = Deg * 180 / 3.14f;	//�x���@�ɕϊ�

					for (Angle = Deg - 30; Angle <= Deg + 30; Angle += 7)
					{
						auto bullet = gameObject.engine->Create<GameObject>(
							"enemy bullet", gameObject.x, gameObject.y);

						bullet->AddSprite(tc[0]);					//�e�N�X�`��
						bullet->priority = prioEnemyBullet;			//�D�揇��
						bullet->AddCollision({ -10,-10,10,10 });	//�����蔻��

						const float rad = Angle * 3.14f / 180;		//�ʓx�@�ɕϊ�
						auto move = bullet->AddComponent<Move>();	//�ړ�
						move->vx = speed * cos(rad);
						move->vy = speed * sin(rad);

						bullet->AddComponent<OutOfScreen>();		//��ʊO

						auto damageSource = bullet->AddComponent<DamageSource>();	//�_���[�W
						damageSource->targetName = "player";	//�v���C���[���w��
					}
					ShotFlg = false;
				}
			}

		}

		//�Q�U��
		void EddyAttack(GameObject& gameObject, float deltaTime)
		{
			if (Timer == 0)
			{
				intervalB = 0.1f;	//�A�ˊԊu
				BeforeX = gameObject.x;
				BeforeY = gameObject.y;
				TakeTime = 0;
				Angle = 0;
				Timer++;
			}

			if (Timer < ShotCnt + 1)
			{
				TakeTime += deltaTime;
				//��ʂ̒��S�ɍs��
				if (TakeTime <= 1)
				{
					//�ړ������i�C�[�W���O�j
					gameObject.y = BeforeY + Bound(TakeTime) * (400 - BeforeY);
				}
				else
				{
					//���������
					if (Deg < 360)
					{
						//�Q���������Ȃ���e�𔭎˂�����
						timerB += deltaTime;//�^�C�}�[�����Z
						if (timerB >= intervalB)	//�^�C�}�[���C���^�[�o���𒴂�����
						{
							speed = 400;
							//�e�𔭎�
							auto bullet = gameObject.engine->Create<GameObject>(
								"enemy bullet", gameObject.x + x, gameObject.y + y);

							bullet->AddSprite(tc[0]);				//�e�N�X�`��
							bullet->priority = prioEnemyBullet;		//�D�揇��
							bullet->AddCollision({ -10,-10,10,10 });//�����蔻��

							auto move = bullet->AddComponent<Move>();	//�ړ�
							Angle = Deg * 3.14f / 180;	//�ʓx�@�ɕϊ�
							move->vx = cos(Angle) * speed;
							move->vy = sin(Angle) * speed;

							bullet->AddComponent<OutOfScreen>();	//��ʊO

							auto damageSource = bullet->AddComponent<DamageSource>();	//�_���[�W
							damageSource->targetName = "player";	//�v���C���[���w��


							//�e�𔭎�
							auto bullet3 = gameObject.engine->Create<GameObject>(
								"enemy bullet", gameObject.x + x, gameObject.y + y);

							bullet3->AddSprite(tc[0]);					//�e�N�X�`��
							bullet3->priority = prioEnemyBullet;		//�D�揇��
							bullet3->AddCollision({ -10,-10,10,10 });	//�����蔻��

							auto move3 = bullet3->AddComponent<Move>();	//�ړ�
							Angle = (Deg + 90) * 3.14f / 180;			//�ʓx�@�ɕϊ�
							move3->vx = cos(Angle) * speed;
							move3->vy = sin(Angle) * speed;

							bullet3->AddComponent<OutOfScreen>();	//��ʊO

							auto damageSource3 = bullet3->AddComponent<DamageSource>();	//�_���[�W
							damageSource3->targetName = "player";	//�v���C���[���w��


								//�e�𔭎�
							auto bullet2 = gameObject.engine->Create<GameObject>(
								"enemy bullet", gameObject.x + x, gameObject.y + y);

							bullet2->AddSprite(tc[0]);					//�e�N�X�`��
							bullet2->priority = prioEnemyBullet;		//�D�揇��
							bullet2->AddCollision({ -10,-10,10,10 });	//�����蔻��

							auto move2 = bullet2->AddComponent<Move>();	//�ړ�
							Angle = Deg * 3.14f / 180;					//�ʓx�@�ɕϊ�
							move2->vx = cos(Angle) * speed * (-1);		//���Ε����ɔ��˂��邽��*-1
							move2->vy = sin(Angle) * speed * (-1);		//���Ε����ɔ��˂��邽��*-1

							bullet2->AddComponent<OutOfScreen>();	//��ʊO

							auto damageSource2 = bullet2->AddComponent<DamageSource>();	//�_���[�W
							damageSource2->targetName = "player";	//�v���C���[���w��


															//�e�𔭎�
							auto bullet4 = gameObject.engine->Create<GameObject>(
								"enemy bullet", gameObject.x + x, gameObject.y + y);

							bullet4->AddSprite(tc[0]);					//�e�N�X�`��
							bullet4->priority = prioEnemyBullet;		//�D�揇��
							bullet4->AddCollision({ -10,-10,10,10 });	//�����蔻��

							auto move4 = bullet4->AddComponent<Move>();	//�ړ�
							Angle = (Deg + 90) * 3.14f / 180;			//�ʓx�@�ɕϊ�(�����p�x�����炵�Ĕ���)
							move4->vx = cos(Angle) * speed * (-1);
							move4->vy = sin(Angle) * speed * (-1);

							bullet4->AddComponent<OutOfScreen>();	//��ʊO

							auto damageSource4 = bullet4->AddComponent<DamageSource>();	//�_���[�W
							damageSource4->targetName = "player";	//�v���C���[���w��

							if (gameObject.PinchFlg)
							{
								//�e�𔭎�
								auto bullet = gameObject.engine->Create<GameObject>(
									"enemy bullet", gameObject.x + x, gameObject.y + y);

								bullet->AddSprite(tc[1]);					//�e�N�X�`��
								bullet->priority = prioEnemyBullet;		//�D�揇��
								bullet->AddCollision({ -10,-10,10,10 });	//�����蔻��

								auto move = bullet->AddComponent<Move>();	//�ړ�
								Angle = (Deg + 45) * 3.14f / 180;
								move->vx = cos(Angle) * speed;
								move->vy = sin(Angle) * speed;

								bullet->AddComponent<OutOfScreen>();	//��ʊO

								auto damageSource = bullet->AddComponent<DamageSource>();	//�_���[�W
								damageSource->targetName = "player";	//�v���C���[���w��


								//�e�𔭎�
								auto bullet3 = gameObject.engine->Create<GameObject>(
									"enemy bullet", gameObject.x + x, gameObject.y + y);

								bullet3->AddSprite(tc[1]);					//�e�N�X�`��
								bullet3->priority = prioEnemyBullet;		//�D�揇��
								bullet3->AddCollision({ -10,-10,10,10 });	//�����蔻��

								auto move3 = bullet3->AddComponent<Move>();	//�ړ�
								Angle = (Deg + 135) * 3.14f / 180;
								move3->vx = cos(Angle) * speed;
								move3->vy = sin(Angle) * speed;

								bullet3->AddComponent<OutOfScreen>();	//��ʊO

								auto damageSource3 = bullet3->AddComponent<DamageSource>();	//�_���[�W
								damageSource3->targetName = "player";	//�v���C���[���w��


									//�e�𔭎�
								auto bullet2 = gameObject.engine->Create<GameObject>(
									"enemy bullet", gameObject.x + x, gameObject.y + y);

								bullet2->AddSprite(tc[1]);					//�e�N�X�`��
								bullet2->priority = prioEnemyBullet;		//�D�揇��
								bullet2->AddCollision({ -10,-10,10,10 });	//�����蔻��

								auto move2 = bullet2->AddComponent<Move>();	//�ړ�
								Angle = (Deg + 45) * 3.14f / 180;
								move2->vx = cos(Angle) * speed * (-1);
								move2->vy = sin(Angle) * speed * (-1);

								bullet2->AddComponent<OutOfScreen>();	//��ʊO

								auto damageSource2 = bullet2->AddComponent<DamageSource>();	//�_���[�W
								damageSource2->targetName = "player";	//�v���C���[���w��


																//�e�𔭎�
								auto bullet4 = gameObject.engine->Create<GameObject>(
									"enemy bullet", gameObject.x + x, gameObject.y + y);

								bullet4->AddSprite(tc[1]);					//�e�N�X�`��
								bullet4->priority = prioEnemyBullet;		//�D�揇��
								bullet4->AddCollision({ -10,-10,10,10 });	//�����蔻��

								auto move4 = bullet4->AddComponent<Move>();	//�ړ�
								Angle = (Deg + 135) * 3.14f / 180;
								move4->vx = cos(Angle) * speed * (-1);
								move4->vy = sin(Angle) * speed * (-1);

								bullet4->AddComponent<OutOfScreen>();	//��ʊO

								auto damageSource4 = bullet4->AddComponent<DamageSource>();	//�_���[�W
								damageSource4->targetName = "player";	//�v���C���[���w��

							}
							timerB -= intervalB;	//�C���^�[�o�������炷
							Deg += 5;				//�p�x�𑫂�
						}
					}
					else
					{
						Angle = 0;
						Timer++;//����������������Z����
					}
				}
			}
			else
			{
				//���ɖ߂�Ȃ����̂܂܂���
				if (gameObject.y <= CenterY)
				{
					gameObject.y += MoveSpeed * deltaTime;
				}
				else
				{
					ChabgeAttack(1);
					Timer = 0;
					TakeTime = 0;
				}

			}

		}

		//�u���X�U��
		void BressShot(GameObject& gameObject, float deltaTime)
		{
			if (Timer == 0)
			{
				intervalB = 0.01f;
				BeforeY = gameObject.y;
				if (gameObject.PinchFlg)
				{
					numberOfFire = 30;
				}
				else
				{
					numberOfFire = 50;
				}
			}
			Timer += deltaTime;
			//��ɍs��
			if (numberOfFire > 0)
			{
				if (Timer < 1)
				{
					gameObject.y = BeforeY + Drop(Timer) * (700 - BeforeY);
				}
				else//�A�˂�����
				{
					if (timerB > intervalB)
					{
						//3�����ɋʂ�ł�
						for (int i = -20; i < 40; i += 20)
						{
							speed = 500 + static_cast<float>(sin(rand() % 360)) * 100;

							auto bullet = gameObject.engine->Create<GameObject>(
								"enemy bullet", gameObject.x, gameObject.y);

							bullet->AddSprite(tc[0]);					//�e�N�X�`��
							bullet->priority = prioEnemyBullet;			//�D�揇��
							bullet->AddCollision({ -10,-10,10,10 });	//�����蔻��

							float disx = target->x - gameObject.x;
							float disy = target->y - gameObject.y;
							Angle = atan2(disy, disx);
							Deg = (static_cast<float>(Angle * 180) / 3.14f) + i;
							Deg += static_cast<float>(sin(rand() % 360)) * 5;
							Angle = Deg * 3.14f / 180;

							auto move = bullet->AddComponent<Move>();	//�ړ�
							move->vx = speed * cos(Angle);
							move->vy = speed * sin(Angle);


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

				//�C�[�W���O�̂��߂ɍ��̍��W������
				BeforeY = gameObject.y;
				TakeTime = 0;
			}
			else    //�A�˂��I������A��ʒu�ɖ߂�
			{
				TakeTime += deltaTime;
				if (TakeTime <= 1)
				{
					gameObject.y = BeforeY + Drop(TakeTime) * (CenterY - BeforeY);
				}
				else
				{
					Timer = 0;
					TakeTime = 0;
					ChabgeAttack(2);
				}
			}

		}

		//�Q�������烉���_���ɍU��
		void RandomShot(GameObject& gameObject, float deltaTime)
		{
			if (Timer == 0)
			{
				Deg = 0;
				if (gameObject.PinchFlg)
				{
					intervalB = 0.05f;	//�A�A�ˊԊu
				}
				else
				{
					intervalB = 0.1f;	//�A�A�ˊԊu
				}
				numberOfFire = 80;
			}
			Timer += deltaTime;

			if (numberOfFire > 0)
			{
				if (timerB > intervalB)
				{
					speed = 400;

					//�e�𔭎�
					x = 100;
					y = -50;
					auto bullet = gameObject.engine->Create<GameObject>(
						"enemy bullet", gameObject.x + x, gameObject.y + y);

					bullet->AddSprite(tc[0]);					//�e�N�X�`��
					bullet->priority = prioPlayer;		//�D�揇��
					bullet->AddCollision({ -10,-10,10,10 });	//�����蔻��

					int angle = rand() % 360;
					angle = static_cast<int>(angle * 3.14f / 180);
					auto move = bullet->AddComponent<WallThroughMove>();	//�ړ�
					move->vx = speed * static_cast<float>(cos(angle)) - rand() % 100;
					move->vy = speed * static_cast<float>(sin(angle)) - rand() % 100;
					move->Counter = 1;

					bullet->AddComponent<OutOfScreen>();	//��ʊO

					auto damageSource = bullet->AddComponent<DamageSource>();	//�_���[�W
					damageSource->targetName = "player";	//�v���C���[���w��


					//�e�𔭎�
					auto bullet2 = gameObject.engine->Create<GameObject>(
						"enemy bullet2", gameObject.x - x, gameObject.y + y);

					bullet2->AddSprite(tc[1]);					//�e�N�X�`��
					bullet2->priority = prioPlayer;		//�D�揇��
					bullet2->AddCollision({ -10,-10,10,10 });	//�����蔻��

					angle = rand() % 360;
					angle = static_cast<int>(angle * 3.14f / 180);
					auto move2 = bullet2->AddComponent<WallThroughMove>();	//�ړ�
					move2->vx = speed * static_cast<float>(cos(angle)) + rand() % 100;
					move2->vy = speed * static_cast<float>(sin(angle)) + rand() % 100;
					move2->Counter = 1;

					bullet2->AddComponent<OutOfScreen>();	//��ʊO

					auto damageSource2 = bullet2->AddComponent<DamageSource>();	//�_���[�W
					damageSource2->targetName = "player";	//�v���C���[���w��

					//�A�˃J�E���^�����炵�A�O�ȏ�ɂȂ����甭�˃��[�h����
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
				ChabgeAttack(3);
				Timer = 0;
				x = 0;
				y = 0;
			}

		}

		//�N���X�X�e�B���K�[
		void CrossStinger(GameObject& gameObject, float deltaTime)
		{
			if (Timer == 0)
			{
				Angle = 0;
				BeforeX = gameObject.x;
				BeforeY = gameObject.y;
				TakeTime = 0;

				if (gameObject.PinchFlg)
				{
					numberOfFire = 25;
				}
				else
				{
					numberOfFire = 20;
				}
				intervalB = 0.1f;	//�A�A�ˊԊu
			}

			Timer += deltaTime;
			if (numberOfFire > 0)
			{
				if (Timer < 1)
				{
					gameObject.y = BeforeY + Bound(Timer) * (600 - BeforeY);
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
						move->Boss = Boss;
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
						move2->Boss = Boss;
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
				BeforeY = gameObject.y;

			}
			else
			{
				TakeTime += deltaTime;
				if (TakeTime <= 1)
				{
					gameObject.y = BeforeY + Bound(TakeTime) * (CenterY - BeforeY);
				}
				else
				{
					ChabgeAttack(5);
					Timer = 0;
				}
			}

		}

		//������ɔ��˒e������
		void UpBoundShot(GameObject& gameObject, float deltaTime)
		{
			if (Timer == 0)
			{
				Deg = 0;
				numberOfFire = 30;
				intervalB = 0.2f;	//�A�A�ˊԊu

			}
			Timer += deltaTime;

			if (numberOfFire > 0)
			{
				if (timerB > intervalB)
				{
					if (gameObject.PinchFlg)
					{
						speed = 600;
					}
					else
					{
						speed = 400;
					}
					if (numberOfFire % 2 == 0)
					{
						auto bullet = gameObject.engine->Create<GameObject>(
							"enemy bullet", gameObject.x, gameObject.y);

						bullet->AddSprite(tc[0]);					//�e�N�X�`��
						bullet->priority = prioEnemyBullet;			//�D�揇��
						bullet->AddCollision({ -10,-10,10,10 });	//�����蔻��

						Angle = (static_cast<int>(rand()) % 40 + 30) * 3.14f / 180;
						auto move = bullet->AddComponent<boundMove>();	//�ړ�
						move->vx = speed * cos(Angle);
						move->vy = speed * sin(Angle);


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

						Angle = (static_cast<int>(rand()) % 30 + 120) * 3.14f / 180;
						auto move = bullet->AddComponent<boundMove>();	//�ړ�
						move->vx = speed * cos(Angle);
						move->vy = speed * sin(Angle);

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
				ChabgeAttack(4);
				Timer = 0;
			}


		}

		//HP�������ȉ��ɂȂ������̍s��
		void HalfHP(GameObject& gameObject, float deltaTime)
		{
			if (Timer == 0)
			{
				//���݂̈ʒu����
				BeforeX = gameObject.x;
				BeforeY = gameObject.y;
			}

			if (Timer < 1)
			{
				Timer += deltaTime;
			}
			else
			{
				Timer = 1;
			}

			//�ړ������i�C�[�W���O�j
			gameObject.x = BeforeX + Elastic(Timer) * (CenterX - BeforeX);
			gameObject.y = BeforeY + Elastic(Timer) * (CenterY - BeforeY);

			if (Timer == 1)
			{
				Pattern = rand() % 6;
				Timer = 0;
				TakeTime = 0;
			}


		}

		//�s���J�n
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
				Pattern = rand() % 6;
			}

		}
};

#endif //COMPONENT_BOSSMOVE_H_INCLUDED
