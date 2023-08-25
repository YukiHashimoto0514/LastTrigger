//@file HPBar.h

#ifndef COMPONENT_HPBAR_H_INCLUDED
#define COMPONENT_HPBAR_H_INCLUDED
#include "../../Application/Component.h"
#include "../../Application/GameObject.h"

//HP�o�[��\������R���|�[�l���g

class HPBar :public Component
{
public:
	HPBar() = default;			//�R���X�g���N�^
	virtual ~HPBar() = default;	//�f�X�g���N�^

	virtual void Start(GameObject& gameObject)override
	{
		GreenBar = gameObject.engine->Create<GameObject>(
			"HPBar", gameObject.x - 200, gameObject.y);
		GreenBar->AddSprite(tc[0]);
		GreenBar->priority = prioEnemy;
	}

	virtual void Update(GameObject& gameObject, float deltaTime)override
	{
		if (GreenBar->Red >= 1)
		{
			GreenBar->Red -= deltaTime * 2;
		}

		//HP�𓯊�������
		NowHP = gameObject.HP / MaxHP * 300;

		//�Q�[���I�u�W�F�N�g�ɂ��Ă���+���炵�ĉE�����猸��悤�ɂ��Ă���
		GreenBar->x = gameObject.x - ViewSizeX * 0.5f + (MaxHP * 0.5f + NowHP * 0.5f) - 125;

		GreenBar->y = gameObject.y;

		//�`��X�V
		tc[0] = MakeTexcoord(0, 220, NowHP, 10);	//�ΐF
		GreenBar->spriteList.clear();
		GreenBar->AddSprite(tc[0]);
	}

	virtual void OnTakeDamage(GameObject& gameObject, GameObject& other, const Damage& damage)override
	{
		if (gameObject.HP <= 0)
		{
			//���񂾂Ƃ�HP�o�[������
			GreenBar->spriteList.clear();
		}
	}

	float MaxHP = 0;		//�ő�HP
	float NowHP = 0;		//���݂�HP
	int HPScale = 30;		//HP�o�[�̊g�k
	const int ViewSizeX = 540;	//��ʃT�C�Y��X
	const int VIewSIzeY = 704;	//��ʃT�C�Y�̂�
	
	GameObjectPtr GreenBar;

	Texcoord tc[2] = { MakeTexcoord(0, 220, NowHP/2, 10),	//�ΐF
		   MakeTexcoord(0, 230, MaxHP, 10)		//�ԐF
	};

};


#endif // !COMPONENT_HPBAR_H_INCLUDED
