//@file DamageSource.h

#ifndef COMPONENT_DAMAGESOURCE_H_INCLUDED
#define COMPONENT_DAMAGESOURCE_H_INCLUDED

#include "../../Application/Component.h"
#include "../../Application/GameObject.h"
#include "../../Application/VecMath.h"

//�_���[�W���R���|�[�l���g

class DamageSource :public Component
{
public: 
	DamageSource() = default;//�R���X�g���N�^
	virtual ~DamageSource() = default;//�f�X�g���N�^

	virtual void OnCollision(GameObject& gameObject, GameObject& other)override
	{
		//�^�[�Q�b�g���Ɠ������O�̃Q�[���I�u�W�F�N�g�Ƀ_���[�W��^����
		if (other.name == targetName)
		{
			//���G���Ԃ�菬�����Ȃ�
			if (other.UnbeatableTime <= 0)
			{
				other.TakeDamage(gameObject, { amount,0 });//amount��HP�����炷
				other.Red = 6;//�@�̂�Ԃ�����
				
				//�O�Ԗڂ͕K�����b�V�������_���[
				auto& ra = static_cast<MeshRenderer&>(*other.componentList[0]);
				for (int i = 0; i < ra.materials.size(); i++)
				{
					//�ԐF�ɕύX����
					ra.materials[i]->baseColor.x += 5.5;
				}
			}

			//�{�X�Ȃ�
			if (other.Point == 6)
			{
				//�F�ւ�
				AddColor(other);
			}


			//�v���C���[�Ȃ�A���G���Ԃ�݂���
			if (other.name == "player")
			{
				//���G���Ԃ�����������
				if (other.UnbeatableTime <= 0)
				{
					//���G���Ԃ�ݒ�
					other.UnbeatableTime = 100;

					//�v���C���[�݂̂̐F�ς�
					AddColor(other);
				}
			}

			if (isOnce)
			{
				gameObject.isDead = true;
			}
		}
	}

	
	std::string targetName;	//�_���[�W��^������Q�[���I�u�W�F�N�g��
	
	float amount = 1;		//�_���[�W��
	bool isOnce = true;		//��x�_���[�W��^�����玀�ʂ̂�

private:

	//�F�����Z����
	void AddColor(GameObject& other)
	{
		//�O�Ԗڂ͕K�����b�V�������_���[
		auto& ra = static_cast<MeshRenderer&>(*other.componentList[0]);
		for (int i = 0; i < ra.mesh->materials.size(); i++)
		{
			//�ԐF�ɕύX����
			ra.mesh->materials[i]->baseColor.x += 1.5;
		}

	}
};




#endif  COMPONENT_DAMAGESOURCE_H_INCLUDED