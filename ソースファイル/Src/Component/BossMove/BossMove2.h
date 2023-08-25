//@file BossMove2.h

#ifndef COMPONENT_BOSSMOVE2_H_INCLUDED
#define COMPONENT_BOSSMOVE2_H_INCLUDED

#include "../../Application/Component.h"
#include "../../Application/GameObject.h"
#include <time.h>
#include "../Move/Lerp.h"

//��{����ɂ�����ʓI�ȃ{�X
class BossMove2 :public Component
{
public:
	BossMove2() = default;			//�R���X�g���N�^
	virtual ~BossMove2() = default;	//�f�X�g���N�^

	virtual void Start(GameObject& gameObject)override
	{
		Pattern = 100;
		srand((unsigned int)time(NULL));
		CenterX = 272;
		CenterY = 500;
	}

	virtual void Update(GameObject& gameObject, float deltaTime)override
	{
		if (gameObject.PinchFlg)
		{
			MoveSpeed = 100;
			speed = 300;
			if (!isOnce)
			{
				Timer = 0;
				Pattern = 50;
				isOnce = true;
			}
		}
		else
		{
			MoveSpeed = 60;
		}

		switch (Pattern)
		{
	    //���Ɉړ�����
		case 0:
			SideMoveAttack(gameObject, deltaTime);
			break;

		//�㉺�Ɉړ�����
		case 1:
			WaveMoveAttack(gameObject, deltaTime);
			break;

		//�ォ��3�����U��
		case 2:
			ThreeBurst(gameObject, deltaTime);
			break;

			//���S����Q���U��
		case 3:
			EddyAttack(gameObject, deltaTime);
			break;

			
		//�^�񒆂ɖ߂�
		case 50:
			HalfHP(gameObject, deltaTime);

			break;

		//�ŏ��̈��̂݌Ă΂��
		case 100:
			MoveStart(gameObject, deltaTime);
			break;

		}
	}



	//���������ɐݒ肷��ϐ�
	int Pattern = 100;//�s���p�^�[��

	//�e�̃e�N�X�`�����W
	Texcoord tc[2] = { MakeTexcoord(144, 365, 16, 16),
					   MakeTexcoord(144, 365, 16, 16)
	                 };

	float intervalA = 6;	//���ˊԊu
	float intervalB = 0.1f;	//�A�A�ˊԊu
	int numberOfFire = 26;	//�A�ː�
	float speed = 200;		//�e�̑��x
	float x = 0;			//���ˈʒu�̂w���W
	float y = 0;			//���ˈʒu�̂x���W
	GameObjectPtr target;	//�W�I

	//���s���ɕω�����ϐ�
	float angle = 0;		//���ˊp�x
	bool ShotFlg = false;	//true=���@false���҂�

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

	int ShotCnt = 0;		//����s�����邩

	//HP�������ɂȂ������Ɏg���ϐ�
	float BeforeX = 0;
	float BeforeY = 0;
	bool isOnce = false;

	private:

		//�U���p�^�[����ς���֐�
		void ChabgeAttack(int old)
		{
			while (old == Pattern)
			{
				Pattern = rand() % 4;
			}
		}

		//���E�ɗh��Ȃ���U������
		void SideMoveAttack(GameObject& gameObject, float deltaTime)
		{
			//1��E�ɂ�����
			if (Timer == 1)
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
				if (gameObject.x <= 180)
				{
					LRMove = false;
					ShotFlg = true;
				}
				else if (gameObject.x >= 404)
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
					for (angle = 240; angle <= 300; angle += 4)
					{
						auto bullet = gameObject.engine->Create<GameObject>(
							"enemy bullet", gameObject.x, gameObject.y);

						bullet->AddSprite(tc[0]);					//�e�N�X�`��
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
					ShotFlg = false;
				}
			}

		}

		//�g��`���Ȃ���U������
		void WaveMoveAttack(GameObject& gameObject, float deltaTime)
		{
			Timer += deltaTime;

			if (Timer >= 6 && abs(gameObject.x - CenterX) <= 5 && abs(gameObject.y - CenterY) <= 5)
			{
				ChabgeAttack(1);
				Timer = 0;

			}
			else
			{
				Angle += 1.0f * 3.14f / 180.0f;
				gameObject.y = gameObject.y + sin(Angle) * 0.9f;

				if (gameObject.y == 600)
				{
					ShotFlg = true;
				}

				if (gameObject.x <= 200)
				{
					LRMove = false;
					ShotFlg = true;
				}
				else if (gameObject.x >= 404)
				{
					LRMove = true;
					ShotFlg = true;
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
					for (angle = 240; angle <= 300; angle += 2)
					{
						auto bullet = gameObject.engine->Create<GameObject>(
							"enemy bullet", gameObject.x, gameObject.y);

						bullet->AddSprite(tc[0]);					//�e�N�X�`��
						bullet->priority = prioEnemyBullet;			//�D�揇��
						bullet->AddCollision({ -10,-10,10,10 });	//�����蔻��

						const float rad = angle * 3.14f / 180;
						auto move = bullet->AddComponent<MoveAndStop>();	//�ړ�
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

		//�ォ��3�����ɍU������
		void ThreeBurst(GameObject& gameObject, float deltaTime)
		{

			//��ɍs��
			if (Timer < 30)
			{
				if (gameObject.y < 700)
				{
					gameObject.y += MoveSpeed * deltaTime;
				}
				else//�A�˂�����
				{
					if (timerB > intervalB)
					{
						vx = target->x - gameObject.x;
						vy = target->y - gameObject.y;

						float rad = atan2(vy, vx);
						Angle = rad * 180 / 3.14f;
						float AngleChild = Angle;


						if (!gameObject.PinchFlg)
						{
							//3�����ɋʂ�ł�
							for (int i = -30; i < 60; i += 30)
							{
								auto bullet = gameObject.engine->Create<GameObject>(
									"enemy bullet", gameObject.x + x, gameObject.y + y);

								bullet->AddSprite(tc[0]);					//�e�N�X�`��
								bullet->priority = prioEnemyBullet;		//�D�揇��
								bullet->AddCollision({ -10,-10,10,10 });	//�����蔻��

								auto move = bullet->AddComponent<Move>();	//�ړ�
								Angle = i + AngleChild;

								rad = Angle * 3.14f / 180;
								move->vx = cos(rad) * speed;
								move->vy = sin(rad) * speed;

								bullet->AddComponent<OutOfScreen>();	//��ʊO

								auto damageSource = bullet->AddComponent<DamageSource>();	//�_���[�W
								damageSource->targetName = "player";	//�v���C���[���w��
							}
						}
						else
						{
							//5�����ɋʂ�ł�
							for (int i = -30; i < 45; i += 15)
							{
								auto bullet = gameObject.engine->Create<GameObject>(
									"enemy bullet", gameObject.x + x, gameObject.y + y);

								if (abs(i) == 15)
								{
									bullet->AddSprite(tc[1]);					//�e�N�X�`��
								}
								else
								{
									bullet->AddSprite(tc[0]);				//�e�N�X�`��
								}
								bullet->priority = prioEnemyBullet;		//�D�揇��
								bullet->AddCollision({ -10,-10,10,10 });	//�����蔻��

								auto move = bullet->AddComponent<Move>();	//�ړ�
								Angle = i + AngleChild;

								rad = Angle * 3.14f / 180;
								move->vx = cos(rad) * speed;
								move->vy = sin(rad) * speed;

								bullet->AddComponent<OutOfScreen>();	//��ʊO

								auto damageSource = bullet->AddComponent<DamageSource>();	//�_���[�W
								damageSource->targetName = "player";	//�v���C���[���w��
							}

						}
						Timer++;
						timerB -= intervalB;
					}
					else
					{
						timerB += deltaTime;
					}
				}
			}

			//�A�˂��I������A��ʒu�ɖ߂�
			if (Timer >= 30)
			{

				if (gameObject.y > CenterY)
				{
					gameObject.y -= MoveSpeed * deltaTime;
				}
				else
				{
					ChabgeAttack(2);
					Timer = 0;
				}
			}

		}
		//�Q�U��
		void EddyAttack(GameObject& gameObject, float deltaTime)
		{
			if (gameObject.PinchFlg)
			{
				ShotCnt = 2;
			}
			else
			{
				ShotCnt = 1;
			}
			if (Timer < ShotCnt)
			{
				//��ʂ̒��S�ɍs��
				if (gameObject.y > 400)
				{
					gameObject.y -= MoveSpeed * deltaTime;
					Angle = 0;
				}
				else
				{
					if (Angle < 360)
					{
						//�Q���������Ȃ���e�𔭎˂�����
						timerB += deltaTime;//�^�C�}�[�����Z
						if (timerB >= intervalB)	//�^�C�}�[���C���^�[�o���𒴂�����
						{
							//�e�𔭎�
							auto bullet = gameObject.engine->Create<GameObject>(
								"enemy bullet", gameObject.x + x, gameObject.y + y);

							bullet->AddSprite(tc[0]);					//�e�N�X�`��
							bullet->priority = prioEnemyBullet;		//�D�揇��
							bullet->AddCollision({ -10,-10,10,10 });	//�����蔻��

							auto move = bullet->AddComponent<Move>();	//�ړ�
							angle = Angle * 3.14f / 180;
							move->vx = cos(angle) * speed;
							move->vy = sin(angle) * speed;

							bullet->AddComponent<OutOfScreen>();	//��ʊO

							auto damageSource = bullet->AddComponent<DamageSource>();	//�_���[�W
							damageSource->targetName = "player";	//�v���C���[���w��

							if (gameObject.PinchFlg)
							{
								//�e�𔭎�
								auto bullet = gameObject.engine->Create<GameObject>(
									"enemy bullet", gameObject.x + x, gameObject.y + y);

								bullet->AddSprite(tc[1]);					//�e�N�X�`��
								bullet->priority = prioEnemyBullet;		//�D�揇��
								bullet->AddCollision({ -10,-10,10,10 });	//�����蔻��

								auto move = bullet->AddComponent<Move>();	//�ړ�
								angle = Angle * 3.14f / 180;
								move->vx = cos(angle) * speed * (-1);
								move->vy = sin(angle) * speed * (-1);

								bullet->AddComponent<OutOfScreen>();	//��ʊO

								auto damageSource = bullet->AddComponent<DamageSource>();	//�_���[�W
								damageSource->targetName = "player";	//�v���C���[���w��

							}
							timerB -= intervalB;	//�C���^�[�o�������炷
							Angle += 5;				//�p�x�𑫂�

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
				if (gameObject.y < CenterY)
				{
					gameObject.y += MoveSpeed * deltaTime;
				}
				else
				{
					ChabgeAttack(3);
					Timer = 0;
				}

			}

		}

		//�s���J�n
		void MoveStart(GameObject& gameObject, float deltaTime)
		{
			if (Timer == 0)
			{
				BeforeY = gameObject.y;
			}
			Timer += deltaTime;

			if (Timer <= 1)
			{
				gameObject.y = BeforeY + Circle(Timer) * (500 - BeforeY);
			}
			else
			{
				Pattern = 2;
			}

		}

		//HP�������ɂȂ�����
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
				Pattern = rand() % 4;
				Timer = 0;
			}

		}
};




#endif //COMPONENT_BOSSMOVE_H_INCLUDED
