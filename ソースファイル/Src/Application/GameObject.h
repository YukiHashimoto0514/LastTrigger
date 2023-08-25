//@file GameObject.h
#ifndef GAMEOBJECT_H_INCLUDED
#define GAMEOBJECT_H_INCLUDED
#include "../Engine/Sprite.h"
#include "../Engine/Rect.h"
#include "Component.h"
#include <string>
#include <vector>
#include <memory>
#include <cmath>
#include <type_traits>

//��s�錾
class Engine;
class GameObject;
class MeshRenderer;
class ProgramPipeline;

using GameObjectPtr = std::shared_ptr<GameObject>;		//�Q�[���I�u�W�F�N�g�̃|�C���^
using GameObjectList = std::vector<GameObjectPtr>;		//�Q�[���I�u�W�F�N�g�̃|�C���^�̔z��
using MeshRendererPtr = std::shared_ptr<MeshRenderer>;	//�`��R���|�[�l���g

//�Q�[�����ɓo�ꂷ��l�X�ȃQ�[���I�u�W�F�g��\�����N���X
class GameObject
{
public:
	GameObject() = default;				//�R���X�g���N�^
	virtual ~GameObject() = default;	//�f�X�g���N�^

	//�C�x���g�̑���
	virtual void Start();							//�ŏ���Update�̒��O�ŌĂяo�����
	virtual void Update(float deltaTime);			//���t���[��1��Ăяo�����
	virtual void OnCollision(GameObject& object);	//�Փ˂��N�����Ƃ��ɌĂяo�����
	virtual void OnDestroy();						//�Q�[���I�u�W�F�N�g���G���W������폜�����Ƃ��ɌĂяo�����
	virtual void TakeDamage(GameObject& other, const Damage& damage);	//�_���[�W���󂯂��Ƃ��ɌĂяo�����

	bool IsStarted() const { return isStarted; }	//�n�܂��Ă��邩�ǂ���

	//�X�v���C�g�̑���
	void AddSprite(const Texcoord& tc,
		float x = 0, float y = 0, float scale = 1, float rotation = 0);

	const SpriteList& GetSprite() const { return spriteList; }

	//�R���W�����̑���
	void AddCollision(const Rect& r);
	const RectList& GetCollision()const { return collisionList; }
	const Rect& GetAabb()const { return aabb; }
	
	//�R���|�[�l���g�̑���
	template<typename T>
	std::shared_ptr<T> AddComponent()	//����͂ǂ�Ȍ^�ɂł��Ή��ł���֐��̏������i���g���ꏏ�̏ꍇ�j
	{
		std::shared_ptr<T> p = std::make_shared<T>();
		componentList.push_back(p);

		//���b�V�������_���̏ꍇ�͐�p���X�g�ɒǉ�
		if constexpr (std::is_base_of<MeshRenderer, T>::value)
		{
			meshRendererList.push_back(p);
		}
		return p;
	}

	//�`��
	void DrawStaticMesh(const ProgramPipeline& program) const;

	Engine* engine = nullptr;		//�G���W���̃A�h���X
	std::string name;				//�I�u�W�F�N�g��
	float x = 0;					//x���W
	float y = 0;					//y���W
	float z = 0;					//z���W
	float rotation[3] = { 0,0,0 };	//��]
	float radias = 0;				//�p�x
	float Scale = 1;				//�傫��
	float Red = 1;					//�ԐF
	float Green = 1;				//�ΐF
	float Blue = 1;					//�F
	float alpha = 1;				//�����x
	float HP = 1;					//�̗�
	int priority = 0;				//�\���D�揇��
	SpriteList spriteList;			//�X�v���C�g�z��
	ComponentList componentList;	//�R���|�[�l���g�z��
	bool isDead = false;			//���S�t���O
	int Point = 0;					//�X�R�A�{��

	int Power[5] = { 0,0,0,0,0 };	//�p���[�A�b�v��
	float UnbeatableTime = 0.0f;			//���G����
	bool PinchFlg = false;			//true=HP��������菬���� false=HP�������ȏ�

private:
	bool isStarted = false;			//�X�^�[�g���s�ς݃t���O
	RectList collisionList;			//�R���W�����z��
	Rect aabb = { 0,0,0,0 };		//�R���W�������E�{�b�N�X(�ŏ��̒����`)
	//   aabb = Axis Aligned Bounding Box

	std::vector<MeshRendererPtr> meshRendererList;
};




#endif //GAMEOBJECT_H_INCLUDED