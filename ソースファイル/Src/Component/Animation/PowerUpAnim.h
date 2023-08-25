//@file PowerUpAnim.h

#ifndef COMPONENT_POWERUPANIM_H_INCLUDED
#define COMPONENT_POWERUPANIM_H_INCLUDED
#include "../../Application/Component.h"
#include "../../Engine/SpritePriority.h"
#include "../../Application/GameObject.h"
#include "../../Engine/Engine.h"
#include "Animator2D.h"

//�����R���|�[�l���g

class PowerUpAnim :public Component
{
public:
	PowerUpAnim() = default;			//�R���X�g���N�^
	virtual ~PowerUpAnim() = default;	//�f�X�g���N�^

	virtual void OnCollision(GameObject& gameObject, GameObject& other)override
	{
			//�A�j���[�V�����f�[�^
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

			//�����̕\�����W�f�[�^
			const float x[] = { 0 };
			const float y[] = { 0 };

			//������\��
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

					explosion->AddSprite(tc[0]);			//�����摜
					explosion->priority = prioExplosion;	//�D�揇��

					auto anim = explosion->AddComponent<Animator2D>();	//�A�j���[�V��������������
					anim->loopTime = false;		//���[�v�Đ����Ȃ�
					anim->killWhenEnd = true;	//�Đ��������ɃQ�[���I�u�W�F�N�g���폜
					anim->clip.resize(std::size(tc));
					anim->interval = 0.06f;
					for (size_t frameNo = 0; frameNo < std::size(tc); ++frameNo)
					{
						anim->AddSprite(frameNo, tc[frameNo], x[i], y[i]);
					}
				}

			}

	}
	int count = 0;	//�A�j���[�V�����̐�������
};



#endif // !COMPONENT_EXPLOSION_H_INCLUDED
