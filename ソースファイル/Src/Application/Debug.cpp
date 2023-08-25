//@file Debug.cpp

#include "Debug.h"
#include <Windows.h>
#include <stdio.h>
#include <Stdarg.h>

namespace Debug
{
	//�����t����������f�o�b�O�E�B���h�E�ɕ\������

	void Log(Type type, const char* func, const char* format, ...)
	{
		char buffer[1024];
		char* p = buffer;
		size_t size = sizeof(buffer) - 1;	//�����ɉ��s��ǉ����邽��

		//���O�̎�ނƊ֐�����ݒ�
		static const char* const typeNames[] = { "�G���[","�x��","���" };
		size_t n = snprintf(p, size, "[%s]%s:",
			typeNames[static_cast<int>(type)], func);
		p += n;
		size -= n;

		//���b�Z�[�W��ݒ�
		va_list ap;

		va_start(ap, format);	//va_list�̏�����
		//           �i�o�͐�̃A�h���X�A�o�͉\�ȃo�C�g���A�����t��������Ava_list�^�ϐ��j
		p += vsnprintf(p, size, format, ap);
		va_end(ap);				//va_list�̔j��

		//�����ɉ��s��ǉ�
		p[0] = '\n';
		p[1] = '\0';

		//�쐬������������f�o�b�O�E�B���h�E�ɕ\��
		OutputDebugString(buffer);

	}
}