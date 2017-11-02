//--------------------------------------------------------------------------------
//	�}�e���A���}�l�[�W��
//�@materialManager.h
//	Author : Xu Wenjie
//	Date   : 2016-07-24
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �N���X��`
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	�}�e���A��
//--------------------------------------------------------------------------------
class Material
{
public:
	Material(const Color& ambient, const Color& diffuse, const Color& specular, const Color& emissive, const float& power)
		: ambient(ambient)
		, diffuse(diffuse)
		, specular(specular)
		, emissive(emissive)
		, power(power)
	{}
	~Material() {}

	Color	ambient;	// �����̔��˗�
	Color	diffuse;	// ���ˌ��̔��˗�
	Color	specular;	// ���ʌ��̔��˗�
	Color	emissive;	// ������
	float	power;		// �n�C���C�g�̃V���[�v�l�X

	//�L���X�g
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	operator D3DMATERIAL9 () const;
#endif

	//operator
	bool operator==(const Material& material) const;
};

//--------------------------------------------------------------------------------
//	�}�e���A���}�l�[�W��
//--------------------------------------------------------------------------------
class MaterialManager
{
public:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	static auto	Create(void)
	{
		if (instance) return instance;
		instance = new MaterialManager;
		instance->init();
		return instance;
	}
	static void Release(void) { SAFE_UNINIT(instance); }
	static auto Instance(void) { return instance; }

	const unsigned short	SaveMaterial(const Color &ambient, const Color &diffuse, const Color &specular, const Color &emissive, const float &power);
	const Material&			GetMaterial(const unsigned short& usID);

private:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	MaterialManager();
	~MaterialManager() {}
	void init(void) {}
	void uninit(void) { materials.clear(); }

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	unordered_map<unsigned short, Material> materials;
	static MaterialManager*					instance;
};