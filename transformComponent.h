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
#ifdef _DEBUG
	friend class CDebugManager;
#endif // _DEBUG

public:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	CTransformComponent(CGameObject* const pGameObj);
	~CTransformComponent() {}
	
	bool					Init(void) override { return true; }
	void					Uninit(void) override {}
	void					UpdateMatrix(void);	
	void					UpdateMatrix(const Matrix44& mtxParent);
	void					SwapParam(void);
	
	//親子関係
	void					RegisterChild(CTransformComponent* pChild);
	void					DeregisterChild(CTransformComponent* pChild);
	void					RegisterParent(CTransformComponent* pParent, const Vector3& vOffsetPos = Vector3(0.0f), const Vector3& vOffsetRot = Vector3(0.0f));

	//Get関数
	const auto&				GetPos(void) const { return m_vPos; }
	const auto&				GetPosNext(void) const { return m_vPosNext; }
	const Quaternion		GetRot(void) const;
	const Quaternion		GetRotNext(void) const;
	const Vector3			GetEulerRot(void) const;
	const auto&				GetScale(void) const { return m_vScale; }
	const auto&				GetScaleNext(void) const { return m_vScaleNext; }
	const auto&				GetForward(void) const { return m_vForward; }
	const auto&				GetForwardNext(void) const { return m_vForwardNext; }
	const auto&				GetUp(void) const { return m_vUp; }
	const auto&				GetUpNext(void) const { return m_vUpNext; }
	const auto&				GetRight(void) const { return m_vRight; }
	const auto&				GetRightNext(void) const { return m_vRightNext; }
	const Matrix44			GetMatrixRot(void) const;
	const Matrix44			GetMatrixRotNext(void) const;
	const Matrix44&			GetMatrix(void) const { return m_mtxThis; }
	const Matrix44			GetMatrixWorldNext(void) const;
	const auto				GetParent(void) const { return m_pParent; }
	auto&					GetChildren(void) { return m_listChildren; }

	//Set関数
	void					SetPos(const Vector3& vPos) { m_vPos = vPos; }
	void					SetPosNext(const Vector3& vPosNext) { m_vPosNext = vPosNext; }
	void					SetRot(const Quaternion& qRot);
	void					SetRotNext(const Quaternion& qRotNext);
	void					SetRotNext(const Vector3& vRot);
	void					MovePosNext(const Vector3& vMovement) { m_vPosNext += vMovement; }
	void					SetScale(const Vector3& vScale) { m_vScale = vScale; }
	void					SetScaleNext(const Vector3& vScaleNext) { m_vScaleNext = vScaleNext; }
	void					SetForward(const Vector3& vForward) { m_vForward = vForward; }
	void					SetForwardNext(const Vector3& vForward) { m_vForwardNext = vForward; }
	void					SetUp(const Vector3& vUp) { m_vUp = vUp; }
	void					SetUpNext(const Vector3& vUp) { m_vUpNext = vUp; }
	void					SetRight(const Vector3& vRight) { m_vRight = vRight; }
	void					SetRightNext(const Vector3& vRight) { m_vRightNext = vRight; }
	void					SetMatrix(const Matrix44& mtx) { m_mtxThis = mtx; }
	void					SetOffset(const Vector3& vPos, const Vector3& vRot)
	{
		m_vOffsetPos = vPos;
		m_vOffsetRot = vRot;
	}

	//回転関数
	void					RotByEuler(const Vector3& vRot);
	void					RotByPitch(const float& fRadian);
	void					RotByYaw(const float& fRadian);
	void					RotByRoll(const float& fRadian);
	void					RotByUp(const Vector3& vUp);
	void					RotByForward(const Vector3& vForward);
	void					RotByRight(const Vector3& vRight);

	//他の関数
	Vector3					TransformDirectionToWorld(const Vector3& vDirection);
	Vector3					TransformDirectionToLocal(const Vector3& vDirection);

private:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	void		calculateMtxThis(void);
	void		calculateMtxThis(const Matrix44& mtxParent);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	//パラメーター
	Vector3		m_vPos;			//オブジェクト位置
	Vector3		m_vPosNext;		//次のオブジェクト位置
	Vector3		m_vScale;		//オブジェクトサイズ
	Vector3		m_vScaleNext;	//次のオブジェクトサイズ
	Vector3		m_vForward;		//オブジェクトの前方向
	Vector3		m_vForwardNext;	//次のオブジェクトの前方向
	Vector3		m_vUp;			//オブジェクトの上方向
	Vector3		m_vUpNext;		//次のオブジェクトの上方向
	Vector3		m_vRight;		//オブジェクトの右方向
	Vector3		m_vRightNext;	//次のオブジェクトの右方向
	Matrix44	m_mtxThis;		//自分の回転平行移動行列
	
	//親子関係
	CTransformComponent*			m_pParent;
	list<CTransformComponent*>		m_listChildren;
	Vector3							m_vOffsetPos;
	Vector3							m_vOffsetRot;
};