//--------------------------------------------------------------------------------
//
//　gameObject3D.h
//	Author : Xu Wenjie
//	Date   : 2017-04-27
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _GAMEOBJECT_3D_H_
#define _GAMEOBJECT_3D_H_

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "gameObject.h"
#include "textureManager.h"
#include "materialManager.h"
#include "modelManager.h"

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class CGameObject3D : public CGameObject
{
public:
	CGameObject3D(const GOM::PRIORITY& pri = GOM::PRI_3D);
	~CGameObject3D() {}

	//Get関数
	CKFVec3 GetForward(void) const { return m_vForward; }
	CKFVec3 GetUp(void) const { return m_vUp; }
	CKFVec3 GetRight(void) const { return m_vRight; }
	CKFVec3 GetForwardNext(void) const { return m_vForwardNext; }
	CKFVec3 GetUpNext(void) const { return m_vUpNext; }
	CKFVec3 GetRightNext(void) const { return m_vRightNext; }
	//const CGameObject3D& GetParents(void) const { return *m_pParents; }

	//Set関数
	void	SetForward(const CKFVec3& vForward) { m_vForward = vForward; }
	void	SetUp(const CKFVec3& vUp) { m_vUp = vUp; }
	void	SetRight(const CKFVec3& vRight) { m_vRight = vRight; }
	void	SetForwardNext(const CKFVec3& vForward) { m_vForwardNext = vForward; }
	void	SetUpNext(const CKFVec3& vUp) { m_vUpNext = vUp; }
	void	SetRightNext(const CKFVec3& vRight) { m_vRightNext = vRight; }

	//生成関数
	static CGameObject3D* CreateSkyBox(const CKFVec3& vPos, const CKFVec3& vRot, const CKFVec3& vScale);
	static CGameObject3D* CreateField(const int& nNumBlockX, const int& nNumBlockZ, const CKFVec2& vBlockSize, const CKFVec3& vPos, const CKFVec3& vRot, const CKFVec3& vScale);
	static CGameObject3D* CreateCube(const CKFVec3& vSize, const CKFColor& cColor, const CKFVec3& vPos, const CKFVec3& vRot, const CKFVec3& vScale);
	static CGameObject3D* CreateModel(const CMOM::MODEL_NAME& modelName, const CKFVec3& vPos, const CKFVec3& vRot, const CKFVec3& vScale);

protected:
	virtual void SwapParam(void) override;

	CKFVec3	m_vForward;
	CKFVec3 m_vUp;
	CKFVec3 m_vRight;
	CKFVec3	m_vForwardNext;
	CKFVec3 m_vUpNext;
	CKFVec3 m_vRightNext;

	//CGameObject3D*	m_pParents;	//親パーツ
};

#endif