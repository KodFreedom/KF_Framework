//--------------------------------------------------------------------------------
//	トランスフォームコンポネント
//　transformComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-07-05
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "component.h"

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class CTransformComponent : public CComponent
{
public:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	CTransformComponent(CGameObject* const pGameObj);
	~CTransformComponent() {}
	
	bool					Init(void) override { return true; }
	void					Uninit(void) override {}
	void					UpdateMatrix(void);	
	void					UpdateMatrix(const CKFMtx44& mtxParent);
	void					SwapParam(void);
	
	//親子関係
	void					RegisterChild(CTransformComponent* pChild);
	void					DeregisterChild(CTransformComponent* pChild);
	void					RegisterParent(CTransformComponent* pParent, const CKFVec3& vOffsetPos = CKFVec3(0.0f), const CKFVec3& vOffsetRot = CKFVec3(0.0f));

	//Get関数
	const auto&				GetPos(void) const { return m_vPos; }
	const auto&				GetPosNext(void) const { return m_vPosNext; }
	const CKFQuaternion		GetRot(void) const;
	const CKFQuaternion		GetRotNext(void) const;
	const auto&				GetScale(void) const { return m_vScale; }
	const auto&				GetScaleNext(void) const { return m_vScaleNext; }
	const auto&				GetForward(void) const { return m_vForward; }
	const auto&				GetForwardNext(void) const { return m_vForwardNext; }
	const auto&				GetUp(void) const { return m_vUp; }
	const auto&				GetUpNext(void) const { return m_vUpNext; }
	const auto&				GetRight(void) const { return m_vRight; }
	const auto&				GetRightNext(void) const { return m_vRightNext; }
	const CKFMtx44			GetMatrixRot(void) const;
	const CKFMtx44			GetMatrixRotNext(void) const;
	const CKFMtx44&			GetMatrix(void) const { return m_mtxThis; }
	const CKFMtx44			GetMatrixWorldNext(void) const;
	const auto				GetParent(void) const { return m_pParent; }
	auto&					GetChildren(void) { return m_listChildren; }

	//Set関数
	void					SetPos(const CKFVec3& vPos) { m_vPos = vPos; }
	void					SetPosNext(const CKFVec3& vPosNext) { m_vPosNext = vPosNext; }
	void					SetRot(const CKFQuaternion& qRot);
	void					SetRotNext(const CKFQuaternion& qRotNext);
	void					MovePosNext(const CKFVec3& vMovement) { m_vPosNext += vMovement; }
	void					SetScale(const CKFVec3& vScale) { m_vScale = vScale; }
	void					SetScaleNext(const CKFVec3& vScaleNext) { m_vScaleNext = vScaleNext; }
	void					SetForward(const CKFVec3& vForward) { m_vForward = vForward; }
	void					SetForwardNext(const CKFVec3& vForward) { m_vForwardNext = vForward; }
	void					SetUp(const CKFVec3& vUp) { m_vUp = vUp; }
	void					SetUpNext(const CKFVec3& vUp) { m_vUpNext = vUp; }
	void					SetRight(const CKFVec3& vRight) { m_vRight = vRight; }
	void					SetRightNext(const CKFVec3& vRight) { m_vRightNext = vRight; }
	void					SetMatrix(const CKFMtx44& mtx) { m_mtxThis = mtx; }
	void					SetOffset(const CKFVec3& vPos, const CKFVec3& vRot)
	{
		m_vOffsetPos = vPos;
		m_vOffsetRot = vRot;
	}

	//回転関数
	void					RotByEuler(const CKFVec3& vRot);
	void					RotByPitch(const float& fRadian);
	void					RotByYaw(const float& fRadian);
	void					RotByRoll(const float& fRadian);
	void					RotByUp(const CKFVec3& vUp);
	void					RotByForward(const CKFVec3& vForward);
	void					RotByRight(const CKFVec3& vRight);

	//他の関数
	CKFVec3					TransformDirectionToWorld(const CKFVec3& vDirection);
	CKFVec3					TransformDirectionToLocal(const CKFVec3& vDirection);

private:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	void		calculateMtxThis(void);
	void		calculateMtxThis(const CKFMtx44& mtxParent);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	//パラメーター
	CKFVec3		m_vPos;			//オブジェクト位置
	CKFVec3		m_vPosNext;		//次のオブジェクト位置
	CKFVec3		m_vScale;		//オブジェクトサイズ
	CKFVec3		m_vScaleNext;	//次のオブジェクトサイズ
	CKFVec3		m_vForward;		//オブジェクトの前方向
	CKFVec3		m_vForwardNext;	//次のオブジェクトの前方向
	CKFVec3		m_vUp;			//オブジェクトの上方向
	CKFVec3		m_vUpNext;		//次のオブジェクトの上方向
	CKFVec3		m_vRight;		//オブジェクトの右方向
	CKFVec3		m_vRightNext;	//次のオブジェクトの右方向
	CKFMtx44	m_mtxThis;		//自分の回転平行移動行列
	
	//親子関係
	CTransformComponent*			m_pParent;
	list<CTransformComponent*>		m_listChildren;
	CKFVec3							m_vOffsetPos;
	CKFVec3							m_vOffsetRot;
};