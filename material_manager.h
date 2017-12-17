//--------------------------------------------------------------------------------
//�@material_manager.h
//  manage the materials' save, load
//	�}�e���A���Ǘ���
//	Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include "main.h"

//--------------------------------------------------------------------------------
//  �}�e���A��
//--------------------------------------------------------------------------------
class Material
{
public:
	Material(const Color& ambient = Color::kGray
		, const Color& diffuse = Color::kWhite
		, const Color& specular = Color::kBlack
		, const Color& emissive = Color::kBlack
		, const float& power = 1.0f)
		: ambient_(ambient), diffuse_(diffuse)
		, specular_(specular), emissive_(emissive)
		, power_(power) {}
	Material(const String& color_texture
		, const String& diffuse_texture = String()
		, const String& diffuse_texture_mask = String()
		, const String& specular_texture = String()
		, const String& specular_texture_mask = String()
		, const String& normal_texture = String()
		, const String& detail_texture = String()
		, const String& detail_mask = String()
		, const String& tint_by_base_mask = String()
		, const String& rim_mask = String()
		, const String& translucency = String()
		, const String& metalness_mask = String()
		, const String& self_illum_mask = String()
		, const String& fresnel_warp_color = String()
		, const String& fresnel_warp_rim = String()
		, const String& fresnel_warp_specular = String()
		, const Color& ambient = Color::kGray
		, const Color& diffuse = Color::kWhite
		, const Color& specular = Color::kBlack
		, const Color& emissive = Color::kBlack
		, const float& power = 1.0f)
		: color_texture_(color_texture)
		, diffuse_texture_(diffuse_texture)
		, diffuse_texture_mask_(diffuse_texture_mask)
		, specular_texture_(specular_texture)
		, specular_texture_mask_(specular_texture_mask)
		, normal_texture_(normal_texture)
		, detail_texture_(detail_texture)
		, detail_mask_(detail_mask)
		, tint_by_base_mask_(tint_by_base_mask)
		, rim_mask_(rim_mask)
		, translucency_(translucency)
		, metalness_mask_(metalness_mask)
		, self_illum_mask_(self_illum_mask)
		, fresnel_warp_color_(fresnel_warp_color)
		, fresnel_warp_rim_(fresnel_warp_rim)
		, fresnel_warp_specular_(fresnel_warp_specular)
		, ambient_(ambient), diffuse_(diffuse)
		, specular_(specular), emissive_(emissive)
		, power_(power) {}
	~Material() {}

	String color_texture_;
	String diffuse_texture_;
	String diffuse_texture_mask_;
	String specular_texture_;
	String specular_texture_mask_;
	String normal_texture_;
	String detail_texture_;
	String detail_mask_;
	String tint_by_base_mask_;
	String rim_mask_;
	String translucency_;
	String metalness_mask_;
	String self_illum_mask_;
	String fresnel_warp_color_;
	String fresnel_warp_rim_;
	String fresnel_warp_specular_;
	Color ambient_; // �����̔��˗�
	Color diffuse_; // ���ˌ��̔��˗�
	Color specular_; // ���ʌ��̔��˗�
	Color emissive_; // ������
	float power_; // �n�C���C�g�̃V���[�v�l�X
};

//--------------------------------------------------------------------------------
//	�}�e���A���}�l�[�W��
//--------------------------------------------------------------------------------
class MaterialManager
{
public:
	//--------------------------------------------------------------------------------
	//  ��������
	//  return : MaterialManager*
	//--------------------------------------------------------------------------------
	static MaterialManager* Create(void)
	{
		auto instance = MY_NEW MaterialManager();
		instance->Init();
		return instance;
	}

	//--------------------------------------------------------------------------------
	//  �j������
	//--------------------------------------------------------------------------------
	void Release(void) { Uninit(); }

	//--------------------------------------------------------------------------------
	//  �^����ꂽ���O�̃}�e���A�����g��
	//  ���łɂ���̏ꍇ���[�U�[��1�����A�Ȃ��̏ꍇ�t�@�C������ǂݍ���
	//  material_name : �}�e���A����
	//--------------------------------------------------------------------------------
	void Use(const String& material_name);

	//--------------------------------------------------------------------------------
	//  �^����ꂽ���O�̃}�e���A�����g��
	//  ���łɂ���̏ꍇ���[�U�[��1�����A�Ȃ��̏ꍇ�^����ꂽ�l�Ő�������
	//  material_name : �}�e���A����
	//  material : �}�e���A�����
	//--------------------------------------------------------------------------------
	void Use(const String& material_name, Material* material);

	//--------------------------------------------------------------------------------
	//  �^����ꂽ���O�̃}�e���A�����g��Ȃ�
	//  ���[�U�[��-1�ɂȂ�A0�ɂȂ����ꍇ����������j������
	//  material_name : �}�e���A����
	//--------------------------------------------------------------------------------
	void Disuse(const String& material_name);

	//--------------------------------------------------------------------------------
	//  �^����ꂽ���O�̃}�e���A���̃|�C���^���擾
	//  material_name : �e�N�X�`����
	//  return : const Material*
	//--------------------------------------------------------------------------------
	const Material* GetMaterial(const String& material_name) const
	{
		auto iterator = materials_.find(material_name);
		if (materials_.end() == iterator)
		{// �Ȃ��̏ꍇ�f�t�H���g�̃}�e���A����Ԃ�
			return &kDefaultMaterial;
		}
		return iterator->second.pointer;
	}

private:
	//--------------------------------------------------------------------------------
	//  �\���̒�`
	//--------------------------------------------------------------------------------
	struct MaterialInfo
	{
		MaterialInfo() : user_number(1), pointer(nullptr) {}
		int	user_number;
		Material* pointer;
	};

	//--------------------------------------------------------------------------------
	//  �ÓI�����o�ϐ�
	//--------------------------------------------------------------------------------
	static const Material kDefaultMaterial;

	//--------------------------------------------------------------------------------
	//  constructors and destructors
	//--------------------------------------------------------------------------------
	MaterialManager() {}
	MaterialManager(const MaterialManager& value) {}
	MaterialManager& operator=(const MaterialManager& value) {}
	~MaterialManager() {}

	//--------------------------------------------------------------------------------
	//  ����������
	//--------------------------------------------------------------------------------
	void Init(void);

	//--------------------------------------------------------------------------------
	//  �I������
	//--------------------------------------------------------------------------------
	void Uninit(void);

	//--------------------------------------------------------------------------------
	//  �t�@�C������}�e���A���̓ǂݍ���
	//	material_name�F�}�e���A���̖��O
	//	return�FMaterial*
	//--------------------------------------------------------------------------------
	Material* LoadFromFile(const String& material_name);

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	unordered_map<String, MaterialInfo> materials_; // �}�e���A����ۑ�����Ƃ���
};