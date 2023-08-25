//@file standard_3D.vert

#version 450

//�V�F�[�_�[�ւ̓���
layout(location=0) in vec3 inPosition;
layout(location=1) in vec2 inTexcoord;
layout(location=2) in vec3 inNormal;

//�V�F�[�_�[����̏o��
layout(location=0) out vec3 outPosition;
layout(location=1) out vec2 outTexcoord;
layout(location=2) out vec3 outNormal;
layout(location=3) out vec3 outShadowPosition;

//�e�N�X�`��
layout(binding=0) uniform sampler2D texColor;
layout(binding=1) uniform sampler2DShadow texDepth;

//�v���O��������̓���
layout(location=0) uniform mat4 matModel;
layout(location=1) uniform mat4 matShadowTexture;

//x:����p�ɂ�鐅���g�嗦
//y:����p�ɂ�鐂���g�嗦
//z:���ߖ@�p�����[�^�[A
//w:���ߖ@�p�����[�^�[B
layout(location=3) uniform vec4 cameraData;
layout(location=4) uniform vec3 cameraPosition;	//�J�����̍��W
layout(location=5) uniform vec2 cameraSinCosX;	//�J������X����]




void main()
{

	//�g��k���Ɖ�]�ƕ��s�ړ�
	outPosition= vec3(matModel * vec4(inPosition, 1));

	//���[���h�@�����v�Z
	mat3 matNormal = transpose(inverse(mat3(matModel)));
	outNormal = normalize(matNormal * inNormal);

	//�e����p�̍��W���v�Z
	float worldShadowRange = 750 * 2;	//�[�x�}�b�v�̕`��͈�(CreateDepthMap�֐����Q��)
	float shadowTexelSize = 1 / float(textureSize(texDepth, 0).x);	//1�e�N�Z���T�C�Y
	float worldShadowTexelSize =	
		worldShadowRange * shadowTexelSize;	//�P�e�N�Z���̃T�C�Y�i���[���h���W�j

	outShadowPosition = vec3(matShadowTexture *
		vec4(outPosition + outNormal * worldShadowTexelSize, 1));

	//�J�������W�����_�ƂȂ�悤�ɕ��s�ړ�
	vec3 pos = outPosition - cameraPosition;

	//�J�����̉E������x���A���ʂ�z���ƂȂ�悤�ɉ�]
	float s =-cameraSinCosX.x;	//��]�����𔽎��v��肩�玞�v���ɕϊ�
	float c = cameraSinCosX.y;

	outPosition.x = pos.x;
	outPosition.y = pos.z * -s + pos.y * c;
	outPosition.z = pos.z * c + pos.y * s;

	outTexcoord = inTexcoord;

	gl_Position.x = outPosition.x * cameraData.x;
	gl_Position.y = outPosition.y * cameraData.y;
	gl_Position.z = -outPosition.z * cameraData.w + cameraData.z;

	gl_Position.w = -outPosition.z;

}

