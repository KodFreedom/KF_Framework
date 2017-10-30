//--------------------------------------------------------------------------------
//	�}�e���A���}�l�[�W��
//�@materialManager.h
//	Author : Xu Wenjie
//	Date   : 2016-07-24
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------
#define CMM CMaterialManager	//CMaterialManager�̗���

//--------------------------------------------------------------------------------
//  �N���X��`
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	�}�e���A��
//--------------------------------------------------------------------------------
class CKFMaterial
{
public:
	CKFMaterial();
	CKFMaterial(const Color& cAmbient, const Color& cDiffuse, const Color& cSpecular, const Color& cEmissive, const float& fPower)
		: m_cAmbient(cAmbient)
		, m_cDiffuse(cDiffuse)
		, m_cSpecular(cSpecular)
		, m_cEmissive(cEmissive)
		, m_fPower(fPower)
	{}
	~CKFMaterial() {}

	Color	m_cAmbient;		// �����̔��˗�
	Color	m_cDiffuse;		// ���ˌ��̔��˗�
	Color	m_cSpecular;	// ���ʌ��̔��˗�
	Color	m_cEmissive;	// ������
	float		m_fPower;		// �n�C���C�g�̃V���[�v�l�X

	//�L���X�g
#ifdef USING_DIRECTX
	operator D3DMATERIAL9 () const;
#endif

	//operator
	bool operator==(const CKFMaterial& mat) const;
};

//--------------------------------------------------------------------------------
//	�}�e���A���}�l�[�W��
//--------------------------------------------------------------------------------
class CMaterialManager
{
public:
	//--------------------------------------------------------------------------------
	//  �񋓌^��`
	//--------------------------------------------------------------------------------
	enum MAT_TYPE
	{
		MT_NORMAL = 0,
	};

	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	CMaterialManager();
	~CMaterialManager() {}

	void					Release(void) 
	{
		m_umMaterial.clear();
		delete this; 
	}
	const unsigned short	SaveMaterial(const Color &cAmbient, const Color &cDiffuse, const Color &cSpecular, const Color &cEmissive, const float &fPower);
	const CKFMaterial&		GetMaterial(const unsigned short& usID);

private:
	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	unordered_map<unsigned short, CKFMaterial> m_umMaterial;
};