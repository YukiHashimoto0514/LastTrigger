//@file Scene.h

#ifndef SCENE_H_INCLUDED
#define SCENE_H_INCLUDED
#include <memory>

//��s�錾
class Engine;
class GameObject;

using GameObjectPtr = std::shared_ptr<GameObject>;

//�V�[���̊��N���X
class Scene
{
public:
	Scene() = default;			//�R���X�g���N�^
	virtual ~Scene() = default;	//�f�X�g���N�^
	virtual bool Initialize(Engine& engine) { return 0; }		//������
	virtual void Update(Engine& engine, float deltaTime) = 0;	//�X�V����
	virtual void Finalize(Engine& engine){}						//�I������

};

using ScenePtr = std::shared_ptr<Scene>;	//�V�[���|�C���^�^


#endif //SCENE_H_INCLUDED