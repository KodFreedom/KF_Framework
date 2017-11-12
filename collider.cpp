//--------------------------------------------------------------------------------
//	コリジョンコンポネント
//　collisionComponent.cpp
//	Author : Xu Wenjie
//	Date   : 2017-06-04
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "collider.h"
#include "manager.h"
#include "gameObject.h"

//--------------------------------------------------------------------------------
//
//  public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
Collider::Collider(GameObject* const owner, const ColliderType& type, const ColliderMode& mode)
	: Component(owner)
	, type(type)
	, mode(mode)
	, isTrigger(false)
	, isRegistered(false)
	, nextWorldMatrix(Matrix44::Identity)
	, offset(Matrix44::Identity)
{}

//--------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------
void Collider::Uninit(void)
{
	if (isRegistered)
	{
		CollisionSystem::Instance()->Deregister(this);
	}
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void Collider::Update(void)
{
	nextWorldMatrix = offset * owner->GetTransform()->GetNextWorldMatrix();
}

//--------------------------------------------------------------------------------
//	関数名：SetOffset
//  関数説明：相対位置の設定
//	引数：	position：位置
//			rotation：回転
//	戻り値：なし
//--------------------------------------------------------------------------------
void Collider::SetOffset(const Vector3& position, const Vector3& rotation)
{
	offset = Matrix44::RotationYawPitchRoll(rotation);
	offset.Elements[3][0] = position.X;
	offset.Elements[3][1] = position.Y;
	offset.Elements[3][2] = position.Z;
}

//--------------------------------------------------------------------------------
//	関数名：Sleep
//  関数説明：一時的に当たり判定処理から抜ける
//	引数：	なし
//	戻り値：なし
//--------------------------------------------------------------------------------
void Collider::Sleep(void)
{
	if (!isRegistered) return;
	isRegistered = false;
	CollisionSystem::Instance()->Deregister(this);
}

//--------------------------------------------------------------------------------
//	関数名：Awake
//  関数説明：当たり判定処理に登録する
//	引数：	なし
//	戻り値：なし
//--------------------------------------------------------------------------------
void Collider::Awake(void)
{
	isRegistered = true;
	CollisionSystem::Instance()->Register(this);
}