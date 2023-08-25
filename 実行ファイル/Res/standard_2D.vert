
//@file standard_2D.vert

#version 450

//�V�F�[�_�[�ւ̓���
layout(location=0) in vec3 inPosition;//���_���W(in���[���肩����炤�Ƃ��ɂ���)
layout(location=1) in vec2 inTexcoord;

//�v���O��������̓���
layout(location=0) uniform float timer;
layout(location=1) uniform vec3 position;

//�V�F�[�_�[����̏o��
layout(location=0) out vec4 outColor;//���_�F
layout(location=1) out vec2 outTexcoord;

//��ʍ��W�n���N���b�v���W�n�ɕϊ����邽�߂̃p�����[�^
layout(location=2) uniform vec4 screenToClip;

//�e�N�X�`��
layout(binding=0) uniform sampler2D texColor;	//�摜�̑傫��

//�e�N�X�`�����W�\����
struct Texcoord
{
vec2 uv;
vec2 size;
};


//�摜�\���p�f�[�^�i�X�v���C�g�j���i�[����\����
struct Sprite
{
	vec4 position;
	Texcoord texcoord;
	vec4 variar;
};

//�X�v���C�g�pSSBO(�V�F�[�_�[�X�g���[�W�o�b�t�@�I�u�W�F�N�g)
layout(std430,binding=0) buffer spriteDataBlock
{
	Sprite spriteList[];
};


void main()
{


	outColor=vec4(spriteList[gl_InstanceID].variar.y,spriteList[gl_InstanceID].variar.z,spriteList[gl_InstanceID].variar.w,spriteList[gl_InstanceID].position.w);

	//�e�N�X�`�����W���v�Z
	Texcoord texcoord=spriteList[gl_InstanceID].texcoord;
	outTexcoord=texcoord.uv+(inTexcoord*texcoord.size);

	//�p�x
	float rad=spriteList[gl_InstanceID].position.z*3.14/180;

//	cos(rad);
//	sin(rad);

	//���_���W���v�Z
	gl_Position.x = inPosition.x*cos(rad)-inPosition.y*sin(rad);	//��]����
	gl_Position.y = inPosition.x*sin(rad)+inPosition.y*cos(rad);	//��]����
	gl_Position.z = inPosition.z;
	gl_Position.w = 1;

	//�}�`�̑傫�����摜�̑傫���Ɠ���������
	gl_Position.xy *= abs(texcoord.size) * textureSize(texColor, 0) * spriteList[gl_InstanceID].variar.x;	//�s�N�Z���ɕϊ�

	//�}�`��SSBO�̍��W�Ɉړ�
	gl_Position.x += spriteList[gl_InstanceID].position.x;
	gl_Position.y += spriteList[gl_InstanceID].position.y;

	//��ʍ��W�n����N���b�v���W�n�֕ϊ�
	gl_Position.xy=(gl_Position.xy*screenToClip.xy)+screenToClip.zw;
}