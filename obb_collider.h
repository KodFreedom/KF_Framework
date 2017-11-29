//--------------------------------------------------------------------------------
//	OBBColliderコンポネント
//　OBBCollider.h
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#pragma once
#include "box_collider.h"

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class ObbCollider : public BoxCollider
{
public:
	//--------------------------------------------------------------------------------
	//  constructors and destructors
	//--------------------------------------------------------------------------------
	ObbCollider(GameObject& owner, const ColliderMode& mode, const Vector3& half_size)
		: BoxCollider(owner, kObb, mode, half_size) {}
	~ObbCollider() {}
};