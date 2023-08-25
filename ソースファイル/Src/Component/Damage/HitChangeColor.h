//@file HitChangeColor.h

#ifndef COMPONENT_HITCHANGECOLOR_H_INCLUDED
#define COMPONENT_HITCHANGECOLOR_H_INCLUDED
#include "../../Application/Component.h"
#include "../../Application/GameObject.h"

//�_���[�W���R���|�[�l���g

class HitChangeColor :public Component
{
public:
	HitChangeColor() = default;//�R���X�g���N�^
	virtual ~HitChangeColor() = default;//�f�X�g���N�^

	virtual void Update(GameObject& gameObject, float deltatime)override
	{
		//2D�I�u�W�F�N�g�p	
		if (gameObject.Red > 1)
		{
			gameObject.Red -= deltatime * 100;
		}
		else
		{
			gameObject.Red = 1;
		}

		//3D���f���̐F�ς�
		auto& ra = static_cast<MeshRenderer&>(*gameObject.componentList[0]);
		if (ra.materials[0]->baseColor.x > 1)
		{
			for (int i = 0; i < ra.materials.size(); i++)
			{
				//�ԐF�����ɖ߂�
				ra.materials[i]->baseColor.x -= deltatime * 100;
			}
		}
		else
		{
			for (int i = 0; i < ra.materials.size(); i++)
			{
				//���̐F�ɖ߂�
				ra.materials[i]->baseColor = VecMath::vec4(1);
			}
		}
	}



};




#endif  //COMPONENT_HITCHANGECOLOR_H_INCLUDED