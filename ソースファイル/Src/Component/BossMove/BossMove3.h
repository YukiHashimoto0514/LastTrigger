//@file BossMove3.h

#ifndef COMPONENT_BOSSMOVE3_H_INCLUDED
#define COMPONENT_BOSSMOVE3_H_INCLUDED

#include "../../Application/Component.h"
#include "../../Application/GameObject.h"
#include <time.h>

//�����_������ɂ����{�X
class BossMove3 :public Component
{
public:
	BossMove3() = default;			//�R���X�g���N�^
	virtual ~BossMove3() = default;	//�f�X�g���N�^

	virtual void Start(GameObject& gameObject)override
	{
		MovePattern = 100;
		CenterX = 272;
		CenterY = 500;
		srand((unsigned int)time(NULL));
	}

	virtual void Update(GameObject& gameObject, float deltaTime)override
	{
		if (gameObject.PinchFlg)
		{
			speed = 300;

			if (!isOnce)
			{
				MovePattern = 50;
				Timer = 0;
				isOnce = true;
			}
		}

		switch (MovePattern)
		{
		//��������̓���
		case 0:

			if (Timer == 0)
			{
				rndMove = rand() % 5;
				Timer++;
				MoveSpeed = static_cast <float>(60 + (rand() % 40 - 11));//�{�X�̑������|10�`30�̒l�ŕω�������
				BulletPattern = rand() % 2;
			}


			//�ǂ̕����ɐi�ނ�
			if (rndMove == 0)		//��ɍs��
			{
				if (gameObject.y < 700)
				{
					gameObject.y += MoveSpeed * deltaTime;
				}
				else
				{
					MoveOK = true;
					MovePattern = 5;
				}
			}
			else if (rndMove == 1)	//����ɍs��
			{
				if (gameObject.x > LeftUpX)
				{
					gameObject.x -= MoveSpeed * deltaTime;
				}
				else
				{
					MoveOK = true;
					MovePattern = 1;
				}


				if (gameObject.y < UpY)
				{
					gameObject.y += MoveSpeed * deltaTime;
				}
			}
			else if (rndMove == 2) //����
			{
				if (gameObject.x > LeftDownX)
				{
					gameObject.x -= MoveSpeed * deltaTime;
				}

				if (gameObject.y > DownY)
				{
					gameObject.y -= MoveSpeed * deltaTime;
				}
				else
				{
					MoveOK = true;
					MovePattern = 2;

				}

			}
			else if (rndMove == 3) //�E��
			{
				if (gameObject.x < RightDownX)
				{
					gameObject.x += MoveSpeed * deltaTime;
				}

				if (gameObject.y > DownY)
				{
					gameObject.y -= MoveSpeed * deltaTime;
				}
				else
				{
					MoveOK = true;
					MovePattern = 3;

				}

			}
			else if (rndMove == 4) //�E��
			{
				if (gameObject.x < RightUpX)
				{
					gameObject.x += MoveSpeed * deltaTime;
				}
				else
				{
					MoveOK = true;
					MovePattern = 4;

				}

				if (gameObject.y < UpY)
				{
					gameObject.y += MoveSpeed * deltaTime;
				}

			}

			if (MoveOK)
			{
				Timer = 0;
				MoveOK = false;
			}

			break;

		//���ォ��̓���
		case 1:

			if (Timer == 0)
			{
				rndMove = rand() % 3;
				Timer++;
				MoveSpeed = static_cast <float>(60 + (rand() % 40 - 11));//�{�X�̑������|10�`30�̒l�ŕω�������
				BulletPattern = rand() % 2;

			}


			//�ǂ̕����ɐi�ނ�
			if (rndMove == 0)		//��ɍs��
			{
				if (gameObject.x < CenterX)
				{
					gameObject.x += MoveSpeed * deltaTime;
				}
				else
				{
					MoveOK = true;
					MovePattern = 5;

				}

				if (gameObject.y < 700)
				{
					gameObject.y += MoveSpeed * deltaTime;
				}


			}
			else if (rndMove == 1)	//�����ɍs��
			{
				if (gameObject.x > LeftDownX)
				{
					gameObject.x -= MoveSpeed * deltaTime;
				}

				if (gameObject.y > DownY)
				{
					gameObject.y -= MoveSpeed * deltaTime;
				}
				else
				{
					MoveOK = true;
					MovePattern = 2;

				}

			}
			else if (rndMove == 2) //�����ɍs��
			{
				if (gameObject.x < CenterX)
				{
					gameObject.x += MoveSpeed * deltaTime;
				}
				else
				{
					MoveOK = true;
					MovePattern = 0;

				}


				if (gameObject.y > CenterY)
				{
					gameObject.y -= MoveSpeed * deltaTime;
				}
			}

			//���̓����Ɉڍs����
			if (MoveOK)
			{
				Timer = 0;
				MoveOK = false;
			}


			break;

		//��������̓���
		case 2:

			if (Timer == 0)
			{
				rndMove = rand() % 3;
				Timer++;
				BulletPattern = rand() % 2;

				MoveSpeed = static_cast <float>(60 + (rand() % 40 - 11));//�{�X�̑������|10�`30�̒l�ŕω�������
			}


			//�ǂ̕����ɐi�ނ�
			if (rndMove == 0)		//����ɍs��
			{
				if (gameObject.x > LeftUpX)
				{
					gameObject.x -= MoveSpeed * deltaTime;
				}

				if (gameObject.y < UpY)
				{
					gameObject.y += MoveSpeed * deltaTime;
				}
				else
				{
					MoveOK = true;
					MovePattern = 1;

				}


			}
			else if (rndMove == 1)	//�E���ɍs��
			{
				if (gameObject.x < RightDownX)
				{
					gameObject.x += MoveSpeed * deltaTime;
				}
				else
				{
					MoveOK = true;
					MovePattern = 3;
				}
			}
			else if (rndMove == 2) //�����ɍs��
			{
				if (gameObject.x < CenterX)
				{
					gameObject.x += MoveSpeed * deltaTime;
				}


				if (gameObject.y < CenterY)
				{
					gameObject.y += MoveSpeed * deltaTime;
				}
				else
				{
					MoveOK = true;
					MovePattern = 0;

				}

			}

			//���̓����Ɉڍs����
			if (MoveOK)
			{
				MovePattern = 3;
				Timer = 0;
				MoveOK = false;
			}


			break;

		//�E������̓���
		case 3:

			if (Timer == 0)
			{
				rndMove = rand() % 3;
				Timer++;
				MoveSpeed = static_cast <float>(60 + (rand() % 40 - 11));//�{�X�̑������|10�`30�̒l�ŕω�������
				BulletPattern = rand() % 2;

			}


			//�ǂ̕����ɐi�ނ�
			if (rndMove == 0)		//�����ɍs��
			{
				if (gameObject.x > LeftDownX)
				{
					gameObject.x -= MoveSpeed * deltaTime;
				}
				else
				{
					MoveOK = true;
					MovePattern = 2;

				}

			}
			else if (rndMove == 1)	//�E��ɍs��
			{
				if (gameObject.x < RightUpX)
				{
					gameObject.x += MoveSpeed * deltaTime;
				}

				if (gameObject.y < UpY)
				{
					gameObject.y += MoveSpeed * deltaTime;
				}
				else
				{
					MoveOK = true;
					MovePattern = 4;

				}



			}
			else if (rndMove == 2) //�����ɍs��
			{
				if (gameObject.x > CenterX)
				{
					gameObject.x -= MoveSpeed * deltaTime;
				}


				if (gameObject.y < CenterY)
				{
					gameObject.y += MoveSpeed * deltaTime;
				}
				else
				{
					MoveOK = true;
					MovePattern = 0;

				}

			}

			//���̓����Ɉڍs����
			if (MoveOK)
			{
				MovePattern = 4;
				Timer = 0;
				MoveOK = false;
			}



			break;

		//�E�ォ��̓���
		case 4:

			if (Timer == 0)
			{
				rndMove = rand() % 3;
				Timer++;
				MoveSpeed = static_cast <float>(60 + (rand() % 40 - 11));//�{�X�̑������|10�`30�̒l�ŕω�������
				BulletPattern = rand() % 2;
			}

			//�ǂ̕����ɐi�ނ�
			if (rndMove == 0)		//�E���ɍs��
			{
				if (gameObject.x > RightDownX)
				{
					gameObject.x -= MoveSpeed * deltaTime;
				}

				if (gameObject.y > DownY)
				{
					gameObject.y -= MoveSpeed * deltaTime;
				}
				else
				{
					MoveOK = true;
					MovePattern = 3;

				}

			}
			else if (rndMove == 1)	//��ɍs��
			{
				if (gameObject.x > CenterX)
				{
					gameObject.x -= MoveSpeed * deltaTime;
				}
				else
				{
					MoveOK = true;
					MovePattern = 5;

				}

				if (gameObject.y < 700)
				{
					gameObject.y += MoveSpeed * deltaTime;
				}



			}
			else if (rndMove == 2) //�����ɍs��
			{
				if (gameObject.x > CenterX)
				{
					gameObject.x -= MoveSpeed * deltaTime;
				}
				else
				{
					MoveOK = true;
					MovePattern = 0;
				}


				if (gameObject.y > CenterY)
				{
					gameObject.y -= MoveSpeed * deltaTime;
				}

			}

			//���̓����Ɉڍs����
			if (MoveOK)
			{
				Timer = 0;
				MoveOK = false;
			}
			break;

		//�ォ��̓���
		case 5:
			if (Timer == 0)
			{
				rndMove = rand() % 3;
				Timer++;
				MoveSpeed = static_cast <float>(60 + (rand() % 40 - 11));//�{�X�̑������|10�`30�̒l�ŕω�������
				BulletPattern = 2;
			}

			//�ǂ̕����ɐi�ނ�
			if (rndMove == 0)	//�E��ɍs��
			{
				if (gameObject.x < RightUpX)
				{
					gameObject.x += MoveSpeed * deltaTime;
				}
				else
				{
					MoveOK = true;
					MovePattern = 4;

				}

				if (gameObject.y > UpY)
				{
					gameObject.y -= MoveSpeed * deltaTime;
				}

			}
			else if (rndMove == 1)	//����ɍs��
			{
				if (gameObject.x > LeftUpX)
				{
					gameObject.x -= MoveSpeed * deltaTime;
				}
				else
				{
					MoveOK = true;
					MovePattern = 1;

				}

				if (gameObject.y > UpY)
				{
					gameObject.y -= MoveSpeed * deltaTime;
				}
			}
			else if (rndMove == 2) //�����ɍs��
			{
				if (gameObject.y > CenterY)
				{
					gameObject.y -= MoveSpeed * deltaTime;
				}
				else
				{
					MoveOK = true;
					MovePattern = 0;
				}


			}

			//���̓����Ɉڍs����
			if (MoveOK)
			{
				Timer = 0;
				MoveOK = false;
			}

			break;

		//�^�񒆂ɖ߂�
		case 50:

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
				MovePattern = 0;
				Timer = 0;
			}


			break;


			//�ŏ��̈��̂݌Ă΂��
		case 100:
			//�������~��Ă���
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
				MovePattern = 0;
				Timer = 0;
			}
			break;
		}

		//�����v���C���[�����Ƀ����_��50��
		if (BulletPattern==0)
		{
			RandomShot(gameObject, deltaTime);
		}

		//���E����S�����ɔ���
		if (BulletPattern==1)
		{
			AllRangeShot(gameObject, deltaTime);
		}

		//�v���C���[�̂�������ɒe������
		if (BulletPattern == 2)
		{
			LockPlayerShot(gameObject, deltaTime);
		}


	}

	//���������ɐݒ肷��ϐ�
	int MovePattern = 100;	//�s���p�^�[��
	int rndMove = 0;		//�s���������

	//�e�̃e�N�X�`�����W
	Texcoord tc[2] = { MakeTexcoord(144, 365, 16, 16),
	};

	float intervalA = 1;	//���ˊԊu
	float intervalB = 0.1f;	//�A�A�ˊԊu
	int numberOfFire = 26;	//�A�ː�
	float speed = 200;		//�e�̑��x
	float x = 0;			//���ˈʒu�̂w���W
	float y = 0;			//���ˈʒu�̂x���W
	float MaxHP;
	GameObjectPtr target;	//�W�I

	//���s���ɕω�����ϐ�
	int BulletPattern = 2;
	float angle = 0;			//���ˊp�x
	bool ShotFlg;//true=���@false���҂�

	bool isFiring;			//���˃t���O
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

	float RightUpX = 440;
	float RightDownX = 380;
	float LeftDownX = 200;
	float LeftUpX = 180;;	
	float UpY = 500;
	float DownY = 400;
	bool MoveOK = false;							//�ړI�n�܂œ��B�������ǂ���true��OK false��NO
	int ShotCnt;			//����s�����邩

	//HP�������ɂȂ������Ɏg���ϐ�
	float BeforeX = 0;
	float BeforeY = 0;
	bool isOnce = false;

	private:

		//�v���C���[�̂�������Ƀ����_���Œe���T�O������
		void RandomShot(GameObject& gameObject, float deltaTime)
		{
			x = 0;
			y = -80;

			numberOfFire = 50;
			ShotFlg = true;
			if (ShotFlg)
			{
				//���˗p�^�C�}�[�����ˊԊu�𒴂�����A���˕ϐ���������
				timerA += deltaTime;//�^�C�}�[�����Z

				if (timerA >= intervalA)	//�^�C�}�[���C���^�[�o���𒴂�����
				{
					isFiring = true;		//���˃t���Otrue
					timerA -= intervalA;	//�^�C�}�[�����炷
					timerB = intervalB;		//�ꔭ�ڂ������ɔ��˂����
					fireCounter = numberOfFire;

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


				//�e�𔭎�
				auto bullet = gameObject.engine->Create<GameObject>(
					"enemy bullet", gameObject.x + x, gameObject.y);

				bullet->priority = prioPlayer;		//�D�揇��
				bullet->AddCollision({ -10,-10,10,10 });	//�����蔻��
				bullet->AddSprite(tc[0]);					//�e�N�X�`��
				float disX = target->x - gameObject.x;
				float disY = target->y - gameObject.y;

				Angle = atan2(disY, disX);
				angle = Angle * 180.0f / 3.14f;//�x�ɒ���
				angle += rand() % 60 - 30;//-30~29
				Angle = angle * 3.14f / 180;//���W�A����

				if (fireCounter % 2 == 0)
				{
					auto move = bullet->AddComponent<MoveAndStop>();	//�ړ�
					move->vx = speed * cos(Angle);
					move->vy = speed * sin(Angle);
				}
				else
				{
					auto move = bullet->AddComponent<MoveAndStop>();	//�ړ�
					move->vx = speed * cos(Angle);
					move->vy = speed * sin(Angle);
					move->Stop = 2;
				}
				bullet->AddComponent<OutOfScreen>();	//��ʊO

				auto damageSource = bullet->AddComponent<DamageSource>();	//�_���[�W
				damageSource->targetName = "player";	//�v���C���[���w��


				//�A�˃J�E���^�����炵�A�O�ȏ�ɂȂ����甭�˃��[�h����
				--fireCounter;

				if (fireCounter <= 0)
				{
					isFiring = false;	//���˃t���O��false�ɂ���
					ShotFlg = false;
				}
			}

		}

		//�Q��������S�����ɒe������
		void AllRangeShot(GameObject& gameObject, float deltaTime)
		{
			numberOfFire = 200;
			ShotFlg = true;
			if (ShotFlg)
			{
				//���˗p�^�C�}�[�����ˊԊu�𒴂�����A���˕ϐ���������
				timerA += deltaTime;//�^�C�}�[�����Z

				if (timerA >= intervalA)	//�^�C�}�[���C���^�[�o���𒴂�����
				{
					isFiring = true;		//���˃t���Otrue
					timerA -= intervalA;	//�^�C�}�[�����炷
					timerB = intervalB;		//�ꔭ�ڂ������ɔ��˂����
					fireCounter = numberOfFire;

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


				//�e�𔭎�
				x = 25;
				y = -100;
				auto bullet = gameObject.engine->Create<GameObject>(
					"enemy bullet", gameObject.x + x, gameObject.y);

				bullet->AddSprite(tc[0]);					//�e�N�X�`��
				bullet->priority = prioPlayer;		//�D�揇��
				bullet->AddCollision({ -10,-10,10,10 });	//�����蔻��

				float angle = static_cast<float>(rand() % 360);
				angle = angle * 3.14f / 180;
				auto move = bullet->AddComponent<MoveAndStop>();	//�ړ�
				move->vx = speed * cos(angle) - rand() % 100;
				move->vy = speed * sin(angle) - rand() % 100;

				bullet->AddComponent<OutOfScreen>();	//��ʊO

				auto damageSource = bullet->AddComponent<DamageSource>();	//�_���[�W
				damageSource->targetName = "player";	//�v���C���[���w��


				//�e�𔭎�
				auto bullet2 = gameObject.engine->Create<GameObject>(
					"enemy bullet2", gameObject.x - x, gameObject.y);

				bullet2->AddSprite(tc[0]);					//�e�N�X�`��
				bullet2->priority = prioPlayer;		//�D�揇��
				bullet2->AddCollision({ -10,-10,10,10 });	//�����蔻��

				angle = static_cast<float>(rand() % 360);
				angle = angle * 3.14f / 180;
				auto move2 = bullet2->AddComponent<MoveAndStop>();	//�ړ�
				move2->vx = speed * cos(angle) + rand() % 100;
				move2->vy = speed * sin(angle) + rand() % 100;
				move2->Stop = 2;
				bullet2->AddComponent<OutOfScreen>();	//��ʊO

				auto damageSource2 = bullet2->AddComponent<DamageSource>();	//�_���[�W
				damageSource2->targetName = "player";	//�v���C���[���w��

				//�A�˃J�E���^�����炵�A�O�ȏ�ɂȂ����甭�˃��[�h����
				--fireCounter;

				if (fireCounter <= 0)
				{
					isFiring = false;	//���˃t���O��false�ɂ���
					ShotFlg = false;
				}
			}

		}

		void LockPlayerShot(GameObject& gameObject, float deltaTime)
		{
			x = 0;
			y = -80;
			vx = target->x - gameObject.x;
			vy = target->y - gameObject.y;

			float rad = atan2(vy, vx);
			Angle = rad * 180 / 3.14f;
			float AngleChild = Angle;


			//11�����ɋʂ�ł�
			if (timerB > intervalB)
			{
				for (int i = -15; i < 20; i += 5)
				{
					auto bullet = gameObject.engine->Create<GameObject>(
						"enemy bullet", gameObject.x + x, gameObject.y);

					bullet->AddSprite(tc[0]);					//�e�N�X�`��
					bullet->priority = prioEnemyBullet;		//�D�揇��
					bullet->AddCollision({ -10,-10,10,10 });	//�����蔻��

					auto move = bullet->AddComponent<MoveAndStop>();	//�ړ�
					Angle = i + AngleChild;

					rad = Angle * 3.14f / 180;
					move->vx = cos(rad) * speed;
					move->vy = sin(rad) * speed;

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
};




#endif //COMPONENT_BOSSMOVE3_H_INCLUDED
