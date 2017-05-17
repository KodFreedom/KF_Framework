//--------------------------------------------------------------------------------
//	�}�e���A���}�l�[�W��
//�@materialManager.h
//	Author : Xu Wenjie
//	Date   : 2016-07-24
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _MATERIAL_MANAGER_H_
#define _MATERIAL_MANAGER_H_

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
	~CKFMaterial() {}

	CKFColor	m_cAmbient;		// �����̔��˗�
	CKFColor	m_cDiffuse;		// ���ˌ��̔��˗�
	CKFColor	m_cSpecular;	// ���ʌ��̔��˗�
	CKFColor	m_cEmissive;	// ������
	float		m_fPower;		// �n�C���C�g�̃V���[�v�l�X

								//�L���X�g
	operator D3DMATERIAL9 () const;
};

//--------------------------------------------------------------------------------
//	�}�e���A���}�l�[�W��
//--------------------------------------------------------------------------------
class CMaterialManager
{
public:
	enum MATERIAL
	{
		MAT_NORMAL,
		MAT_MAX
	};

	CMaterialManager() {}
	~CMaterialManager() {}

	void Init(void);

	CKFMaterial GetMaterial(const MATERIAL &mat);
private:
	CKFMaterial InitMaterial(const CKFColor &cAmbient, const CKFColor &cDiffuse, const CKFColor &cSpecular, const CKFColor &cEmissive, const float &fPower);

	CKFMaterial m_aMaterial[MAT_MAX];
};

#endif