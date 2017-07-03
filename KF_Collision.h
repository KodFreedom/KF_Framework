//--------------------------------------------------------------------------------
//	コリジョン判定クラス
//　KF_Collision.h
//	Author : Xu Wenjie
//	Date   : 2017-06-05
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class CSphereColliderComponent;
class CFieldColliderComponent;

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class CKFCollision
{
public:
	static void CheckCollisionSphereWithSphere(CSphereColliderComponent& sphereL, CSphereColliderComponent& sphereR);
	static void CheckCollisionSphereWithField(CSphereColliderComponent& sphere, CFieldColliderComponent& field);

private:
	CKFCollision() {}
	~CKFCollision() {}
};