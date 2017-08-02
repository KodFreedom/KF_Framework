//--------------------------------------------------------------------------------
//	fieldCollider�R���|�l���g
//�@fieldColliderComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-06-05
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "colliderComponent.h"

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  fieldCollider�|�l���g�N���X
//--------------------------------------------------------------------------------
class CFieldColliderComponent : public CColliderComponent
{
public:
	//--------------------------------------------------------------------------------
	//  �\���̒�`
	//--------------------------------------------------------------------------------
	struct INFO
	{
		bool	bInTheField;
		float	fHeight;
		CKFVec3	vFaceNormal;
	};

	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	CFieldColliderComponent(CGameObject* const pGameObj, const int& nNumBlockX, const int& nNumBlockZ, const CKFVec2& vBlockSize)
		: CColliderComponent(pGameObj, CS::COL_FIELD, CS::STATIC)
		, m_nNumBlockX(nNumBlockX)
		, m_nNumBlockZ(nNumBlockZ)
		, m_vBlockSize(vBlockSize)
	{
		m_vectorVtx.clear();
	}

	~CFieldColliderComponent() {}

	bool	Init(void) override;
	void	Uninit(void) override;

	//Get�֐�
	INFO		GetPointInfo(const CKFVec3& vPos);
	list<INFO>	GetRangeInfo(const CKFVec3& vBegin, const CKFVec3& vEnd);

private:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	void		MakeVertex(void);	//�����̓G�f�B�^�ō��

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	int				m_nNumBlockX;
	int				m_nNumBlockZ;
	CKFVec2			m_vBlockSize;
	vector<CKFVec3>	m_vectorVtx;
};