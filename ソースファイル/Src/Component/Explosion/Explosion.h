//@file Explosion.h

#ifndef COMPONENT_EXPLOSION_H_INCLUDED
#define COMPONENT_EXPLOSION_H_INCLUDED
#include "../../Application/Component.h"
#include "../../Engine/SpritePriority.h"
#include "../../Application/GameObject.h"
#include "../../Engine/Engine.h"
#include "../Animation/Animator2D.h"

//爆発コンポーネント

class Explosion :public Component
{
public: 
	Explosion() = default;			//コンストラクタ
	virtual ~Explosion() = default;	//デストラクタ

	virtual void OnDestroy(GameObject& gameObject)override
	{
		//画面外で死んだら、エフェクトを再生しない
		if (gameObject.Point == 0)
		{
			return;
		}


		//爆発アニメーションデータ
		const Texcoord tc[] = {
			MakeTexcoord(320,368,32,32),
			MakeTexcoord(352,368,32,32),
			MakeTexcoord(384,368,32,32),
			MakeTexcoord(416,368,32,32),
			MakeTexcoord(448,368,32,32),
			MakeTexcoord(480,368,32,32),
		};

		//爆発の表示座標データ
		const float x[] = { 0,-12,20,-16,12 };
		const float y[] = { 0,20,12,-16,-20 };

		//爆発を表示
		int i = static_cast<int>(std::size(x));

		if (count < i)
		{
			i = count;
		}

		for (--i; i >= 0; --i)
		{
			auto explosion = gameObject.engine->Create<GameObject>(
				"explosion", gameObject.x, gameObject.y);

			explosion->AddSprite(tc[0]);			//爆発画像
			explosion->priority = prioExplosion;	//優先順位
			explosion->Scale = scale;				//爆発画像の大きさ

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

	int count = 4;	//爆発個数
	float scale = 1;	//爆発画像の大きさ
};



#endif // !COMPONENT_EXPLOSION_H_INCLUDED
