//--------------------------------------------------------------------------------
//	�}�e���A���}�l�[�W��
//�@materialManager.h
//	Author : Xu Wenjie
//	Date   : 2016-07-24
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"

//--------------------------------------------------------------------------------
//  �N���X��`
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

	void		Use(const string& materialName);
	void		Use(const string& materialName, const Color &ambient, const Color &diffuse, const Color &specular, const Color &emissive, const float &power);
	void		Disuse(const string& materialName);
	Material*	GetMaterial(const string& materialName)
	{
		auto iterator = materials.find(materialName);
		if (materials.end() == iterator) return nullptr;
		return iterator->second.Pointer;
	}

private:
	//--------------------------------------------------------------------------------
	//  �\���̒�`
	//--------------------------------------------------------------------------------
	struct MaterialInfo
	{
		MaterialInfo()
			: UserNumber(1)
			, Pointer(nullptr) {}
		int			UserNumber;
		Material*	Pointer;
	};

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
	unordered_map<string, MaterialInfo>		materials;
	static MaterialManager*					instance;
};