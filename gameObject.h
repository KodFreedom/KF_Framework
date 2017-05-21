//--------------------------------------------------------------------------------
//	ゲームオブジェクトスーパークラス
//　gameObject.h
//	Author : Xu Wenjie
//	Date   : 2017-04-26
//--------------------------------------------------------------------------------
#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "gameObjectManager.h"
#include "inputComponent.h"
#include "drawComponent.h"
#include "physicComponent.h"
#include "meshComponent.h"
#include "collisionComponent.h"

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  ゲームオブジェクトクラス
//--------------------------------------------------------------------------------
class CGameObject
{
	friend class CGameObjectManager;

public:
	CGameObject(const GOM::PRIORITY &pri = GOM::PRI_3D);
	~CGameObject() {}
	
	virtual KFRESULT	Init(void) 
	{ 
		m_pInput->Init();
		m_pCollision->Init();
		m_pPhysic->Init();
		m_pMesh->Init();
		m_pDraw->Init();
		return KF_SUCCEEDED;
	}

	virtual void		Uninit(void)
	{
		m_pInput->Release();
		m_pCollision->Release();
		m_pPhysic->Release();
		m_pMesh->Release();
		m_pDraw->Release();
	}

	virtual void		Update(void)
	{
		SwitchParam();
		m_pInput->Update(*this);
	}

	virtual void		LateUpdate(void)
	{
		m_pCollision->Update(*this);
		m_pPhysic->Update(*this);
		m_pMesh->Update(*this);
	}

	virtual void		Draw(void)
	{
		m_pDraw->Draw(*this, *m_pMesh);
	}

	void				Release(void);

	//Get関数
	CKFVec3				GetPos(void) const { return m_vPos; }
	CKFVec3				GetRot(void) const { return m_vRot; }
	CKFVec3				GetScale(void) const { return m_vScale; }
	CKFVec3				GetPosNext(void) const { return m_vPosNext; }
	CKFVec3				GetRotNext(void) const { return m_vRotNext; }
	CKFVec3				GetScaleNext(void) const { return m_vScaleNext; }
	CKFMtx44			GetMatrix(void) const { return m_mtxThis; }

	//Set関数
	void				SetPos(const CKFVec3 &vPos) { m_vPos = vPos; }
	void				SetRot(const CKFVec3 &vRot) { m_vRot = vRot; }
	void				SetScale(const CKFVec3 &vScale) { m_vScale = vScale; }
	void				SetPosNext(const CKFVec3 &vPosNext) { m_vPosNext = vPosNext; }
	void				SetRotNext(const CKFVec3 &vRotNext) { m_vRotNext = vRotNext; }
	void				SetScaleNext(const CKFVec3 &vScaleNext) { m_vScaleNext = vScaleNext; }
	void				SetMatrix(const CKFMtx44 &mtx) { m_mtxThis = mtx; }

protected:
	virtual void		SwitchParam(void);

	//コンポネント
	CInputComponent*		m_pInput;		//入力によるの処理パーツ
	CPhysicComponent*		m_pPhysic;		//物理処理パーツ
	CCollisionComponent*	m_pCollision;	//コリジョンパーツ
	CMeshComponent*			m_pMesh;		//メッシュパーツ
	CDrawComponent*			m_pDraw;		//描画処理パーツ

	//パラメーター
	CKFVec3		m_vPos;			//オブジェクト位置
	CKFVec3		m_vRot;			//オブジェクト回転
	CKFVec3		m_vScale;		//オブジェクトサイズ
	CKFVec3		m_vPosNext;		//次のオブジェクト位置
	CKFVec3		m_vRotNext;		//次のオブジェクト回転
	CKFVec3		m_vScaleNext;	//次のオブジェクトサイズ
	CKFMtx44	m_mtxThis;		//自分の回転平行移動行列

private:
	//パラメーター(管理用)
	bool				m_bActive;		//活動フラグ
	bool				m_bAlive;		//生きるフラグ
	GOM::PRIORITY		m_pri;			//優先度

	//ヌルコンポネント
	static CNullInputComponent		s_nullInput;
	static CNullPhysicComponent		s_nullPhysic;
	static CNullCollisionComponent	s_nullCollision;
	static CNullMeshComponent		s_nullMesh;
	static CNullDrawComponent		s_nullDraw;
};

//--------------------------------------------------------------------------------
//  ヌルゲームオブジェクトクラス
//--------------------------------------------------------------------------------
class CNullGameObject : public CGameObject
{
public:
	CNullGameObject() : CGameObject() {}
	~CNullGameObject() {}

	void	Uninit(void) override {}
	void	Update(void) override {}
	void	LateUpdate(void) override {}
	void	Draw(void) override {}
};

#endif