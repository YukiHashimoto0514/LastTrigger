//@file PowerUpAnim.h

#ifndef COMPONENT_POWERUPANIM_H_INCLUDED
#define COMPONENT_POWERUPANIM_H_INCLUDED
#include "../../Application/Component.h"
#include "../../Engine/SpritePriority.h"
#include "../../Application/GameObject.h"
#include "../../Engine/Engine.h"
#include "Animator2D.h"

//爆発コンポーネント

class PowerUpAnim :public Component
{
public:
	PowerUpAnim() = default;			//コンストラクタ
	virtual ~PowerUpAnim() = default;	//デストラクタ

	virtual void OnCollision(GameObject& gameObject, GameObject& other)override
	{
			//アニメーションデータ
			const Texcoord tc[] = {
				MakeTexcoord(0,720,64,64),
				MakeTexcoord(64,720,64,64),
				MakeTexcoord(128,720,64,64),
				MakeTexcoord(192,720,64,64),
				MakeTexcoord(256,720,64,64),
				MakeTexcoord(320,720,64,64),
				MakeTexcoord(384,720,64,64),
				MakeTexcoord(448,720,64,64),
				MakeTexcoord(512,720,64,64),
				MakeTexcoord(576,720,64,64),
			};

			//爆発の表示座標データ
			const float x[] = { 0 };
			const float y[] = { 0 };

			//爆発を表示
			int i = static_cast<int>(std::size(x));

			if (count < i)
			{
				i = count;
			}
			
			if (other.name == "item")
			{
				for (--i; i >= 0; --i)
				{
					auto explosion = gameObject.engine->Create<GameObject>(
						"explosion", gameObject.x, gameObject.y);

					explosion->AddSprite(tc[0]);			//爆発画像
					explosion->priority = prioExplosion;	//優先順位

					auto anim = explosion->AddComponent<Animator2D>();	//アニメーションを持たせる
					anim->loopTime = false;		//ループ再生しない
					anim->killWhenEnd = true;	//再生完了時にゲームオブジェクトを削除
					anim->clip.resize(std::size(tc));
					anim->interval = 0.06f;
					for (size_t frameNo = 0; frameNo < std::size(tc); ++frameNo)
					{
						anim->AddSprite(frameNo, tc[frameNo], x[i], y[i]);
					}
				}

			}

	}
	int count = 0;	//アニメーションの数を入れる
};



#endif // !COMPONENT_EXPLOSION_H_INCLUDED
