//@file Camera.h

#ifndef COMPONENT_CAMERA_H_INCLUDED
#define COMPONENT_CAMERA_H_INCLUDED

#include "../Application/Component.h"
#include <math.h>

//�r���[�|�[�g
struct Viewport
{
	int x;		//���������W
	int y;		//���������W
	int width;	//��
	int height;	//����
};

//�J�����R���|�[�l���g
class Camera : public Component
{
public:
	Camera() = default;				//�R���X�g���N�^
	virtual ~Camera() = default;	//�f�X�g���N�^

	virtual void Update(GameObject& gameObject, float deltaTime)override
	{
		//���ߖ@�p�����[�^�[A,B���v�Z
		A = -2 * far * near / (far - near);
		B = (far + near) / (far - near);

		//����p�ɂ��g�嗦���v�Z
		const float rad = fovY * acos(-1.0f) / 180;
		fovScaleY = 1 / tan(rad / 2);
		fovScaleX = aspect * fovScaleY;
	}


	float near = 1;			//�ߕ���
	float far = 1000;		//������
	float fovY = 60;		//����p
	float aspect = 9.0f / 16.0f;	//�A�X�y�N�g��
	Viewport viewport = { 0,0,1280,720 };

	//���j�t�H�[���ϐ��ɐݒ肷��p�����[�^�[
	float fovScaleX = 1;	//����p�ɂ��g�嗦
	float fovScaleY = 1;	//����p�ɂ��g�嗦
	float A = 0.01f;		//���ߖ@�p�����[�^�[A
	float B = 0.99f;		//���ߖ@�p�����[�^�[B


};

#endif //COMPONENT_CAMERA_H_INCLUDED




