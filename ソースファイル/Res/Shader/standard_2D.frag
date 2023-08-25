
//@file standard_2D.frag

#version 450

//�V�F�[�_�[�ւ̓���
layout(location=0) in vec4 inColor;
layout(location=1) in vec2 inTexcoord;

//�e�N�X�`���T���v��
layout(binding=0) uniform sampler2D texColor;

//�o�͂���F�f�[�^
out vec4 outColor;	//�ϐ��錾�iout=�C���q���[���������鎞�ɂ���j

void main()
{
	vec4 c=texture(texColor,inTexcoord);
	outColor=c*inColor;
}