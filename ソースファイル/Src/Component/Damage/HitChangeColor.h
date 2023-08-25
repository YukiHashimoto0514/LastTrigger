//@file HitChangeColor.h

#ifndef COMPONENT_HITCHANGECOLOR_H_INCLUDED
#define COMPONENT_HITCHANGECOLOR_H_INCLUDED
#include "../../Application/Component.h"
#include "../../Application/GameObject.h"

//ダメージ源コンポーネント

class HitChangeColor :public Component
{
public:
	HitChangeColor() = default;//コンストラクタ
	virtual ~HitChangeColor() = default;//デストラクタ

	virtual void Update(GameObject& gameObject, float deltatime)override
	{
		//2Dオブジェクト用	
		if (gameObject.Red > 1)
		{
			gameObject.Red -= deltatime * 100;
		}
		else
		{
			gameObject.Red = 1;
		}

		//3Dモデルの色変え
		auto& ra = static_cast<MeshRenderer&>(*gameObject.componentList[0]);
		if (ra.materials[0]->baseColor.x > 1)
		{
			for (int i = 0; i < ra.materials.size(); i++)
			{
				//赤色を元に戻す
				ra.materials[i]->baseColor.x -= deltatime * 100;
			}
		}
		else
		{
			for (int i = 0; i < ra.materials.size(); i++)
			{
				//元の色に戻す
				ra.materials[i]->baseColor = VecMath::vec4(1);
			}
		}
	}



};




#endif  //COMPONENT_HITCHANGECOLOR_H_INCLUDED