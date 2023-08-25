//@file BufferObject.h

#ifndef BUFFEROBJECT_H_INCLUDED
#define BUFFEROBJECT_H_INCLUDED

#include "glad/glad.h"
#include <memory>

//��s�錾
class BufferObject;
using BufferObjectPtr = std::shared_ptr<BufferObject>;

class MappedBufferObject;	//SSBO���Ǘ�����
using MappedBufferObjectPtr = std::shared_ptr<MappedBufferObject>;

class BufferObject
{
public:
	static BufferObjectPtr Create(GLsizeiptr size,
		const void* data = nullptr, GLbitfield flags = 0);

	//�R���X�g���N�^
	explicit BufferObject(GLsizeiptr size,
		const void* data = nullptr, GLbitfield flags = 0);

	//�f�X�g���N�^
	~BufferObject();

	operator GLuint() const { return id; }
	GLsizeiptr GetSize() const { return bufferSize; }

private:
	GLuint id = 0;
	GLsizeiptr bufferSize = 0;
};

//�o�b�t�@�I�u�W�F�N�g�i�}�b�v����j
class MappedBufferObject
{
public:
	static MappedBufferObjectPtr Create(
		GLsizeiptr size, GLenum type, GLenum access);

	//�R���X�g���N�^
	MappedBufferObject(
		GLsizeiptr size, GLenum type, GLenum access);

	//�f�X�g���N�^
	~MappedBufferObject();

	operator GLuint() const { return id; }
	GLsizeiptr GetSize() const { return bufferSize; }

	void WaitSync();										//CPU�������ւ̃R�s�[�̊�����҂�
	uint8_t* GetMappedAddress() const;						//�擾�����A�h���X�Ƀf�[�^���R�s�[����
	void Bind(GLuint index, size_t offset, size_t size);	//GPU�����������蓖�Ăĕ`��ɗ��p����
	void SwapBuffers();										//�R�s�[�p�ƕ`��p�̃o�b�t�@��؂�ւ���

	GLenum GetType() const { return type; }

private:
	GLuint id = 0;
	GLsizeiptr bufferSize = 0;

	uint8_t* p = nullptr;			//�f�[�^�R�s�[��A�h���X
	size_t bufferIndex = 0;			//�R�s�[��o�b�t�@�̃C���f�b�N�X
	GLsync syncList[2] = { 0,0 };	//�����I�u�W�F�N�g
	GLenum type = GL_ARRAY_BUFFER;	//�o�b�t�@�̎��
};


#endif //BUFFEROBJECT_H_INCLUDED
