//--------------------------------------------------------------------------------
//	colliderコンポネント
//　colliderComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-05-18
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "component.h"
#include "KF_CollisionSystem.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  Colliderポネントクラス
//--------------------------------------------------------------------------------
class CColliderComponent : public CComponent
{
public:
	//--------------------------------------------------------------------------------
	//  構造体定義
	//--------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	CColliderComponent(CGameObject* const pGameObj, const CS::COL_TYPE& type, const CS::COL_MODE& mode);
	~CColliderComponent() {}

	virtual bool		Init(void) override { return true; }
	virtual void		Uninit(void) override;
	virtual void		Update(void);
	
	//Set関数
	void				SetTrigger(const bool& bTrigger) { m_bTrigger = bTrigger; }
	void				SetTag(const string& strTag) { m_strTag = strTag; }
	void				SetOffset(const CKFVec3& vPos, const CKFVec3& vRot = CKFVec3(0.0f));

	//Get関数
	CKFVec3				GetLocalPos(void) const { return CKFVec3(m_mtxOffset.m_af[3][0], m_mtxOffset.m_af[3][1], m_mtxOffset.m_af[3][2]); }
	CKFVec3				GetWorldPos(void) const { return CKFVec3(m_mtxWorld.m_af[3][0], m_mtxWorld.m_af[3][1], m_mtxWorld.m_af[3][2]); }
	CKFMtx44			GetMatrixWorld(void) const { return m_mtxWorld; }
	const CS::COL_TYPE	GetType(void) const { return m_type; }
	const bool			IsTrigger(void) const { return m_bTrigger; }
	const string&		GetTag(void) const { return m_strTag; }

	//他
	void				Sleep(void);
	void				Awake(void);

protected:
	//--------------------------------------------------------------------------------
	//  定数定義
	//--------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	CColliderComponent() : CComponent() , m_type(CS::COL_MAX), m_mode(CS::MODE_MAX) {}

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	CS::COL_TYPE	m_type;		//colliderのタイプ
	CS::COL_MODE	m_mode;		//Static/Dynamic mode
	bool			m_bTrigger;	//Trigger Flag
	CKFMtx44		m_mtxOffset;//相対行列
	CKFMtx44		m_mtxWorld;	//ワールド行列(処理速度向上のため)
	string			m_strTag;	//識別用タグ
};