//@fiel Animator2D.h

#ifndef COMPONENT_ANIMATOR2D_H_INCLUDED
#define COMPONENT_ANIMATOR2D_H_INCLUDED
#include "../../Application/Component.h"
#include "../../Application/GameObject.h"
#include "../../Engine/Sprite.h"
#include <vector>
#include <algorithm>


//2D�A�j���[�V�����R���|�[�l���g

class Animator2D :public Component
{
public:
	Animator2D() = default;				//�R���X�g���N�^
	virtual ~Animator2D() = default;	//�f�X�g���N�^

	virtual void Update(GameObject& gameObject, float deltaTime)override
	{
		//�f�[�^���Ȃ��ꍇ�͉������Ȃ�
		if (clip.empty())
		{
			return;
		}

		//�o�ߎ��Ԃ��X�V
		timer += deltaTime;

		//�Đ��ʒu���v�Z
		const int size = static_cast<int>(clip.size());
		int i = static_cast<int>(timer / interval);

		if (loopTime)
		{
			i %= size;//���[�v�Đ�
		}
		else
		{
			//�A�j���[�V�����I����killWhenEnd�t���O��true�Ȃ�Q�[���I�u�W�F�N�g���폜
			if (i >= size && killWhenEnd)
			{
				gameObject.isDead = true;
			}
			i = std::clamp(i, 0, size - 1);
		}

		//�X�v���C�g�f�[�^���Đ��ʒu�̃f�[�^�ɍX�V
		gameObject.spriteList = clip[i];

	}

	//�t���[���ɃX�v���C�g��ǉ�
	void AddSprite(size_t frameNo, const Texcoord& tc,
		float x = 0, float y = 0, float scale = 1, float rotation = 0)
	{
		if (frameNo >= clip.size())
		{
			clip.resize(frameNo + 1);
		}
		clip[frameNo].push_back({ x,y,0,0,tc });
	}

	bool loopTime = true;			//���[�v�Đ��̗L��
	bool killWhenEnd = false;		//�A�j���[�V�����I�����ɃQ�[���I�u�W�F�N�g���폜
	float timer = 0;				//�o�ߎ��ԁi�b�j
	float interval = 0.1f;			//�t���[����i�߂�Ԋu
	std::vector<SpriteList> clip;	//�A�j���[�V�����N���b�v
};


#endif // !COMPONENT_ANIMATOR2D_H_INCLUDED

