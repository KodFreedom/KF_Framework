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
	Material()
		: Ambient(Color::White)
		, Diffuse(Color::White)
		, Specular(Color::Black)
		, Emissive(Color::Black)
		, Power(1.0f)
	{
		MainTexture.clear();
	}
	~Material() {}

	string	MainTexture;// �e�N�X�`��
	Color	Ambient;	// �����̔��˗�
	Color	Diffuse;	// ���ˌ��̔��˗�
	Color	Specular;	// ���ʌ��̔��˗�
	Color	Emissive;	// ������
	float	Power;		// �n�C���C�g�̃V���[�v�l�X

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
	void		Disuse(const string& materialName);
	Material*	GetMaterial(const string& materialName)
	{
		auto iterator = materials.find(materialName);
		if (materials.end() == iterator) return nullptr;
		return iterator->second.Pointer;
	}
	void		CreateMaterialFileBy(const string& materialName, const string& mainTextureName, const Color& ambient = Color::Gray, const Color& diffuse = Color::White, const Color& specular = Color::Black, const Color& emissive = Color::Black, const float& power = 1.0f);

private:
	//--------------------------------------------------------------------------------
	//  �\���̒�`
	//--------------------------------------------------------------------------------
	struct MaterialInfo
	{
		MaterialInfo() : UserNumber(1), Pointer(nullptr) {}
		int			UserNumber;
		Material*	Pointer;
	};

	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	MaterialManager();
	~MaterialManager() {}
	void		init(void) {}
	void		uninit(void) { materials.clear(); }
	Material*	loadFrom(const string& materialName);

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	unordered_map<string, MaterialInfo>		materials;
	static MaterialManager*					instance;
};