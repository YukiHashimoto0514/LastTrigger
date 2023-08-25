//@file FramebufferObject.cpp

#include "FramebufferObject.h"
#include "../Application/Debug.h"

//FBO���쐬����

//@param width	�t���[���o�b�t�@�̕�  �i�s�N�Z�����j
//@param height	�t���[���o�b�t�@�̍����i�s�N�Z�����j
//@param type	FBO�̎��
	
//@return �쐬����FBO	
FramebufferObjectPtr FramebufferObject::Create(int width, int height,
	FboType type = FboType::colorDepth)
{
	return std::make_shared<FramebufferObject>(width, height, type);
}

//�R���X�g���N�^

//@param width	�t���[���o�b�t�@�̕�  �i�s�N�Z�����j
//@param height	�t���[���o�b�t�@�̍����i�s�N�Z�����j
//@param type	FBO�̎��
FramebufferObject::FramebufferObject(int width, int height, FboType type)
{
	//�J���[�e�N�X�`�����쐬
	if (type & FboType::color)
	{
		texColor = Texture::Create("FBO(Color)", width, height, GL_RGBA16F, GL_LINEAR);

		//texColor��null�ł͂Ȃ��A�Ǘ��ԍ���0�Ȃ�
		if (!texColor || !*texColor)
		{
			LOG_ERROR("FBO�p�J���[�e�N�X�`���̍쐬�Ɏ��s");
			texColor.reset();//�J���[�e�N�X�`����j��

			return;
		}
		texColor->SetWrapMode(GL_CLAMP_TO_EDGE);
	}

	//�[�x�e�N�X�`�����쐬
	if (type & FboType::depth)
	{
		texDepth = Texture::Create("FBO(Depth)", width, height,
			GL_DEPTH_COMPONENT32F, GL_LINEAR);

		if (!texDepth || !*texDepth)
		{
			LOG_ERROR("FBO�p�[�x�e�N�X�`���̍쐬�Ɏ��s");
			texColor.reset();	//�J���[�e�N�X�`����j��
			texDepth.reset();	//�[�x�e�N�X�`����j��

			return;
		}
		texDepth->SetWrapMode(GL_CLAMP_TO_EDGE);

	}
	//FBO���쐬
	glCreateFramebuffers(1, &fbo);//�쐬���A�i�[����A�h���X

	if (static_cast<int>(type) & 1)
	{
		                  //���蓖�Đ�A�p�r�A�e�N�X�`��ID�A�~�b�v�}�b�v���x��
		glNamedFramebufferTexture(fbo, GL_COLOR_ATTACHMENT0, *texColor, 0);
	}
	else
	{
		                      //������FBO�A�`���J���[�o�b�t�@
		glNamedFramebufferDrawBuffer(fbo, GL_NONE);
	}

	if (static_cast<int>(type) & 2)
	{
		glNamedFramebufferTexture(fbo, GL_DEPTH_ATTACHMENT, *texDepth, 0);
	}
	
	//FBO�̗L�������`�F�b�N
	                 //�`�F�b�N����FBO��ID�AID��0�����������Ƀ`�F�b�N����o�C���f�B���O�|�C���g
	if (glCheckNamedFramebufferStatus(fbo, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		LOG_ERROR("FBO�̍쐬�Ɏ��s");
		glDeleteFramebuffers(1, &fbo);
		fbo = 0;
		texColor.reset();
		texDepth.reset();

		return;
	}

	//�쐬����
	LOG("FBO(width=%d,height=%d)���쐬", width, height);
}

//�f�X�g���N�^
FramebufferObject::~FramebufferObject()
{
	       //�폜����FBO�̐��A�폜����FBO��ID�z��
	glDeleteFramebuffers(1, &fbo);
}