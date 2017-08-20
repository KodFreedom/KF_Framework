//--------------------------------------------------------------------------------
//	�v���C���[�r�w�C�r�A�R���|�l���g
//�@fieldEditorBehaviorComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-07-17
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "behaviorComponent.h"

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �v���C���[�s���R���|�l���g�N���X
//--------------------------------------------------------------------------------
class CFieldEditorBehaviorComponent : public CBehaviorComponent
{
public:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	CFieldEditorBehaviorComponent(CGameObject* const pGameObj);
	~CFieldEditorBehaviorComponent() {}

	bool	Init(void) override;
	void	Uninit(void) override;
	void	Update(void) override;
	void	LateUpdate(void) override {}

	void	OnTrigger(CColliderComponent& colliderThis, CColliderComponent& collider) override {}
	void	OnCollision(CCollisionInfo& collisionInfo) override {}

	//Get�֐�
	void	AdjustPosInField(CKFVec3& vPos);

	//Set�֐�
	void	SetActive(const bool& bActive) { m_bActive = bActive; }
	void	SetPos(const CKFVec3& vPos) { m_vEditorPos = vPos; }

private:
	struct EINFO
	{
		list<int> listChoosenIdx;
	};

	float			getHeight(const CKFVec3& vPos);
	EINFO			getInfo(void);

	int				m_nNumBlockX;
	int				m_nNumBlockZ;
	CKFVec2			m_vBlockSize;
	vector<CKFVec3>	m_vectorVtx;
	CKFVec3			m_vPosMin;
	CKFVec3			m_vPosMax;
	CKFVec3			m_vEditorPos;
	float			m_fEditorRadius;
	bool			m_bActive;
};