//--------------------------------------------------------------------------------
//	AABBColliderコンポネント
//　AABBCollider.cpp
//	Author : Xu Wenjie
//	Date   : 2017-07-28
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "AABBCollider.h"
#include "gameObject.h"
#include "transform.h"

//--------------------------------------------------------------------------------
//
//	public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	更新処理
//--------------------------------------------------------------------------------
void AABBCollider::Update(void)
{
	BoxCollider::Update();

	//回転を初期化する
	auto& scale = owner->GetTransformComponent()->GetScaleNext();
	nextWorldMatrix.Elements[0][0] = scale.X;
	nextWorldMatrix.Elements[0][1] = 0.0f;
	nextWorldMatrix.Elements[0][2] = 0.0f;
	nextWorldMatrix.Elements[1][0] = 0.0f;
	nextWorldMatrix.Elements[1][1] = scale.Y;
	nextWorldMatrix.Elements[1][2] = 0.0f;
	nextWorldMatrix.Elements[2][0] = 0.0f;
	nextWorldMatrix.Elements[2][1] = 0.0f;
	nextWorldMatrix.Elements[2][2] = scale.Z;
}