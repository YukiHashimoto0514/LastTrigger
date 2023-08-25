//@Engine.cpp
#define _CRT_SECURE_NO_WARNINGS
#include "Engine.h"
#include "../Application/MainGameScene.h"
#include "../Application/TitleScene.h"
#include "../Application/EasyAudio.h"
#include "../Application/ProgramPipeline.h"
#include "Texture.h"
#include "Mesh.h"
#include "../Application/Debug.h"
#include "VertexArray.h"
#include "../Component/Camera.h"
#include "../Component/MeshRenderer.h"
#include <Windows.h>
#include <fstream>
#include <filesystem>

using namespace VecMath;

//OpenGL��̃��b�Z�[�W����������R�[���o�b�N�֐�
// 
// @param source			���b�Z�[�W�̔z�M��(OpenGL,Windows,�V�F�[�_�[�Ȃ�)
// @param type				���b�Z�[�W�̎��(�G���[�x���Ȃ�)
// @param id				���b�Z�[�W����ʂɎ��ʂ���l
// @param severity			���b�Z�[�W�̏d�v�x(���A���A��A�Œ�)
// @param length			���b�Z�[�W�̕�����. �����Ȃ烁�b�Z�[�W�͂O�I�[����Ă���
// @param message			���b�Z�[�W�{��
// @param userParam			�R�[���o�b�N�ݒ莞�Ɏw�肵���|�C���^

void GLAPIENTRY DebugCallback(GLenum, GLenum type, GLenum id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	std::string s;
	if (length < 0)
	{
		s = message;
	}
	else
	{
		s.assign(message, message + length);
	}
	s += '\n';
	OutputDebugString(s.c_str());//(�o�͂��镶����)
}

//SSBO�̃T�C�Y���v�Z����
size_t CalcSsboSize(size_t n)//���ȏ�̂Q�T�U�̔{���̒l
{
	const size_t alignment = 256;//�A���C�����g�i�f�[�^�̋��E�j
	return ((n + alignment - 1) / alignment) * alignment;
};



//�G���W�������s����
// 	   @retval 0 ����Ɏ��s����������(retval=�߂�l)
// 	   @retval 0�ȊO�@�G���[����������

int Engine::Run()
{
	int ret = Initialize();//������
	if (ret == 0)
	{
		ret = MainLoop();//���C�����[�v
	}
	Finalize();//�I��
	return ret;
}

//�z�u�f�[�^��ǂݍ���

TileMap Engine::LoadTileMap(const char* filename)
{
	//�t�@�C�����J��
	std::ifstream file;
	file.open(filename);
	if (!file)
	{
		return { };	//���s�i�t�@�C�������Ⴄ�H�j
	}

	//�ǂݍ��ݐ�ƂȂ�ϐ�������
	TileMap tileMap;
	tileMap.data.reserve(bgSizeX * bgSizeY);	//�K���ȗe�ʂ�\��

	//�t�@�C����ǂݍ���
	while (!file.eof())
	{
		//��s�ǂݍ���
		std::string line;
		std::getline(file, line);	//�t�@�C������ǂݍ���

		if (line.size() <= 0)
		{
			continue;	//�f�[�^�̖����s�͖���
		}
		line.push_back(',');	//�I�[��ǉ�

		//�J���}��؂蕶����𐔒l�ɕϊ�
		tileMap.sizeX = 0;
		const char* start = line.data();
		const char* end = start + line.size();

		for (const char* p = start; p != end; ++p)
		{
			if (*p == ',')
			{
				const int n = atoi(start);	//������𐔎���
				tileMap.data.push_back(n);
				start = p + 1;				//���̃J���}��؂��ڎw���悤�ɍX�V
				++tileMap.sizeX;			//���̃^�C�������X�V
			}
		}
		++tileMap.sizeY;//�c�̃^�C�����X�V
	}

	//�ǂݍ��񂾔z�u�f�[�^��Ԃ�
	return tileMap;
}

//�z�u�f�[�^�ɏ]���Ĕw�i�X�v���C�g��ݒ肷��
void Engine::SetSpriteList(const TileMap& tileMap)
{
	//�w�i�X�v���C�g���폜
	spriteListBg.clear();

	//�z�u�J�n���W��������
	bgBaseX = 16;
	bgBaseY = 16;

	//�z�u�f�[�^���Q�Ƃ��Ĕw�i�X�v���C�g��ݒ�
	//�摜�T�C�Y512*512,�^�C���T�C�Y32*32
	for (size_t y = tileMap.sizeY; y > 0;)
	{
		--y;	//�}�C�i�X�ɂȂ�Ȃ��悤�ɁA�����Ō��Z
		for (size_t x = 0; x < tileMap.sizeX; ++x)
		{
			//�^�C���ԍ����擾
			const int tileNo = tileMap.data[y * tileMap.sizeX + x];

			//�^�C���ԍ��ɑΉ�����e�N�X�`�����W�����߂�
			const float u = static_cast<float>(tileNo % 16) * 32;
			const float v = static_cast<float>(tileNo / 16) * 32;

			//�w�i�X�v���C�g��ݒ�
			spriteListBg.push_back({
				static_cast<float>(x * 32 + bgBaseX),
				static_cast<float>(y * 32 + bgBaseY),
				0,1,MakeTexcoord(u,v,32,32),1,1,1,1 });
		}
	}
}

//3D���f���z�u�t�@�C����ǂݍ���

//@param filename   3D���f���z�u�t�@�C��
//@param translate	���ׂĂ�3D���f�����W�ɉ��Z���镽�s�ړ���
//@param scale		���ׂĂ�3D���f���Ɋ|����g�嗦
//@return �t�@�C������ǂݍ��񂾃Q�[���I�u�W�F�N�g�̔z��

GameObjectList Engine::LoadGameObjectMap(const char* filename,
	const vec3& translate, const vec3& scale)
{
	//�t�@�C�����J��
	std::ifstream ifs(filename);

	if (!ifs)
	{
		LOG_WARNING("%s���J���܂���", filename);
		return{};
	}

	GameObjectList gameObjectList;
	gameObjectList.reserve(300);//�K���Ȑ���\��

	//�s�P�ʂœǂݍ���
	while (!ifs.eof())
	{
		std::string line;
		std::getline(ifs, line);

		const char* p = line.c_str();

		int readByte = 0;

		//�f�[�^�s�̊J�n����
		char name[256];

					  //���������e����
		if (sscanf(p, R"( { "name" : "%255[^"]" %n)", name, &readByte) != 1)
		{
			continue;	//�f�[�^�s�ł͂Ȃ�
		}
		name[255] = '\0';
		p += readByte;	//�ǂݎ��ʒu���X�V

		//���O�ȊO�̗v�f��ǂݍ���
		char baseMeshName[256];

		if (sscanf(p, R"(, "mesh" : "%255[^"]" %n)", baseMeshName, &readByte) != 1)
		{
			continue;
		}
		baseMeshName[255] = '\0';
		p += readByte;	//�ǂݎ��ʒu���X�V

		vec3 t(0);
		if (sscanf(p, R"(, "translate" : [ %f, %f, %f ] %n)",
			&t.x, &t.y, &t.z, &readByte) != 3)
		{
			continue;
		}
		p += readByte;	//�ǂݎ��ʒu���X�V

		vec3 r(0);
		if (sscanf(p, R"(, "rotate" : [ %f, %f, %f ] %n)",
			&r.x, &r.y, &r.z, &readByte) != 3)
		{
			continue;
		}

		p += readByte;	//�ǂݎ��ʒu���X�V

		vec3 s(1);
		if (sscanf(p, R"(, "scale" : [ %f, %f, %f ] %n)",
			&s.x, &s.y, &s.z, &readByte) != 3)
		{
			continue;
		}

		p += readByte;	//�ǂݎ��ʒu���X�V

		//���b�V����ǂݍ���
		const std::string meshName = std::string("Res/Model/") + baseMeshName + ".obj";
		auto mesh = LoadOBJ(meshName.c_str());

		if (!mesh)
		{
			LOG_WARNING("���b�V���t�@�C��%s�̓ǂݍ��݂Ɏ��s", meshName.c_str());
			continue;
		}

		//���W�̊g��k���ƕ��s�ړ�
		t = t * scale + translate;
		s *= scale;

		//�Q�[���I�u�W�F�N�g���쐬
		auto gameObject = Create<GameObject>(meshName);

		gameObject->x = t.x;
		gameObject->y = t.y;
		gameObject->z = t.z;

		//Unity�͍�����W�n�Ȃ̂ŁA��]�������E����W�n�ɕϊ�����
		gameObject->rotation[0] = radians(-r.x);
		gameObject->rotation[1] = radians(-r.y);
		gameObject->rotation[2] = radians(r.z);

		auto renderer = gameObject->AddComponent<MeshRenderer>();
		renderer->mesh = mesh;
		renderer->scale = s;

		//Unity����G�N�X�|�[�Ƃ���OBJ��X�������]���Ă���̂ŁAY����180�x��]������
		renderer->rotation.y = radians(180);

		//�Q�[���I�u�W�F�N�g��ǉ�
		gameObjectList.push_back(gameObject);
	}


	//�쐬�����Q�[���I�u�W�F�N�g�z���Ԃ�
	LOG("3D���f���z�u�t�@�C��%s��ǂݍ��݂܂���", filename);

	return gameObjectList;
}

//UI���C���[��ǉ�����

//@param image      ���C���[�Ŏg�p����e�N�X�`���摜
//@param filterMode �e�N�X�`���ɐݒ肷��g��k���t�B���^
//@param reserve    ���C���[�p�ɗ\�񂷂�Q�[���I�u�W�F�N�g��
//@return �ǉ��������C���[�ԍ�

size_t Engine::AddUILayer(const char* image, GLenum filterMode, size_t reserve)
{
	TexturePtr tex = Texture::Create(image, filterMode);
	uiLayerList.push_back({ tex });
	uiLayerList.back().gameObjectList.reserve(reserve);
	uiLayerList.back().spriteList.reserve(reserve);

	return uiLayerList.size() - 1;
}

//���ׂẴ��C���[���폜����
void Engine::ClearUILayers()
{
	uiLayerList.clear();
}

//OBJ�t�@�C����ǂݍ���
//@param filename �ǂݍ���OBJ�t�@�C����
//@return�쐬�������b�V���̃A�h���X

Mesh::StaticMeshPtr Engine::LoadOBJ(const char* filename)
{
	return meshBuffer->LoadOBJ(filename);
}

//�Q�[���G���W��������������
//		@retval 0 ����ɏ��������ꂽ
//		@retval 0�ȊO�@�G���[����������

int Engine::Initialize()
{
	//GLFW�̏�����
	if (glfwInit() != GLFW_TRUE)
	{
		return 1;
	}


	//�`��E�B���h�E�̍쐬
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);//	�q���g�̎�ށA�q���g�ɐݒ肷��l
	window = glfwCreateWindow(1280, 720, title.c_str(), nullptr, nullptr);

	if (!window)
	{
		glfwTerminate();
		return 1;
	}

	//OpenGL�R���e�L�X�g�̍쐬
	glfwMakeContextCurrent(window);

	//OpenGL�֐��̃A�h���X���擾����
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		glfwTerminate();
		return 1;
	}

	//���b�Z�[�W�R�[���o�b�N�̐ݒ�
	glDebugMessageCallback(DebugCallback, nullptr);//(�Ăяo���֐��̃A�h���X�AuserParam�����ɓn�����l)

	//�V�F�[�_�[�t�@�C��(2D)�̓ǂݍ���
	progSprite = ProgramPipeline::Create(
		"Res/Shader/standard_2D.vert", "Res/Shader/standard_2D.frag");

	//�V�F�[�_�[�t�@�C��(3D)�̓ǂݍ���
	progStandard3D = ProgramPipeline::Create(
		"Res/Shader/standard_3D.vert", "Res/Shader/standard_3D.frag");

	//�e�p�I�u�W�F�N�g���쐬
	fboShadow = FramebufferObject::Create(1024, 1024, FboType::depth);
	progShadow = ProgramPipeline::Create("Res/Shader/shadow.vert", "Res/Shader/shadow.frag");

	//�Q�[���E�B���h�E�pFBO���쐬
	const vec2 viewSize = GetViewSize();
	fboGameWindow = FramebufferObject::Create(
		static_cast<int>(viewSize.x), static_cast<int>(viewSize.y), FboType::colorDepth);
	
	//�ڂ����V�F�[�_��ǂݍ���
	progGaussianBlur = ProgramPipeline::Create(
		"Res/Shader/standard_2D.vert", "Res/Shader/gaussian_blur.frag");

	//�u���[���p�V�F�[�_�\��ǂݍ���
	progHighPassFilter = ProgramPipeline::Create(
		"Res/Shader/simple_2D.vert", "Res/Shader/high_pass_filter.frag");
	progDownSampling = ProgramPipeline::Create(
		"Res/Shader/simple_2D.vert", "Res/Shader/down_sampling.frag");
	progUpSampling = ProgramPipeline::Create(
		"Res/Shader/simple_2D.vert", "Res/Shader/up_sampling.frag");

	//���W�ϊ��s������j�t�H�[���ϐ��ɃR�s�[
	//�g�p���钸�_���W��-0.5~0.5�̃T�C�Y�Ȃ̂�2�{����
	//�X�v���C�g��FBO�̃e�N�X�`�����W�͋t�Ȃ̂�Y���}�C�i�X�ɂ���
	const mat4 matBloomM = {//���f���s��
		vec4(2,  0, 0, 0),
		vec4(0, -2, 0, 0),
		vec4(0,  0, 2, 0),
		vec4(0,  0, 0, 1),
	};

	const mat4 matBloomVP = mat4(1);
	const GLuint bloomProgramIdList[] = {
		*progHighPassFilter,*progDownSampling,*progUpSampling };

	for (auto prog : bloomProgramIdList)
	{
		glProgramUniformMatrix4fv(prog, 0, 1, GL_FALSE, &matBloomM[0][0]);
		glProgramUniformMatrix4fv(prog, 1, 1, GL_FALSE, &matBloomVP[0][0]);
	}

	//���C����ʂ̃u���[���G�t�F�N�g�pFBO���쐬
	int bloomX = static_cast<int>(viewSize.x);
	int bloomY = static_cast<int>(viewSize.y);

	for (auto& e : fboBloom)
	{
		bloomX /= 2;
		bloomY /= 2;

		e = FramebufferObject::Create(bloomX, bloomY, FboType::color);
	}


	//���_�f�[�^��GPU�������ɃR�s�[
	struct Vertex {
		float x, y, z;	//���_���W
		float u, v;		//�e�N�X�`�����[
	};

	//�摜�̈ʒu���W�����߂�
	const Vertex vertexData[] = {
		{-0.5f ,-0.5f, 0 , 0 , 1},
		{ 0.5f , -0.5f, 0 , 1 , 1},
		{ 0.5f ,  0.5f, 0 , 1 , 0},
		{-0.5f ,  0.5f, 0 , 0, 0},
	};

	glCreateBuffers(1, &vbo);	//(�I�u�W�F�N�g���A�ǂ��Ɋi�[���邩)
	glNamedBufferStorage(vbo, sizeof(vertexData), vertexData, 0);		//(�Ǘ��ԍ��A�R�s�[����o�C�g���A�R�s�[����f�[�^�̃A�h���X�A�t���O)

	//�C���f�b�N�X�f�[�^��GPU�������ɃR�s�[
	const GLshort indexData[] = {
		0,1,2,2,3,0,
	};
	glCreateBuffers(1, &ibo);
	glNamedBufferStorage(ibo, sizeof(indexData), indexData, 0);

	//���_�f�[�^�̍\������ݒ�
	glCreateVertexArrays(1, &vao);		//���_������Ă���	(�쐬����I�u�W�F�N�g���A�ԍ����i�[����z��̃A�h���X)
	glBindVertexArray(vao);				//���삵�����I�u�W�F�N�g���w��	(OpenGL�R���e�L�X�g�̊��蓖�Ă�VAO�̊Ǘ��ԍ�)
	glBindBuffer(GL_ARRAY_BUFFER, vbo);	//(���蓖�Ă�I�u�W�F�N�g�̎�ށAOpenGL�R���e�L�X�g�Ɋ��蓖�Ă�o�b�t�@�I�u�W�F�N�g�̊Ǘ��ԍ�)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glEnableVertexAttribArray(0);		//���_������L���ɂ���	(�L���ɂ��钸�_�����z��̃C���f�b�N�X)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);//GPU�ɓ`�B����	(���_�����z��̃C���f�b�N�X�A�f�[�^�̗v�f���A�f�[�^�̌^�A���K���̗L���A���̃f�[�^�܂ł̃o�C�g���A�ŏ��̃f�[�^�ʒu)
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(const void*)offsetof(Vertex, u));

	//�e�N�X�`�����쐬����
	const char* filenameImgList[] = { "Res/Map/bg0_2.tga" ,"Res/Map/objects16.tga" };
	for (size_t i = 0; i < std::size(tex); ++i)
	{
		tex[i] = Texture::Create(filenameImgList[i], GL_NEAREST);
	}


	//�X�v���C�g�̔z����쐬
	spriteListBg.reserve(10'000);
	spriteListObj.reserve(10'000);

	//�X�v���C�g�p�̂r�r�a�n���쐬
	ssboSprite = MappedBufferObject::Create(
		CalcSsboSize(maxSpriteCount * sizeof(Sprite)),
		GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);

	//3D���f���p�̃o�b�t�@���쐬
	meshBuffer = Mesh::MeshBuffer::Create(
		sizeof(Mesh::Vertex) * 250'000, sizeof(uint16_t) * 650'000);

	primitiveBuffer = Mesh::PrimitiveBuffer::Create(
		sizeof(Mesh::Vertex) * 250'000, sizeof(uint16_t) * 650'000);

	//���C���J�������쐬
	cameraObject = Create<GameObject>("Main Camera");
	cameraObject->AddComponent<Camera>();


	//������������
	std::random_device rd;	//�^�̗����𐶐�����I�u�W�F�N�g�i�x���j
	rg.seed(rd());			//�^���������u�^�̗����v�ŏ�����

	//�Q�[���I�u�W�F�N�g�z��̗e�ʂ�\��
	gameObjectList.reserve(1000);	//1000���\�񂵂��B

	//�������C�u������������
	if (!Audio::Initialize())
	{
		return 1;	//���������s
	}

	//�ŏ��̃V�[�����쐬���ď�����
	scene = std::make_shared<TitleScene>();
	if (scene)
	{
		scene->Initialize(*this);
	}


	return 0;//����ɏ��������ꂽ
}

//���C�����[�v�̎��s

int Engine::MainLoop()
{
	//FPS�v���p�̕ϐ�
	double fpsTime = glfwGetTime();		//�o�ߎ��Ԃ��擾
	double fpsCount = 0;				//�v���p
	double prevTime = glfwGetTime();	//�O��̃��[�v�Ōv����������

	//���C�����[�v
	while (!glfwWindowShouldClose(window))
	{
		//FPS�v��
		const double curTime = glfwGetTime();	//�o�ߎ��Ԃ��擾
		const double differTime = curTime - fpsTime;
		++fpsCount;

		//�P�b���Ƃ�FPS��\��
		if (differTime >= 1)
		{
			const std::string fps = std::to_string(fpsCount / differTime);		//�o�Ă����l�𕶎���ɕϊ�
			const std::string newTitle = title + "-FPS:" + fps;						//�^�C�g���̏�������
			glfwSetWindowTitle(window, newTitle.c_str());							//�^�C�g���̍ĕ\��

			//�v���p�ϐ������Z�b�g
			fpsTime = curTime;
			fpsCount = 0;
		}

		//�o�ߎ���(�f���^�^�C��)���v��
		const float deltaTime = static_cast<float>(curTime - prevTime) * SlowSpeed;
		prevTime = curTime;

		//�V�[���̍X�V
		ChangeScene(deltaTime);

		//�Q�[���I�u�W�F�N�g���X�V
		UpdateGameObject(deltaTime);		//�A�b�v�f�[�g
		CollideGameObject();				//�Փ˔���
		RemoveGameObject(gameObjectList);	//�Q�[���I�u�W�F�N�g������

		for (auto& layer : uiLayerList)
		{
			RemoveGameObject(layer.gameObjectList);
		}

		MakeSpriteList(gameObjectList, spriteListObj);			//�X�v���C�g���X�g�����
		for (auto& layer : uiLayerList)
		{
			MakeSpriteList(layer.gameObjectList, layer.spriteList);
		}

		//�w�i���X�N���[��������
		BGScroll(deltaTime);

		//GPU���o�b�t�@���g���I���̂�҂�
		ssboSprite->WaitSync();

		//�X�v���C�g�f�[�^��SSBO�ɃR�s�[
		const std::vector<Sprite>* drawList[] = { &spriteListBg,&spriteListObj };
		uint8_t* p = ssboSprite->GetMappedAddress();

		for (size_t i = 0; i < std::size(drawList); ++i)
		{
			memcpy(p, drawList[i]->data(), drawList[i]->size() * sizeof(Sprite));
			p += CalcSsboSize(drawList[i]->size() * sizeof(Sprite));
		}

		//�Q�[���E�B���h�E�p�̃X�v���C�g�f�[�^��SSBO�ɃR�s�[
		SpriteList spriteListPostProcess;

		if (!gameObjectList.empty())
		{
			//�|�X�g�v���Z�X�p�X�v���C�g��ǉ�
			const vec2 gameWindowPos = GetWindowSize() * 0.5f;
			spriteListPostProcess.push_back(
				Sprite{ gameWindowPos.x,gameWindowPos.y,0,1,
				Texcoord{0,1,1,-1}, 1, 1,1,1 });

			const size_t size = spriteListPostProcess.size() * sizeof(Sprite);
			memcpy(p, spriteListPostProcess.data(), size);
			p += CalcSsboSize(size);
		}

		//UI���C���[�̃X�v���C�g�f�[�^��SSBO�ɃR�s�[
		for (const auto& layer : uiLayerList)
		{
			memcpy(p, layer.spriteList.data(), layer.spriteList.size() * sizeof(Sprite));
			p += CalcSsboSize(layer.spriteList.size() * sizeof(Sprite));
		}

		//�`�����Q�[���E�B���h�E�ɕύX
		glBindFramebuffer(GL_FRAMEBUFFER, *fboGameWindow);
		const vec2 viewSize = GetViewSize();

		glViewport(0, 0,
			static_cast<int>(viewSize.x), static_cast<int>(viewSize.y));

		glBindVertexArray(vao);

		//�o�b�N�o�b�t�@���N���A
		glClearColor(0.9f, 0.8f, 0.6f, 0.0f);//��,��,��,�����x(0~1�̒l����)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//�`��Ɏg���V�F�[�_���w��
		glUseProgram(*progSprite);

		//�[�x�e�X�g�𖳌���
		glDisable(GL_DEPTH_TEST);

		//���ʃJ�����O�𖳌���
		glDisable(GL_CULL_FACE);

		//�A���t�@�u�����h��L���ɂ���
		glEnable(GL_BLEND);		//�w�肵���@�\��L���ɂ���
		glBlendEquation(GL_FUNC_ADD);	//�����������̂���	
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	//�����������̂���

#if 0
		//���j�t�H�[���ϐ��Ƀf�[�^���R�s�[����
		const float timer = static_cast<float>(glfwGetTime());//�N��������̎��Ԍo�߂��擾
		glProgramUniform1f(progSprite, 0, timer);//�v���O�����I�u�W�F�N�g�̊Ǘ��ԍ��A�����̃��P�[�V�����ԍ��A����f�[�^
#endif
		//���j�t�H�[���ϐ��ɁA���W�ϊ��p�p�����[�^���R�s�[����
		glProgramUniform4f(*progSprite, 2,
			2.0f / viewSize.x, 2.0f / viewSize.y, -1, -1);

		//�X�v���C�g�`��p��VAO��ݒ�
		glBindVertexArray(vao);

		//2D�w�i��`��
		size_t spriteSsboOffset = 0;
		DrawSpriteList(*drawList[0], *tex[0], spriteSsboOffset);

		//3D���b�V����`��
		DrawStaticMesh(gameObjectList);
		
		//2D�I�u�W�F�N�g�`��
		glBindVertexArray(vao);		//�X�v���C�g�`��pVAO��ݒ�
		glUseProgram(*progSprite);	//�`��Ɏg���V�F�[�_�[���w��
		glDisable(GL_DEPTH_TEST);	//�[�x�e�X�g������
		glDisable(GL_CULL_FACE);	//���ʃJ�����O�𖳌���
		DrawSpriteList(*drawList[1], *tex[1], spriteSsboOffset);

		//�u���[����`��
		DrawBloomEffect();

		//�f�t�H���g�t���[���o�b�t�@�ɖ߂�
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, 1280, 720);
		glProgramUniform4f(*progSprite, 2, 2.0f / 1280, 2.0f / 720, -1, -1);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//�Q�[���E�B���h�E��`��
		glUseProgram(*progSprite);
		glProgramUniform4f(*progSprite, 2, 2.0f / 1280, 2.0f / 720, -1, -1);

		if (!gameObjectList.empty())
		{
			//�ڂ������x���O���傫����΂ڂ����V�F�[�_���g��
			if (blurStrength > 0)
			{
				glUseProgram(*progGaussianBlur);
				const vec2 windowSize = GetWindowSize();

				glProgramUniform4f(*progGaussianBlur, 2,
					2 / windowSize.x, 2 / windowSize.y, -1, -1);

				glProgramUniform1f(*progGaussianBlur, 100, blurStrength);

				DrawSpriteList(spriteListPostProcess,
					*fboGameWindow->GetColorTexture(), spriteSsboOffset);

				glUseProgram(*progSprite);
			}
			else
			{
				DrawSpriteList(spriteListPostProcess,
					*fboGameWindow->GetColorTexture(), spriteSsboOffset);
			}
		}

		//UI���C���[�̃X�v���C�g�z���`��
		for (const auto& layer : uiLayerList)
		{
			DrawSpriteList(layer.spriteList, *layer.tex, spriteSsboOffset);
		}

		//�X�v���C�g�pSSBO�̊��蓖�Ă���������
		glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 0, 0, 0, 0);//������SSBO�̊Ǘ��ԍ����O�ɂ���

		//�X�v���C�g�`��̒���Ƀt�F���X���쐬
		ssboSprite->SwapBuffers();



		//�������C�u�������X�V
		Audio::Update();

		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	return 0;
}

//�Q�[���G���W�����I������
//		@retval 0�@�@�@����ɏI������
//		@retval 0�ȊO�@�G���[����������

int Engine::Finalize()
{
	//�������C�u�������I��
	Audio::Finalize();

	//GLFW�̏I��
	glfwTerminate();

	return 0;
}

//�Q�[���I�u�W�F�N�g�̏�Ԃ��X�V
void Engine::UpdateGameObject(float deltaTime)
{
	//�������ɔz�񂪕ω����Ȃ��悤�ɃR�s�[���쐬
	std::vector<GameObject*> list(gameObjectList.size());
	std::transform(gameObjectList.begin(), gameObjectList.end(), list.begin(),//�ϊ��͈͂̐擪�A�I�[�A�R�s�[��̐擪
		[](const GameObjectPtr& e) {return e.get(); });//�ϊ��֐�


	//UI���C���[�̃Q�[���I�u�W�F�N�g�̃R�s�[���쐬
	for (auto& layer : uiLayerList)
	{
		const size_t prevListSize = list.size();
		list.resize(prevListSize + layer.gameObjectList.size());
		std::transform(layer.gameObjectList.begin(), layer.gameObjectList.end(),
			list.begin() + prevListSize,
			[](const GameObjectPtr& e) {return e.get(); });

		//�J�����I�u�W�F�N�g��ǉ�
		if (cameraObject)
		{
			list.push_back(cameraObject.get());
		}
	}

	//Start�C�x���g
	for (auto& e : list)
	{
		e->Start();
	}

	//Update�C�x���g
	for (auto& e : list)
	{
		if (!e->isDead)
		{
			e->Update(deltaTime);
		}
	}
}

//�Q�[���I�u�W�F�N�g�̏Փ˔���
void Engine::CollideGameObject()
{
	struct WorldCollider
	{
		GameObject* gameObject;
		Rect aabb;
		std::vector<Rect> collision;
	};

	//���[���h���W�̃R���C�_�[���v�Z
	std::vector<WorldCollider> tmp;
	tmp.reserve(gameObjectList.size());

	for (auto& e : gameObjectList)
	{
		//�X�^�[�g�����Ă��Ȃ��A�܂��͎���ł����͔��肵�Ȃ�
		if (!e->IsStarted() || e->isDead)
		{
			continue;
		}

		//���[���h���W�����[�J�����W�{�Q�[���I�u�W�F�N�g�̍��W
		const RectList& local = e->GetCollision();
		WorldCollider world;
		world.gameObject = e.get();
		world.aabb = Add(e->GetAabb(), e->x, e->y);
		world.collision.resize(local.size());

		for (size_t i = 0; i < local.size(); ++i)
		{
			world.collision[i] = Add(local[i], e->x, e->y);
		}
		tmp.push_back(world);
	}

	//�R���C�_�[�̏Փ˔���
	for (auto pa = tmp.begin(); pa != tmp.end(); ++pa)
	{
		for (auto pb = pa + 1; pb != tmp.end(); ++pb)
		{
			if (pb->gameObject->isDead)
			{
				continue;//�Փ˂��Ă��Ȃ�
			}
			if (!Overlap(pa->aabb, pb->aabb))
			{
				continue;//�Փ˂��Ă��Ȃ�
			}
			if (!Overlap(pa->collision, pb->collision))
			{
				continue;//�Փ˂��Ă��Ȃ�
			}

			//�Փ˂��Ă���̂ŁA�Փ˃C�x���g�֐������s
			pa->gameObject->OnCollision(*pb->gameObject);
			pb->gameObject->OnCollision(*pa->gameObject);
		}

		if (pa->gameObject->isDead)
		{
			break;//pa������ł���Ȃ烋�[�v��ł��؂�
		}
	}
}

//���S��Ԃ̃Q�[���I�u�W�F�N�g���폜
void Engine::RemoveGameObject(GameObjectList& gameObjectList)
{
	//�����Ă���I�u�W�F�N�g�Ǝ���ł���I�u�W�F�N�g�ɕ�����
	const auto i = std::stable_partition(gameObjectList.begin(), gameObjectList.end(),//�͈͂̐擪�A�I���A�ӂ�킯����
		[](const GameObjectPtr& e) { return !e->isDead; }//�֐�����т������߂ɁAremove�ł͂Ȃ��Apartition
	);

	//����ł���Q�[���I�u�W�F�N�g��ʂ̔z��Ɉړ�
	GameObjectList deadList(
		std::make_move_iterator(i),
		std::make_move_iterator(gameObjectList.end())
	);

	//�z�񂩂�ړ��ς݃I�u�W�F�N�g������
	gameObjectList.erase(i, gameObjectList.end());

	//����ł���I�u�W�F�N�g�ɑ΂���Destoroy�C�x���g�����s
	for (auto& e : deadList)
	{
		e->OnDestroy();
	}
}

//�Q�[���I�u�W�F�N�g����X�v���C�g�z����쐬
void Engine::MakeSpriteList(GameObjectList& gameObjectList, SpriteList& spriteList)
{
	//�����̓������̗p�̃X�v���C�g�z�����ɂ���
	spriteList.clear();

	//�D�揇�ʂ��傫�����Ԃɕ��ѕς���
	std::stable_sort(
		gameObjectList.begin(), gameObjectList.end(),//�擪�A�I�[
		[](const GameObjectPtr& a, const GameObjectPtr& b)//���וς�����
		{return a->priority > b->priority; }
	);
	

	//�Q�[���I�u�W�F�N�g�̃X�v���C�g�f�[�^���X�v���C�g�z��ɒǉ�
	//�����Ƀ��[���h���W�֕ϊ�����
	for (const auto& e : gameObjectList)
	{
		for (const auto& sprite : e->spriteList)
		{
			spriteList.push_back(sprite);
			Sprite& s = spriteList.back();
			s.x += e->x;
			s.y += e->y;
			s.z = e->radias;
			s.w = e->alpha;
			s.size = e->Scale;
			s.red = e->Red;
			s.green = e->Green;
			s.blue = e->Blue;
		}
	}
}

//�X�v���C�g�z���`�悷��

//@param spriteList			�`�悷��X�v���C�g�z��
//@param tex				�`��Ɏg�p����e�N�X�`��
//@param spriteSsboOffset	SSBO���̑f�����ƃf�[�^�J�n�ʒu

void Engine::DrawSpriteList(const SpriteList& spriteList, GLuint tex, size_t& spriteSsboOffset)
{

	const size_t size = CalcSsboSize(spriteList.size() * sizeof(Sprite));

	if (size == 0)
	{
		return;	//�`��f�[�^���Ȃ���΃X�L�b�v
	}


	//�e�N�X�`�����w�肷��
	glBindTextures(0, 1, &tex);//���蓖�ĊJ�n�C���f�b�N�X�A���蓖�Ă���A�e�N�X�`���Ǘ��ԍ��z��̃A�h���X

	//�X�v���C�g�pSSBO�����蓖�Ă�
	ssboSprite->Bind(0, spriteSsboOffset, size);
	spriteSsboOffset += size;

	//�}�`��`��
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0,
		static_cast<GLsizei>(spriteList.size()));

}

//3D���b�V����`��

void Engine::DrawStaticMesh(GameObjectList& gameObjectList)
{

	//���s����
	struct DirectionalLight
	{
		vec3 color;			//�F
		float intensity;	//���邳
		vec3 direction;		//����
	};

	DirectionalLight directionalLight = {
		vec3(1.0f,0.9f,0.8f),
		5, 
		vec3(0.57735f,-0.57735f,-0.57735f)
	};

	//�[�x�}�b�v�쐬�p�̒����X���W
	const vec2 viewSize = GetViewSize();
	const vec3 shadowTarget =
		vec3(viewSize.x * 0.5f, viewSize.y * 0.5f, 0);

	//�[�x�}�b�v�쐬�p�̎��_���W
	const vec3 shadowEye =
		shadowTarget - directionalLight.direction * 1000;

	//�[�x�}�b�v���쐬
	const mat4 matShadow = CreateDepthMap(
		gameObjectList, shadowEye, shadowTarget, 750, 100, 2000);

	//�Q�[���E�B���h�E�pFBO�ɕ`��
	glBindFramebuffer(GL_FRAMEBUFFER, *fboGameWindow);
	glUseProgram(*progStandard3D);
	glBindVertexArray(*meshBuffer->GetVAO());

	//�[�x�e�X�g��L����
	glEnable(GL_DEPTH_TEST);

	//���ʃJ�����O��L����
	glEnable(GL_CULL_FACE);

	//�A���t�@�u�����h��L����
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//�[�x�}�b�v���e�N�X�`���Ƃ��Ċ��蓖�Ă�
	const GLuint texDepth = *fboShadow->GetDepthTexture();
	glBindTextures(1, 1, &texDepth);

	//�e���W����e�N�X�`�����W�ɕϊ�����s��
	const mat4 matTexture = {
		vec4(0.5f, 0.0f, 0.0f, 0.0f),
		vec4(0.0f, 0.5f, 0.0f, 0.0f),
		vec4(0.0f, 0.0f, 0.5f, 0.0f),
		vec4(0.5f, 0.5f, 0.5f, 1.0f),
	};
	const mat4 matShadowTexture = matTexture * matShadow;
	glProgramUniformMatrix4fv(*progStandard3D, 1, 1, GL_FALSE, &matShadowTexture[0][0]);
	
	//���s�����̏���ݒ�
	const vec3 color = directionalLight.color * directionalLight.intensity;
	glProgramUniform3f(*progStandard3D, 100, color.x, color.y, color.z);

	glProgramUniform3f(*progStandard3D, 101,
		directionalLight.direction.x,
		directionalLight.direction.y,
		directionalLight.direction.z);

	//�����̏���ݒ�
	const vec3 ambientLight = vec3(0.2f, 0.2f, 0.3f);
	glProgramUniform3f(*progStandard3D, 104,
		ambientLight.x, ambientLight.y, ambientLight.z);

	//3D�\���p�̃J��������ݒ�
	Camera& camera = static_cast<Camera&>(*cameraObject->componentList[0]);
	glProgramUniform4f(*progStandard3D, 3,
		camera.fovScaleX, camera.fovScaleY, camera.A, camera.B);

	glProgramUniform3f(*progStandard3D, 4,
		cameraObject->x, cameraObject->y, cameraObject->z);

	glProgramUniform2f(*progStandard3D, 5,
		sin(cameraObject->rotation[0]), cos(cameraObject->rotation[0]));

	//�r���[�|�[�g��ݒ�
	glViewport(
		camera.viewport.x, camera.viewport.y,
		camera.viewport.width, camera.viewport.height);

	//���b�V�������_���������ׂẴQ�[���I�u�W�F�N�g��`��
	for (const auto& gameObject : gameObjectList)
	{
		gameObject->DrawStaticMesh(*progStandard3D);
	}

	glBindTextures(0, 2, nullptr);
	glBindVertexArray(0);
	glUseProgram(0);

}

//�[�x�}�b�v���쐬����

//@param gameObjectList �`�悷��Q�[���I�u�W�F�N�g�̔z��
//@param eye            �e�`��p�̎��_���W
//@param target         �e�`��p�̒����_�̍��W
//@param range          eye����e�`��͈͂̏�[�A���[�A���[�A�E�[�܂ł̋���
//@param zNear          eye����e�`��͈͂̎�O�[�܂ł̋���
//@param zFar           eye����e�`��͈͂����[�܂ł̋���

//@return �e�`��p�r���[�v���W�F�N�V�����s��

mat4 Engine::CreateDepthMap(GameObjectList& gameObjectList,
	const vec3& eye, const vec3& target,
	float range, float zNear, float zFar)
{
	//�`�����e�p�e�a�n�ɕύX
	glBindFramebuffer(GL_FRAMEBUFFER, *fboShadow);
	glClear(GL_DEPTH_BUFFER_BIT);

	//�r���[�|�[�g���e�a�n�T�C�Y�ɍ��킹��
	const GLuint texDepth = *fboShadow->GetDepthTexture();
	GLint w, h;
	glGetTextureLevelParameteriv(texDepth, 0, GL_TEXTURE_WIDTH, &w);//�e�pFBO�T�C�Y
	glGetTextureLevelParameteriv(texDepth, 0, GL_TEXTURE_WIDTH, &h);
	glViewport(0, 0, w, h);

	//�`��p�����[�^��ݒ�
	glBindVertexArray(*meshBuffer->GetVAO());	//3D���f���pVAO���o�C���h
	glUseProgram(*progShadow);	//�e�p�`��p�V�F�[�_���w��
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);		//�A���t�@�u�����h�𖳌���

	//�e�p�r���[�s����쐬
	const mat4 matShadowView = mat4::LookAt(eye, target, vec3(0, 1, 0));

	//�e�p�v���W�F�N�V�����s����쐬
	const mat4 matShadowProj = mat4::Orthogonal(
		-range, range,
		-range, range,
		zNear, zFar);

	//�e�p�r���[�v���W�F�N�V�����s����쐬
	const mat4 matShadow = matShadowProj * matShadowView;

	//matShadow�����j�t�H�[���ϐ��ɃR�s�[
	glProgramUniformMatrix4fv(*progShadow, 1, 1, GL_FALSE, &matShadow[0][0]);

	//���b�V�������_���������ׂẴQ�[���I�u�W�F�N�g��`��
	for (const auto& gameObject : gameObjectList)
	{
		gameObject->DrawStaticMesh(*progShadow);
	}

	return matShadow;
}

//�J���[�e�N�X�`����FBO�ɕ`�悷��
//@param fbo      �`���FBO
//@param texture  FBO�ɕ`�悷��e�N�X�`��
//@param wrapMode �e�N�X�`���ɐݒ肷�郉�b�v���[�h
void DrawTextureToFbo(FramebufferObject& fbo,
	Texture& texture, GLenum wrapMode)
{
	const Texture& texFbo = *fbo.GetColorTexture();
	glViewport(0, 0, texFbo.GetWidth(), texFbo.GetHeight());
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	texture.SetWrapMode(wrapMode);
	glBindTextureUnit(0, texture);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

}

void Engine::DrawBloomEffect()
{
	//�u���[���G�t�F�N�g�p��GL�R���e�L�X�g��ݒ�
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	//�Q�[����ʗpFBO���獂�P�x�����𒊏o
	glUseProgram(*progHighPassFilter);
	glDisable(GL_BLEND);//�������������Ȃ�����
	{
		//���P�x������FBO�̖����������Z�����̂ŁA���̉e����ł��������߂ɖ����Ŋ���
		const float strength = bloomStrength / static_cast<float>(std::size(fboBloom));
		glProgramUniform2f(*progHighPassFilter, 100, bloomThreshold, strength);

		DrawTextureToFbo(*fboBloom[0],
			*fboGameWindow->GetColorTexture(), GL_CLAMP_TO_BORDER);

	}

	//�k���ڂ������s���A�P�i������FBO�ɃR�s�[
	glUseProgram(*progDownSampling);

	for (size_t i = 1; i < std::size(fboBloom); i++)
	{
		DrawTextureToFbo(*fboBloom[i],
			*fboBloom[i - 1]->GetColorTexture(), GL_CLAMP_TO_BORDER);
	}
	
	//�g��ڂ������s���A�P�i�傫��FBO�ɉ��Z����
	glUseProgram(*progUpSampling);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);//���Z����(�V�F�[�_�o�́{FBO)

	for (size_t i = std::size(fboBloom) - 1; i > 0;)
	{
		i--;
		DrawTextureToFbo(*fboBloom[i],
			*fboBloom[i + 1]->GetColorTexture(), GL_CLAMP_TO_EDGE);
	}

	//�Ō�̊g��ڂ������s���A�Q�[����pFBO�ɉ��Z����
	{
		DrawTextureToFbo(*fboGameWindow,
			*fboBloom[0]->GetColorTexture(), GL_CLAMP_TO_EDGE);
	}
}

void Engine::ChangeScene(float deltaTime)
{
	//�V�[���̐؂�ւ�
	if (nextScene)
	{
		if (scene)
		{
			scene->Finalize(*this);
		}
		nextScene->Initialize(*this);
		scene = std::move(nextScene);
	}

	//�V�[�����X�V
	if (scene)
	{
		scene->Update(*this, deltaTime);
	}
}
void Engine::BGScroll(float deltaTime)
{
	const int viewSizeY = 22 * 32;	//�`��͈͂̍���
	const float scrollSpeed = 32;	//�X�N���[�����x�i�s�N�Z�����b�j
	if (spriteListBg.size() >= bgSizeX * bgSizeY)
	{
		const float bgBaseYEnd = -static_cast<float>(bgSizeY * 32) + viewSizeY + 16;	//�X�N���[���I�[���W
		bgBaseY -= scrollSpeed * deltaTime;

		if (bgBaseY < bgBaseYEnd)
		{
			bgBaseY = bgBaseYEnd;//�I�[���W�ŃX�N���[�����~�߂�
		}

		for (size_t y = 0; y < bgSizeY; ++y)
		{
			const float sy = y * 32 + bgBaseY;

			for (size_t x = 0; x < bgSizeX; ++x)
			{
				spriteListBg[y * bgSizeX + x].y = sy;
			}
		}
	}

}