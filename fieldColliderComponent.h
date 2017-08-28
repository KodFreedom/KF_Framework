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

	struct FINFO
	{
		bool	bInFieldRange;
		float   fPenetration;
		CKFVec3 vFaceNormal;
	};

	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	CFieldColliderComponent(CGameObject* const pGameObj, const string& strFieldName)
		: CColliderComponent(pGameObj, CS::COL_FIELD, CS::STATIC)
		, m_nNumBlockX(0)
		, m_nNumBlockZ(0)
		, m_vBlockSize(CKFVec2(0.0f))
	{
		m_vectorVtx.clear();
		load(strFieldName);
	}

	~CFieldColliderComponent() {}

	bool	Init(void) override { return true; }
	void	Uninit(void) override;

	//Get�֐�
	//INFO	GetPointInfo(const CKFVec3& vPos);
	FINFO	GetProjectionInfo(const CKFVec3& vPos);
	bool	GetVtxByRange(const CKFVec3& vBegin, const CKFVec3& vEnd, int& nNumVtxXOut, int& nNumVtxZOut, vector<CKFVec3>& vecOut);

private:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	void	load(const string& strFieldName);
	bool	getPointInfo(const CKFVec3& vPos, CKFVec3& vPLeftUp, CKFVec3& vPRightDown, CKFVec3& vPSide, CKFVec3& vFaceNormal);
	
	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	int				m_nNumBlockX;
	int				m_nNumBlockZ;
	CKFVec2			m_vBlockSize;
	vector<CKFVec3>	m_vectorVtx;
};