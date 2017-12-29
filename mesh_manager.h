//--------------------------------------------------------------------------------
//�@mesh_manager.h
//	���b�V���Ǘ���
//	Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include "render_system.h"
#include "kf_utility.h"

//--------------------------------------------------------------------------------
//  �񋓌^��`
//--------------------------------------------------------------------------------
enum MeshType // ���b�V���̃^�C�v
{
	k3dMesh = 0,
	k3dSkin,
    k2dMesh
};

#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
enum DrawType // �`��^�C�v
{
	kPointList = 1,
	kLineList = 2,
	kLineStrip = 3,
	kTriangleList = 4,
	kTriangleStrip = 5,
	kTriangleFan = 6,
};
#else
enum DrawType
{
	kPointList = 0,
	kLineList = 1,
	kLineStrip = 3,
	kTriangleList = 4,
	kTriangleStrip = 5,
	kTriangleFan = 6,
};
#endif

//--------------------------------------------------------------------------------
//  �\���̒�`
//--------------------------------------------------------------------------------
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
struct Mesh
{
	Mesh()
		: type(k3dMesh)
		, draw_type(kTriangleStrip)
		, vertex_number(0)
		, index_number(0)
		, polygon_number(0)
		, vertex_buffer(nullptr)
		, index_buffer(nullptr) {}
	~Mesh()
	{
		SAFE_RELEASE(vertex_buffer);
		SAFE_RELEASE(index_buffer);
	}

	MeshType type;
	DrawType draw_type;
	int vertex_number;
	int index_number;
	int polygon_number;
	LPDIRECT3DVERTEXBUFFER9 vertex_buffer;
	LPDIRECT3DINDEXBUFFER9	index_buffer;
};
#else
struct Mesh
{
	Mesh()
		: type(k3d)
		, draw_type(kTriangleStrip)
		, vertex_number(0)
		, index_number(0)
		, polygon_number(0) {}
	MeshType type;
	DrawType draw_type;
	int vertex_number;
	int index_number;
	int polygon_number;
};
#endif

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
class MeshManager
{
public:
	//--------------------------------------------------------------------------------
	//  ��������
	//  return : MeshManager*
	//--------------------------------------------------------------------------------
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	static MeshManager* Create(const LPDIRECT3DDEVICE9 device)
	{
		auto instance = MY_NEW MeshManager(device);
		return instance;
	}
#endif

	//--------------------------------------------------------------------------------
	//  �j������
	//--------------------------------------------------------------------------------
	void Release(void) 
    {
        Uninit();
        MY_DELETE this;
    }
	
	//--------------------------------------------------------------------------------
	//  �^����ꂽ���O�̃��b�V�����g��
	//  ���łɂ���̏ꍇ���[�U�[��1�����A�Ȃ��̏ꍇ�t�@�C������ǂݍ���
	//  mesh_name : ���b�V����
	//--------------------------------------------------------------------------------
	void Use(const String& mesh_name);

	//--------------------------------------------------------------------------------
	//  �^����ꂽ���O�̃��b�V�����g��
	//  ���łɂ���̏ꍇ���[�U�[��1�����A�Ȃ��̏ꍇ�^����ꂽ���_�ō��
	//  mesh_name : ���b�V����
	//  type : �`��^�C�v
	//  vertexes : ���_�f�[�^
	//  indexes : �C���f�b�N�X�f�[�^
	//--------------------------------------------------------------------------------
	void Use(const String& mesh_name, const DrawType& type, const vector<Vertex3d>& vertexes, const vector<int>& indexes, const int& polygon_number = -1);

	//--------------------------------------------------------------------------------
	//  �^����ꂽ���O�̃��b�V�����g��Ȃ�
	//  ���[�U�[��-1�ɂȂ�A0�ɂȂ����ꍇ����������j������
	//  mesh_name : ���b�V����
	//--------------------------------------------------------------------------------
	void Disuse(const String& mesh_name);

	//--------------------------------------------------------------------------------
	//  �^����ꂽ���O�̃��b�V����^����ꂽ���_�ƃC���f�b�N�X�ōX�V����
	//  mesh_name : ���b�V����
	//  vertexes : ���_�f�[�^
	//  indexes : �C���f�b�N�X�f�[�^
	//--------------------------------------------------------------------------------
	void Update(const String& mesh_name, const vector<Vertex3d>& vertexes, const list<int>& indexes);
	
	//--------------------------------------------------------------------------------
	//  �^����ꂽ���O�̃��b�V�����t�@�C���ɏ����o��
	//  mesh_name : ���b�V����
	//  file_name : �t�@�C����
	//--------------------------------------------------------------------------------
	void SaveMeshToFile(const String& mesh_name, const String& file_name);

	//--------------------------------------------------------------------------------
	//  �^����ꂽ���O�̃��b�V���̃|�C���^���擾
	//  mesh_name : ���b�V����
	//  return : Mesh*
	//--------------------------------------------------------------------------------
	Mesh* GetMesh(const String& mesh_name) const
	{
		auto iterator = meshes_.find(mesh_name);
		if (meshes_.end() == iterator) return nullptr;
		return iterator->second.pointer;
	}

private:
	//--------------------------------------------------------------------------------
	//  �萔��`
	//--------------------------------------------------------------------------------
	static constexpr int kSphereCircleNumber = 10;
	static constexpr int kSphereVertexesPerCircle = 10;

	//--------------------------------------------------------------------------------
	//  �\���̒�`
	//--------------------------------------------------------------------------------
	struct MeshInfo
	{
		MeshInfo() : user_number(1) , pointer(nullptr) {}
		int user_number;
		Mesh* pointer;
	};

	//--------------------------------------------------------------------------------
	//  constructors and destructors
	//--------------------------------------------------------------------------------
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	MeshManager(const LPDIRECT3DDEVICE9 device) : device_(device) {}
	MeshManager() : device_(nullptr) {}
	MeshManager(const MeshManager& value) : device_(nullptr) {}
#else
	MeshManager() {}
	MeshManager(const MeshManager& value) {}
#endif
	MeshManager& operator=(const MeshManager& value) {}
	~MeshManager() {}

	//--------------------------------------------------------------------------------
	//  �I������
	//--------------------------------------------------------------------------------
	void Uninit(void);

	//--------------------------------------------------------------------------------
	//  mesh�t�@�C������f�[�^��ǂݍ���
	//  mesh_name : ���b�V����
	//  return : MeshInfo
	//--------------------------------------------------------------------------------
	MeshInfo LoadFromMesh(const String& mesh_name);

	//--------------------------------------------------------------------------------
	//  skin�t�@�C������f�[�^��ǂݍ���
	//  skin_name : �X�L�����b�V����
	//  return : MeshInfo
	//--------------------------------------------------------------------------------
	MeshInfo LoadFromSkin(const String& skin_name);

	//--------------------------------------------------------------------------------
	//  x�t�@�C������f�[�^��ǂݍ���
	//  mesh_name : ���b�V����
	//  return : MeshInfo
	//--------------------------------------------------------------------------------
	MeshInfo LoadFromXFile(const String& mesh_name);

	//--------------------------------------------------------------------------------
	//  cube���b�V���𐶐�����
	//  return : MeshInfo
	//--------------------------------------------------------------------------------
	MeshInfo CreateCube(void);

	//--------------------------------------------------------------------------------
	//  sphere���b�V���𐶐�����
	//  return : MeshInfo
	//--------------------------------------------------------------------------------
	MeshInfo CreateSphere(void);

	//--------------------------------------------------------------------------------
	//  skybox���b�V���𐶐�����
	//  return : MeshInfo
	//--------------------------------------------------------------------------------
	MeshInfo CreateSkyBox(void);

    //--------------------------------------------------------------------------------
    //  Polygon2d�𐶐�����
    //  return : MeshInfo
    //--------------------------------------------------------------------------------
    MeshInfo CreatePolygon2d(void);

	//--------------------------------------------------------------------------------
	//  �^����ꂽ���_�ƃC���f�b�N�X�Ń��b�V���𐶐�����
	//  type : �`��^�C�v
	//  vertexes : ���_�f�[�^
	//  indexes : �C���f�b�N�X�f�[�^
	//  return : MeshInfo
	//--------------------------------------------------------------------------------
	MeshInfo CreateMesh(const DrawType& type, const vector<Vertex3d>& vertexes, const vector<int>& indexes, const int& polygon_number);
	
    //--------------------------------------------------------------------------------
    //  create the 2d vertex and index buffer, return true if successed
    //  �o�[�e�b�N�X�ƃC���f�b�N�X�o�b�t�@�̐���
    //  return : bool
    //--------------------------------------------------------------------------------
    bool CreateBuffer2d(Mesh* const mesh) const;

	//--------------------------------------------------------------------------------
	//  create the 3d vertex and index buffer, return true if successed
	//  �o�[�e�b�N�X�ƃC���f�b�N�X�o�b�t�@�̐���
	//  return : bool
	//--------------------------------------------------------------------------------
	bool CreateBuffer3d(Mesh* const mesh) const;

	//--------------------------------------------------------------------------------
	//  create the 3dSkin vertex and index buffer, return true if successed
	//  �o�[�e�b�N�X�ƃC���f�b�N�X�o�b�t�@�̐���
	//  return : bool
	//--------------------------------------------------------------------------------
	bool CreateBuffer3dSkin(Mesh* const mesh) const;

	//--------------------------------------------------------------------------------
	//  DrawType�ɂ���ă|���S���̒��_��Ԃ�
	//  return : int
	//--------------------------------------------------------------------------------
	int GetVertexNumberPerPolygon(const DrawType& type);

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	unordered_map<String, MeshInfo> meshes_; // ���b�V����ۑ�����Ƃ���
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	const LPDIRECT3DDEVICE9 device_; // directx9�̃f�B�o�C�X
#endif
};