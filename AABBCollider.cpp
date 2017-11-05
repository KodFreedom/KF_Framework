//--------------------------------------------------------------------------------
//	AABBCollider�R���|�l���g
//�@AABBCollider.cpp
//	Author : Xu Wenjie
//	Date   : 2017-07-28
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
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
//	�X�V����
//--------------------------------------------------------------------------------
void AABBCollider::Update(void)
{
	BoxCollider::Update();

	//��]������������
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