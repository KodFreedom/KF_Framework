//--------------------------------------------------------------------------------
//	プレイヤービヘイビアコンポネント
//　fieldEditorBehaviorComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-07-17
//--------------------------------------------------------------------------------
#pragma once
#ifdef _DEBUG
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "behaviorComponent.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class CFieldEditorBehaviorComponent : public CBehaviorComponent
{
public:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	CFieldEditorBehaviorComponent(CGameObject* const pGameObj);
	~CFieldEditorBehaviorComponent() {}

	bool	Init(void) override;
	void	Uninit(void) override;
	void	Update(void) override;
	void	LateUpdate(void) override {}

	void	OnTrigger(CColliderComponent& colliderThis, CColliderComponent& collider) override {}
	void	OnCollision(CCollisionInfo& collisionInfo) override {}

	//Get関数
	void	AdjustPosInField(CKFVec3& vPos, const bool& bAdjustHeight);
	bool	GetActive(void) const { return m_bActive; }

	//Set関数
	void	SetActive(const bool& bActive);
	void	SetPos(const CKFVec3& vPos) { m_vEditorPos = vPos; }

	//Save
	void	SaveAs(const string& strFileName);

private:
	//--------------------------------------------------------------------------------
	//  構造体定義
	//--------------------------------------------------------------------------------
	struct EINFO
	{
		list<int> listChoosenIdx;
	};

	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	float			getHeight(const CKFVec3& vPos);
	EINFO			getInfo(void);
	void			showMainWindow(void);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	int				m_nNumBlockX;
	int				m_nNumBlockZ;
	CKFVec2			m_vBlockSize;
	vector<CKFVec3>	m_vectorVtx;
	CKFVec3			m_vPosMin;
	CKFVec3			m_vPosMax;
	CKFVec3			m_vEditorPos;
	float			m_fEditorRadius;
	float			m_fRaiseSpeed;
	float			m_fExtendSpeed;
	bool			m_bActive;
};
#endif // _DEBUG