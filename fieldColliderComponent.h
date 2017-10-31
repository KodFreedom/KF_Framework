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
		Vector3	vFaceNormal;
	};

	struct FINFO
	{
		bool	bInFieldRange;
		float   fPenetration;
		Vector3 vFaceNormal;
	};

	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	CFieldColliderComponent(CGameObject* const pGameObj, const string& strFieldName)
		: CColliderComponent(pGameObj, CS::COL_FIELD, CS::STATIC)
		, m_nNumBlockX(0)
		, m_nNumBlockZ(0)
		, m_vBlockSize(Vector2(0.0f))
	{
		m_vectorVtx.clear();
		load(strFieldName);
	}

	~CFieldColliderComponent() {}

	bool	Init(void) override { return true; }
	void	Uninit(void) override;

	//Get�֐�
	//INFO	GetPointInfo(const Vector3& Position);
	FINFO	GetProjectionInfo(const Vector3& Position);
	bool	GetVtxByRange(const Vector3& vBegin, const Vector3& vEnd, int& nNumVtxXOut, int& nNumVtxZOut, vector<Vector3>& vecOut);

private:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	void	load(const string& strFieldName);
	bool	getPointInfo(const Vector3& Position, Vector3& vPLeftUp, Vector3& vPRightDown, Vector3& vPSide, Vector3& vFaceNormal);
	
	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	int				m_nNumBlockX;
	int				m_nNumBlockZ;
	Vector2			m_vBlockSize;
	vector<Vector3>	m_vectorVtx;
};