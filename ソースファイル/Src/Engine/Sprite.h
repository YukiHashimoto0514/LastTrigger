//@file Sprite.h

#ifndef  SPRITE_H_INCLUDED
#define SPRITE_H_INCLUDED
#include <vector>

//�}�`�ɕ\������͈͂�\���\����
//���l�̓e�N�X�`�����W�n�Ŏw�肷��
struct Texcoord
{
	float u, v;		//�e�N�X�`�����W
	float sx, sy;	//�摜�̑傫��
};

//�摜�\���p�f�[�^�i�X�v���C�g�j���i�[����\����
struct Sprite 
{
	float x, y, z, w;		//�摜��\��������W
	Texcoord texcoord;		//�e�N�X�`�����W�Ƒ傫��
	float size, red, green, blue;	//�T�C�Y�ƐF
};

//�X�v���C�g�z��
using SpriteList = std::vector<Sprite>;

//�e�N�X�`�����W���쐬���郉���_��(���W�͂҂����鐔�Ŏw��)
inline Texcoord MakeTexcoord(float u,float v,float sx,float sy)
{
	return Texcoord{ u / 1024,v / 1024,sx / 1024,sy / 1024 };
};


#endif // ! SPRITE_H_INCLUDED
