//@file Mesh.h

#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED

#include "glad/glad.h"
#include "../Application/VecMath.h"
#include "../Engine/Texture.h"
#include <vector>
#include <memory>
#include <string>
#include <unordered_map>

//��s�錾
class VertexArray;
using VertexArrayPtr = std::shared_ptr<VertexArray>;

//3D���f���֌W�̋@�\���i�[���閼�O���

namespace Mesh
{
	//��s�錾
	class PrimitiveBuffer;
	using PrimitiveBufferPtr = std::shared_ptr<PrimitiveBuffer>;
	class MeshBuffer;
	using MeshBufferPtr = std::shared_ptr<MeshBuffer>;

	//���_�f�[�^�`��
	struct Vertex
	{
		VecMath::vec3 position; 	//���_���W
		VecMath::vec2 texcoord;		//�e�N�X�`�����W
		VecMath::vec3 normal;		//�@���}�b�v
	};

	//�}�e���A��
	struct Material
	{
		std::string name = "<Default>";				//�}�e���A����
		VecMath::vec4 baseColor = VecMath::vec4(1);	//��{�F�{�A���t�@
		float specularPower = 16;					//���ʔ��ˎw��
		float normalizeFactor = 24.0f / 25.13274f;	//���K���W��
		TexturePtr texBaseColor;					//��{�F�e�N�X�`��
	};
	using MaterialPtr = std::shared_ptr<Material>;

	//�v���~�e�B�u
	struct Primitive
	{
		GLenum mode = GL_TRIANGLES;	//�`�惂�[�h
		GLsizei count = 0;			//�`�悷��C���f�N�X��
		const void* indices = 0;	//�`��J�n�C���f�b�N�X�̃o�C�g�I�t�Z�b�g
		GLint baseVertex = 0;		//�C���f�b�N�X�O�ԂƂ݂Ȃ���钸�_�z��̈ʒu
		int materialNo = -1;		//�}�e���A��
	};

	void Draw(const Primitive& prim);


	//�v���~�e�B�u���Ǘ�����N���X
	class PrimitiveBuffer
	{
	public:
		//�v���~�e�B�u�o�b�t�@���쐬����
		static PrimitiveBufferPtr Create(size_t vboCapacity, size_t iboCapacity)
		{
			return std::make_shared<PrimitiveBuffer>(vboCapacity, iboCapacity);
		}

		//�R���X�g���N�^
		PrimitiveBuffer(size_t vboCapacity, size_t iboCapacity);

		//�f�X�g���N�^
		~PrimitiveBuffer();

		//�R�s�[�Ƒ���̋֎~
		PrimitiveBuffer(const PrimitiveBuffer&) = delete;
		PrimitiveBuffer& operator=(const PrimitiveBuffer&) = delete;

		//�v���~�e�B�u�̒ǉ�
		void AddPrimitive(const Vertex* vertices, size_t vertexCount,
			const uint16_t* indices, size_t indexCount, GLenum mode = GL_TRIANGLES);

		//�v���~�e�B�u�̎擾
		const Primitive& GetPrimitive(size_t index)const { return primitives[index]; }

		//VAO�̎擾
		VertexArrayPtr GetVAO() const { return vao; }

		//���_�f�[�^�A�C���f�b�N�X�f�[�^�̒ǉ�
		GLint AddVertices(const Vertex* vertices, size_t count);
		const void* AddIndices(const uint16_t* indices, size_t count);

		//���ׂẴv���~�e�B�u���폜
		void Clear();

	private:
		std::vector<Primitive> primitives;
		VertexArrayPtr vao;

		GLuint vbo = 0;				//vbo�Ǘ��ԍ�
		uint8_t* pVbo = nullptr;	//vbo�̏������ݐ�A�h���X
		size_t vboCapacity = 0;		//vbo�̑S�e�ʁi�o�C�g�j
		size_t vboSize = 0;			//VBO�̎g�p�ςݗe�ʁi�o�C�g�j

		GLuint ibo = 0;				//ibo�Ǘ��ԍ�
		uint8_t* pIbo = nullptr;	//ibo�̏������ݐ�A�h���X
		size_t iboCapacity = 0;		//ibo�̑S�e�ʁi�o�C�g�j
		size_t iboSize = 0;			//iBO�̎g�p�ςݗe�ʁi�o�C�g�j
	};


	//3D���f��
	struct StaticMesh
	{
		std::string name;	//���b�V����
		std::vector<Primitive> primitives;
		std::vector<MaterialPtr> materials;
	};

	using StaticMeshPtr = std::shared_ptr<StaticMesh>;
	void Draw(const StaticMesh& mesh,
		const std::vector<MaterialPtr>& materials, GLuint program = 0);

	//���b�V�����Ǘ�����N���X
	class MeshBuffer
	{
	public:
		//���b�V���o�b�t�@���쐬����
		static MeshBufferPtr Create(size_t vboCapacity, size_t iboCapacity)
		{
			return std::make_shared<MeshBuffer>(vboCapacity, iboCapacity);
		}

		//�R���X�g���N�^
		MeshBuffer(size_t vboCapacity, size_t iboCapacity);

		//�f�X�g���N�^
		~MeshBuffer() = default;

		//�t�@�C����ǂݍ����StaticMesh��Ԃ�
		StaticMeshPtr LoadOBJ(const char* filename);

		//
		VertexArrayPtr GetVAO() const;

		//���ׂĂ�StaticMesh���폜
		void Clear();

	private:
		PrimitiveBuffer primitiveBuffer;
		std::unordered_map<std::string, StaticMeshPtr> meshes;
	};
}



#endif //MESH_H_INCLUDED