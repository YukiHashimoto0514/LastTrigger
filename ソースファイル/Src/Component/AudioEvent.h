//@file AudioEvent.h

#ifndef COMPONENT_AUDIOEVENT_H_INCLUDED
#define COMPONENT_AUDIOEVENT_H_INCLUDED
#include "../Application/Component.h"
#include "../Application/EasyAudio.h"
#include <string>

//�����Đ��R���|�[�l���g
class AudioEvent :public Component
{
public:
	AudioEvent() = default;
	virtual ~AudioEvent() = default;

	virtual void OnCollision(GameObject&, GameObject&)override
	{
		if (!files.onCollision.empty())
		{
			Audio::PlayOneShot(files.onCollision.c_str());
		}
	}

	virtual void OnDestroy(GameObject& gameObject)override
	{
		//��ʊO�Ŕj�󂳂ꂽ�特�͖炳�Ȃ�
		if (gameObject.Point == 0)
		{
			return;
		}

		if (!files.onDestory.empty())
		{
			Audio::PlayOneShot(files.onDestory.c_str());
		}
	}

	struct {
		std::string onCollision;	//�Փˎ��̉����t�@�C����
		std::string onTakeDamage;	//�_���[�W���󂯂����̉����t�@�C����
		std::string onDestory;		//�j�󎞂̉����t�@�C����
	}files;
};

#endif // !COMPONENT_AUDIOEVENT_H_INCLUDED
