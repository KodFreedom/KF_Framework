//--------------------------------------------------------------------------------
//	�v���C���[�r�w�C�r�A�R���|�l���g
//�@fieldEditorBehaviorComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-07-17
//--------------------------------------------------------------------------------
#pragma once
#ifdef _DEBUG
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
	void	AdjustPosInField(Vector3& Position, const bool& bAdjustHeight);
	bool	GetActive(void) const { return m_bActive; }

	//Set�֐�
	void	SetActive(const bool& bActive);
	void	SetPos(const Vector3& Position) { m_vEditorPos = Position; }

	//Save
	void	SaveAs(const string& strFileName);

private:
	//--------------------------------------------------------------------------------
	//  �\���̒�`
	//--------------------------------------------------------------------------------
	struct EINFO
	{
		list<int> listChoosenIdx;
	};

	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	float			getHeight(const Vector3& Position);
	EINFO			getInfo(void);
	void			showMainWindow(void);

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	int				m_nNumBlockX;
	int				m_nNumBlockZ;
	Vector2			m_vBlockSize;
	vector<Vector3>	m_vectorVtx;
	Vector3			m_PositionMin;
	Vector3			m_PositionMax;
	Vector3			m_vEditorPos;
	float			m_fEditorRadius;
	float			RaiseSpeed;
	float			m_fExtendSpeed;
	bool			m_bActive;
};
#endif // _DEBUG