//@file Item.h

#ifndef COMPONENT_ITEM_H_INCLUDED
#define COMPONENT_ITEM_H_INCLUDED
#include "../Application/Component.h"
#include "../Engine/SpritePriority.h"
#include "../Application/GameObject.h"
#include "../Component/AudioEvent.h"
#include "../Application/EasyAudio.h"
#include "../Application/AudioSettings.h"

#include "../Engine/Engine.h"
#include "Animation/Animator2D.h"

//�����R���|�[�l���g

class Item :public Component
{
public:
	Item() = default;			//�R���X�g���N�^
	virtual ~Item() = default;	//�f�X�g���N�^


	//�A�C�e���摜�̈ʒu
	const Texcoord tc[5] = {
		MakeTexcoord(0,330,32,32),
		MakeTexcoord(32,330,32,32),
		MakeTexcoord(64,330,32,32),
		MakeTexcoord(0,330,32,32),
		MakeTexcoord(32,330,32,32),

	};

	virtual void Start(GameObject& gameObject)override
	{
		rnd = rand() % 5;
		Timer = 0;
	}
	virtual void Update(GameObject& gameObject, float deltatime)override
	{
		//�L�������痣�ꂽ�ꍇ
		if (!FollowFlg)
		{
			gameObject.y -= 100 * deltatime;
		}

		gameObject.AddSprite(tc[rnd]);			//�A�C�e���摜�������_���œE�o
	}

	virtual void OnCollision(GameObject& gameObject, GameObject& other)override
	{
		//�v���C���[���E������
		if (other.name == Upname)
		{
			Audio::PlayOneShot(SE::powerup);
			other.Power[rnd]++;//�K���Ƀp���[�A�b�v
			gameObject.isDead = true;
		}

		if (other.name == "enemy")
		{
			gameObject.y = other.y;
			gameObject.x = other.x;
			FollowFlg = true;
		}
		else
		{
			FollowFlg = false;
		}
	}


	int rnd = 0;//�`��p
	int Timer = 0;//���Ԍv��
	bool FollowFlg = true;	//true�����čs���Ă� false������Ă���
	std::string Upname = "player";

};


#endif // !COMPONENT_ITEM_H_INCLUDED
