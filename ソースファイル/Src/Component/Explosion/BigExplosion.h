//@file BigExplosion.h

#ifndef COMPONENT_BIGEXPLOSION_H_INCLUDED
#define COMPONENT_BIGEXPLOSION_H_INCLUDED
#include "../../Application/Component.h"
#include "../../Engine/SpritePriority.h"
#include "../../Application/GameObject.h"
#include "../../Engine/Engine.h"
#include "../Animation/Animator2D.h"

//�����R���|�[�l���g

class BigExplosion :public Component
{
public:
	BigExplosion() = default;			//�R���X�g���N�^
	virtual ~BigExplosion() = default;	//�f�X�g���N�^

	virtual void OnDestroy(GameObject& gameObject)override
	{
		//��ʊO�Ŏ��񂾂�A�G�t�F�N�g���Đ����Ȃ�
		if (gameObject.Point == 0)
		{
			return;
		}

		//�����A�j���[�V�����f�[�^
		const Texcoord tc[] = {
			MakeTexcoord(303,400,32,32),
			MakeTexcoord(335,400,32,32),
			MakeTexcoord(369,400,48,48),
			MakeTexcoord(417,400,48,48),
			MakeTexcoord(465,400,48,48),
		};

		//�����̕\�����W�f�[�^
		const float x[] = { 0,-20,20,-25,27 };
		const float y[] = { 0,20,30,-24,-15 };

		//������\��
		int i = static_cast<int>(std::size(x));

		if (count < i)
		{
			i = count;
		}

		for (--i; i >= 0; --i)
		{
			auto explosion = gameObject.engine->Create<GameObject>(
				"explosion", gameObject.x, gameObject.y);

			explosion->AddSprite(tc[0]);			//�����摜
			explosion->priority = prioExplosion;	//�D�揇��
			explosion->Scale = scale;				//�����摜�̑傫��
			auto anim = explosion->AddComponent<Animator2D>();	//�A�j���[�V��������������
			anim->loopTime = false;		//���[�v�Đ����Ȃ�
			anim->killWhenEnd = true;	//�Đ��������ɃQ�[���I�u�W�F�N�g���폜
			anim->clip.resize(std::size(tc));
			anim->interval = 0.07f;		//�A�j���[�V�����̍Đ����x
			for (size_t frameNo = 0; frameNo < std::size(tc); ++frameNo)
			{
				anim->AddSprite(frameNo, tc[frameNo], x[i], y[i]);
			}
		}
	}

	int count = 4;	//������
	float scale = 1;//�傫��
};



#endif // !COMPONENT_BIGEXPLOSION_H_INCLUDED
