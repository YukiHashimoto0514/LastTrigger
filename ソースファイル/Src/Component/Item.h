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

//爆発コンポーネント

class Item :public Component
{
public:
	Item() = default;			//コンストラクタ
	virtual ~Item() = default;	//デストラクタ


	//アイテム画像の位置
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
		//キャラから離れた場合
		if (!FollowFlg)
		{
			gameObject.y -= 100 * deltatime;
		}

		gameObject.AddSprite(tc[rnd]);			//アイテム画像をランダムで摘出
	}

	virtual void OnCollision(GameObject& gameObject, GameObject& other)override
	{
		//プレイヤーが拾ったら
		if (other.name == Upname)
		{
			Audio::PlayOneShot(SE::powerup);
			other.Power[rnd]++;//適当にパワーアップ
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


	int rnd = 0;//描画用
	int Timer = 0;//時間計測
	bool FollowFlg = true;	//true＝ついて行ってる false＝離れている
	std::string Upname = "player";

};


#endif // !COMPONENT_ITEM_H_INCLUDED
