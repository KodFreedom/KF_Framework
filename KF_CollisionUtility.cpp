//--------------------------------------------------------------------------------
//	�R���W�����v�Z�֐�
//�@KF_CollisionUtility.cpp
//	Author : Xu Wenjie
//	Date   : 2017-08-13
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "KF_CollisionUtility.h"
#include "KF_PhysicsSystem.h"
#include "sphereColliderComponent.h"
#include "fieldColliderComponent.h"
#include "boxColliderComponent.h"
#include "AABBColliderComponent.h"
#include "OBBColliderComponent.h"
#include "gameObject.h"
#include "3DRigidbodyComponent.h"
#include "behaviorComponent.h"
#include "transformComponent.h"

#ifdef _DEBUG
#include "debugManager.h"
#endif

//--------------------------------------------------------------------------------
//
//  �Փ˔���֐�
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	�֐����FCheckSphereWithSphere
//  �֐������F�X�t�B�A�ƃX�t�B�A�̓����蔻��֐�
//			�@����������R���W�������𕨗����Z�}�l�[�W���ɓo�^����
//	�����F	sphereL�F�X�t�B�A�R���C�_�[
//			sphereR�F�X�t�B�A�R���C�_�[
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void CCollisionDetector::CheckSphereWithSphere(CSphereColliderComponent& sphereL, CSphereColliderComponent& sphereR)
{
	const auto& vSLPos = sphereL.GetWorldPos();
	const auto& fSLRadius = sphereL.GetRadius();
	const auto& vSRPos = sphereR.GetWorldPos();
	const auto& fSRRadius = sphereR.GetRadius();
	auto& vMidLine = vSLPos - vSRPos;
	float fDisMin = fSLRadius + fSRRadius;
	float fDisSqr = CKFMath::VecMagnitudeSquare(vMidLine);

	if (fDisSqr >= fDisMin * fDisMin) { return; }

	if (sphereL.IsTrigger() || sphereR.IsTrigger())
	{//�g���K�[�������畨���������Ȃ�
		auto& listL = sphereL.GetGameObject()->GetBehaviorComponent();
		for (auto& pBehavior : listL) { pBehavior->OnTrigger(sphereL, sphereR); }

		auto& listR = sphereR.GetGameObject()->GetBehaviorComponent();
		for (auto& pBehavior : listR) { pBehavior->OnTrigger(sphereR, sphereL); }
		return;
	}

	auto pCollision = new CCollision;

	//�Փ˓_�̎Z�o
	pCollision->m_vCollisionPos = vSRPos + vMidLine * 0.5f;

	//�Փː[�x�̎Z�o
	float fDis = sqrtf(fDisSqr);
	pCollision->m_fPenetration = fDisMin - fDis;

	//�Փ˖@���̎Z�o
	pCollision->m_vCollisionNormal = vMidLine / fDis;

	//���W�b�h�{�f�B�̎擾
	auto pRBL = sphereL.GetGameObject()->GetRigidbodyComponent();
	auto pRBR = sphereR.GetGameObject()->GetRigidbodyComponent();
	if (pRBL->GetType() == CRigidbodyComponent::RB_3D)
	{
		pCollision->m_pRigidBodyOne = dynamic_cast<C3DRigidbodyComponent*>(pRBL);

		if (pRBR->GetType() == CRigidbodyComponent::RB_3D)
		{
			pCollision->m_pRigidBodyTwo = dynamic_cast<C3DRigidbodyComponent*>(pRBR);
		}
	}
	else
	{//��Ԃ������ĂȂ��Ȃ�Փ˖@���𔽓]����
		pCollision->m_vCollisionNormal *= -1.0f;
		pCollision->m_pRigidBodyOne = dynamic_cast<C3DRigidbodyComponent*>(pRBR);
	}

	//�������Z�V�X�e���Ƀ��W�X�g��
	CMain::GetManager()->GetPhysicsSystem()->RegisterCollision(pCollision);

	//OnCollision
	CCollisionInfo info;
	info.m_pColliderThis = &sphereL;
	info.m_pCollider = &sphereR;
	info.m_listCollision.push_back(pCollision);
	auto& listL = sphereL.GetGameObject()->GetBehaviorComponent();
	for (auto& pBehavior : listL) { pBehavior->OnCollision(info); }

	info.m_pColliderThis = &sphereR;
	info.m_pCollider = &sphereL;
	auto& listR = sphereR.GetGameObject()->GetBehaviorComponent();
	for (auto& pBehavior : listR) { pBehavior->OnCollision(info); }
}

//--------------------------------------------------------------------------------
//	�֐����FCheckSphereWithAABB
//  �֐������F�X�t�B�A��AABB�̓����蔻��֐�
//			�@����������R���W�������𕨗����Z�}�l�[�W���ɓo�^����
//	�����F	sphere�F�X�t�B�A�R���C�_�[
//			aabb�F��]�Ȃ��{�b�N�X�R���C�_�[
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void CCollisionDetector::CheckSphereWithAABB(CSphereColliderComponent& sphere, CAABBColliderComponent& aabb)
{
	const auto& vSPos = sphere.GetWorldPos();
	const auto& fSRadius = sphere.GetRadius();
	const auto& vBHalfSize = aabb.GetHalfSize();
	const auto& vBPos = aabb.GetWorldPos();
	auto& vRealSPos = vSPos - vBPos;

	//�������`�F�b�N
	if (fabsf(vRealSPos.m_fX) - fSRadius > vBHalfSize.m_fX
		|| fabsf(vRealSPos.m_fY) - fSRadius > vBHalfSize.m_fY
		|| fabsf(vRealSPos.m_fZ) - fSRadius > vBHalfSize.m_fZ)
	{
		return;
	}

	CKFVec3 vClosestPos;
	float fDis;

	//AABB�ƃX�t�B�A�̍ŋߓ_�̎Z�o
	fDis = vRealSPos.m_fX;
	if (fDis > vBHalfSize.m_fX) { fDis = vBHalfSize.m_fX; }
	else if (fDis < -vBHalfSize.m_fX) { fDis = -vBHalfSize.m_fX; }
	vClosestPos.m_fX = fDis;

	fDis = vRealSPos.m_fY;
	if (fDis > vBHalfSize.m_fY) { fDis = vBHalfSize.m_fY; }
	else if (fDis < -vBHalfSize.m_fY) { fDis = -vBHalfSize.m_fY; }
	vClosestPos.m_fY = fDis;

	fDis = vRealSPos.m_fZ;
	if (fDis > vBHalfSize.m_fZ) { fDis = vBHalfSize.m_fZ; }
	else if (fDis < -vBHalfSize.m_fZ) { fDis = -vBHalfSize.m_fZ; }
	vClosestPos.m_fZ = fDis;

	//�Փˌ��m
	float fDisSqr = CKFMath::VecMagnitudeSquare(vClosestPos - vRealSPos);
	if (fDisSqr >= fSRadius * fSRadius) { return; }

	//OnTrigger
	if (sphere.IsTrigger() || aabb.IsTrigger())
	{//�g���K�[�������畨���������Ȃ�
		auto& listL = sphere.GetGameObject()->GetBehaviorComponent();
		for (auto& pBehavior : listL) { pBehavior->OnTrigger(sphere, aabb); }

		auto& listR = aabb.GetGameObject()->GetBehaviorComponent();
		for (auto& pBehavior : listR) { pBehavior->OnTrigger(aabb, sphere); }
		return;
	}

	//�Փˏ��
	vClosestPos = vClosestPos + vBPos;
	auto pCollision = new CCollision;
	pCollision->m_vCollisionNormal = vSPos - vClosestPos;
	CKFMath::VecNormalize(pCollision->m_vCollisionNormal);
	pCollision->m_vCollisionPos = vClosestPos;
	pCollision->m_fPenetration = fSRadius - sqrtf(fDisSqr);

	//���W�b�h�{�f�B�̎擾
	auto pRBL = sphere.GetGameObject()->GetRigidbodyComponent();
	auto pRBR = aabb.GetGameObject()->GetRigidbodyComponent();
	if (pRBL->GetType() == CRigidbodyComponent::RB_3D)
	{
		pCollision->m_pRigidBodyOne = dynamic_cast<C3DRigidbodyComponent*>(pRBL);

		if (pRBR->GetType() == CRigidbodyComponent::RB_3D)
		{
			pCollision->m_pRigidBodyTwo = dynamic_cast<C3DRigidbodyComponent*>(pRBR);
		}
	}
	else
	{//��Ԃ������ĂȂ��Ȃ�Փ˖@���𔽓]����
		pCollision->m_vCollisionNormal *= -1.0f;
		pCollision->m_pRigidBodyOne = dynamic_cast<C3DRigidbodyComponent*>(pRBR);
	}

	//�������Z�V�X�e���Ƀ��W�X�g��
	CMain::GetManager()->GetPhysicsSystem()->RegisterCollision(pCollision);

	//OnCollision
	CCollisionInfo info;
	info.m_pColliderThis = &sphere;
	info.m_pCollider = &aabb;
	info.m_listCollision.push_back(pCollision);
	auto& listL = sphere.GetGameObject()->GetBehaviorComponent();
	for (auto& pBehavior : listL) { pBehavior->OnCollision(info); }

	info.m_pColliderThis = &aabb;
	info.m_pCollider = &sphere;
	auto& listR = aabb.GetGameObject()->GetBehaviorComponent();
	for (auto& pBehavior : listR) { pBehavior->OnCollision(info); }
}

//--------------------------------------------------------------------------------
//	�֐����FCheckSphereWithOBB
//  �֐������F�X�t�B�A��OBB�̓����蔻��֐�
//			�@����������R���W�������𕨗����Z�}�l�[�W���ɓo�^����
//	�����F	sphere�F�X�t�B�A�R���C�_�[
//			obb�F��]����{�b�N�X�R���C�_�[
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void CCollisionDetector::CheckSphereWithOBB(CSphereColliderComponent& sphere, COBBColliderComponent& obb)
{
	const auto& vSPos = sphere.GetWorldPos();
	const auto& fSRadius = sphere.GetRadius();
	const auto& vBHalfSize = obb.GetHalfSize();
	const auto& mtxBox = obb.GetMatrixWorld();
	auto& vRealSPos = CKFMath::TransformInverse(mtxBox, vSPos);

	//�������`�F�b�N
	if (fabsf(vRealSPos.m_fX) - fSRadius > vBHalfSize.m_fX
		|| fabsf(vRealSPos.m_fY) - fSRadius > vBHalfSize.m_fY
		|| fabsf(vRealSPos.m_fZ) - fSRadius > vBHalfSize.m_fZ)
	{
		return;
	}

	CKFVec3 vClosestPos;
	float fDis;

	//OBB�ƃX�t�B�A�̍ŋߓ_�̎Z�o
	fDis = vRealSPos.m_fX;
	if (fDis > vBHalfSize.m_fX) { fDis = vBHalfSize.m_fX; }
	else if (fDis < -vBHalfSize.m_fX) { fDis = -vBHalfSize.m_fX; }
	vClosestPos.m_fX = fDis;

	fDis = vRealSPos.m_fY;
	if (fDis > vBHalfSize.m_fY) { fDis = vBHalfSize.m_fY; }
	else if (fDis < -vBHalfSize.m_fY) { fDis = -vBHalfSize.m_fY; }
	vClosestPos.m_fY = fDis;

	fDis = vRealSPos.m_fZ;
	if (fDis > vBHalfSize.m_fZ) { fDis = vBHalfSize.m_fZ; }
	else if (fDis < -vBHalfSize.m_fZ) { fDis = -vBHalfSize.m_fZ; }
	vClosestPos.m_fZ = fDis;

	//�Փˌ��m
	float fDisSqr = CKFMath::VecMagnitudeSquare(vClosestPos - vRealSPos);
	if (fDisSqr >= fSRadius * fSRadius) { return; }

	//OnTrigger
	if (sphere.IsTrigger() || obb.IsTrigger())
	{//�g���K�[�������畨���������Ȃ�
		auto& listL = sphere.GetGameObject()->GetBehaviorComponent();
		for (auto& pBehavior : listL) { pBehavior->OnTrigger(sphere, obb); }

		auto& listR = obb.GetGameObject()->GetBehaviorComponent();
		for (auto& pBehavior : listR) { pBehavior->OnTrigger(obb, sphere); }
		return;
	}

	//�Փˏ��
	vClosestPos = CKFMath::Vec3TransformCoord(vClosestPos, mtxBox);
	auto pCollision = new CCollision;
	pCollision->m_vCollisionNormal = vSPos - vClosestPos;
	CKFMath::VecNormalize(pCollision->m_vCollisionNormal);
	pCollision->m_vCollisionPos = vClosestPos;
	pCollision->m_fPenetration = fSRadius - sqrtf(fDisSqr);

	//���W�b�h�{�f�B�̎擾
	CRigidbodyComponent* pRBL = sphere.GetGameObject()->GetRigidbodyComponent();
	CRigidbodyComponent* pRBR = obb.GetGameObject()->GetRigidbodyComponent();
	if (pRBL->GetType() == CRigidbodyComponent::RB_3D)
	{
		pCollision->m_pRigidBodyOne = dynamic_cast<C3DRigidbodyComponent*>(pRBL);

		if (pRBR->GetType() == CRigidbodyComponent::RB_3D)
		{
			pCollision->m_pRigidBodyTwo = dynamic_cast<C3DRigidbodyComponent*>(pRBR);
		}
	}
	else
	{//��Ԃ������ĂȂ��Ȃ�Փ˖@���𔽓]����
		pCollision->m_vCollisionNormal *= -1.0f;
		pCollision->m_pRigidBodyOne = dynamic_cast<C3DRigidbodyComponent*>(pRBR);
	}

	//�������Z�V�X�e���Ƀ��W�X�g��
	CMain::GetManager()->GetPhysicsSystem()->RegisterCollision(pCollision);

	//OnCollision
	CCollisionInfo info;
	info.m_pColliderThis = &sphere;
	info.m_pCollider = &obb;
	info.m_listCollision.push_back(pCollision);
	auto& listL = sphere.GetGameObject()->GetBehaviorComponent();
	for (auto& pBehavior : listL) { pBehavior->OnCollision(info); }

	info.m_pColliderThis = &obb;
	info.m_pCollider = &sphere;
	auto& listR = obb.GetGameObject()->GetBehaviorComponent();
	for (auto& pBehavior : listR) { pBehavior->OnCollision(info); }
}

//--------------------------------------------------------------------------------
//	�֐����FCheckAABBWithAABB
//  �֐������FAABB��AABB�̓����蔻��֐�
//			�@����������R���W�������𕨗����Z�}�l�[�W���ɓo�^����
//	�����F	aabbL�F��]�Ȃ��{�b�N�X�R���C�_�[
//			aabbR�F��]�Ȃ��{�b�N�X�R���C�_�[
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void CCollisionDetector::CheckAABBWithAABB(CAABBColliderComponent& aabbL, CAABBColliderComponent& aabbR)
{
	if (!checkOverlapAABB(aabbL, aabbR)) { return; }

	//OnTrigger
	if (aabbL.IsTrigger() || aabbR.IsTrigger())
	{//�g���K�[�������畨���������Ȃ�
		auto& listL = aabbL.GetGameObject()->GetBehaviorComponent();
		for (auto& pBehavior : listL) { pBehavior->OnTrigger(aabbL, aabbR); }

		auto& listR = aabbR.GetGameObject()->GetBehaviorComponent();
		for (auto& pBehavior : listR) { pBehavior->OnTrigger(aabbR, aabbL); }
		return;
	}

	//XYZ����Ԑ[�x���󂢂̎���􂢏o��
	const auto& vPosL = aabbL.GetWorldPos();
	const auto& vHalfSizeL = aabbL.GetHalfSize();
	const auto& vPosR = aabbR.GetWorldPos();
	const auto& vHalfSizeR = aabbR.GetHalfSize();
	auto vMidLine = vPosL - vPosR;
	auto vDisNoCol = vHalfSizeL + vHalfSizeR;
	auto fPenetrationX = vDisNoCol.m_fX - fabsf(vMidLine.m_fX);
	auto fPenetrationY = vDisNoCol.m_fY - fabsf(vMidLine.m_fY);
	auto fPenetrationZ = vDisNoCol.m_fZ - fabsf(vMidLine.m_fZ);

	fPenetrationX = fPenetrationX > 0.0f ? fPenetrationX : vDisNoCol.m_fX;
	fPenetrationY = fPenetrationY > 0.0f ? fPenetrationY : vDisNoCol.m_fY;
	fPenetrationZ = fPenetrationZ > 0.0f ? fPenetrationZ : vDisNoCol.m_fZ;
	auto fPenetrationMin = min(fPenetrationX, min(fPenetrationY, fPenetrationZ));
	
	auto pCollision = new CCollision;
	pCollision->m_fPenetration = fPenetrationMin;
	pCollision->m_vCollisionPos = vMidLine * 0.5f;
	if (fPenetrationX == fPenetrationMin)
	{
		pCollision->m_vCollisionNormal = vMidLine.m_fX < 0.0f ? CKFVec3(-1.0f, 0.0f, 0.0f) : CKFVec3(1.0f, 0.0f, 0.0f);
	}
	else if (fPenetrationY == fPenetrationMin)
	{
		pCollision->m_vCollisionNormal = vMidLine.m_fY < 0.0f ? CKFVec3(0.0f, -1.0f, 0.0f) : CKFVec3(0.0f, 1.0f, 0.0f);
	}
	else
	{
		pCollision->m_vCollisionNormal = vMidLine.m_fZ < 0.0f ? CKFVec3(0.0f, 0.0f, -1.0f) : CKFVec3(0.0f, 0.0f, 1.0f);
	}

	//���W�b�h�{�f�B�̎擾
	auto pRBL = aabbL.GetGameObject()->GetRigidbodyComponent();
	auto pRBR = aabbR.GetGameObject()->GetRigidbodyComponent();

	if (pRBL->GetType() == CRigidbodyComponent::RB_3D)
	{
		pCollision->m_pRigidBodyOne = dynamic_cast<C3DRigidbodyComponent*>(pRBL);

		if (pRBR->GetType() == CRigidbodyComponent::RB_3D)
		{
			pCollision->m_pRigidBodyTwo = dynamic_cast<C3DRigidbodyComponent*>(pRBR);
		}
	}
	else
	{//��Ԃ������ĂȂ��Ȃ�Փ˖@���𔽓]����
		pCollision->m_vCollisionNormal *= -1.0f;
		pCollision->m_pRigidBodyOne = dynamic_cast<C3DRigidbodyComponent*>(pRBR);
	}

	//�������Z�V�X�e���Ƀ��W�X�g��
	CMain::GetManager()->GetPhysicsSystem()->RegisterCollision(pCollision);

	//OnCollision
	CCollisionInfo info;
	info.m_listCollision.push_back(pCollision);
	info.m_pColliderThis = &aabbL;
	info.m_pCollider = &aabbR;
	auto& listL = aabbL.GetGameObject()->GetBehaviorComponent();
	for (auto& pBehavior : listL) { pBehavior->OnCollision(info); }

	info.m_pColliderThis = &aabbR;
	info.m_pCollider = &aabbL;
	auto& listR = aabbR.GetGameObject()->GetBehaviorComponent();
	for (auto& pBehavior : listR) { pBehavior->OnCollision(info); }
}

//--------------------------------------------------------------------------------
//	�֐����FCheckBoxWithBox
//  �֐������Fbox��box�̓����蔻��֐�(�����ꂩ���K��OBB)
//			�@����������R���W�������𕨗����Z�}�l�[�W���ɓo�^����
//	�����F	boxL�F�{�b�N�X�R���C�_�[
//			boxR�F�{�b�N�X�R���C�_�[
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void CCollisionDetector::CheckBoxWithBox(CBoxColliderComponent& boxL, CBoxColliderComponent& boxR)
{
	//�{�b�N�X��xyz����ɏd�˂Ă邩�ǂ������`�F�b�N����
	if (!checkOverlapOnAxis(boxL, boxR, CKFVec3(1.0f, 0.0f, 0.0f))
		&& !checkOverlapOnAxis(boxL, boxR, CKFVec3(0.0f, 1.0f, 0.0f))
		&& !checkOverlapOnAxis(boxL, boxR, CKFVec3(0.0f, 0.0f, 1.0f)))
	{
		return;
	}

	bool bFindL = false;
	CCollision collisionDepthMaxL;

	//boxL�̂��ׂĂ̒��_��boxR�Ɣ��肵�A�߂荞�݂���Ԑ[���̒��_��􂢏o��
	auto& listVtxBoxL = boxL.GetWorldVertexes();
	for (auto itr = listVtxBoxL.begin(); itr != listVtxBoxL.end(); ++itr)
	{
		CCollision collision;
		if (checkPointWithBox(collision, *itr, boxR))
		{
			if (!bFindL)
			{
				bFindL = true;
				collisionDepthMaxL = collision;
			}
			else if (collision.m_fPenetration > collisionDepthMaxL.m_fPenetration)
			{
				collisionDepthMaxL = collision;
			}
		}
	}

	bool bFindR = false;
	CCollision collisionDepthMaxR;

	//boxR�̂��ׂĂ̒��_��boxL�Ɣ��肵�A�߂荞�݂���Ԑ[���̒��_��􂢏o��
	auto& listVtxBoxR = boxR.GetWorldVertexes();
	for (auto itr = listVtxBoxR.begin(); itr != listVtxBoxR.end(); ++itr)
	{
		CCollision collision;
		if (checkPointWithBox(collision, *itr, boxL))
		{
			if (!bFindR)
			{
				bFindR = true;
				collisionDepthMaxR = collision;
			}
			else if (collision.m_fPenetration > collisionDepthMaxR.m_fPenetration)
			{
				collisionDepthMaxR = collision;
			}
		}
	}

	if (!bFindL && !bFindR) { return; }

	//OnTrigger
	if (boxL.IsTrigger() || boxR.IsTrigger())
	{//�g���K�[�������畨���������Ȃ�
		auto& listL = boxL.GetGameObject()->GetBehaviorComponent();
		for (auto& pBehavior : listL) { pBehavior->OnTrigger(boxL, boxR); }

		auto& listR = boxR.GetGameObject()->GetBehaviorComponent();
		for (auto& pBehavior : listR) { pBehavior->OnTrigger(boxR, boxL); }
		return;
	}

	CCollisionInfo info;

	//���W�b�h�{�f�B�̎擾
	auto pRBL = boxL.GetGameObject()->GetRigidbodyComponent();
	auto pRBR = boxR.GetGameObject()->GetRigidbodyComponent();

	if (bFindL)
	{
		if (pRBL->GetType() == CRigidbodyComponent::RB_3D)
		{
			collisionDepthMaxL.m_pRigidBodyOne = dynamic_cast<C3DRigidbodyComponent*>(pRBL);

			if (pRBR->GetType() == CRigidbodyComponent::RB_3D)
			{
				collisionDepthMaxL.m_pRigidBodyTwo = dynamic_cast<C3DRigidbodyComponent*>(pRBR);
			}
		}
		else
		{//��Ԃ������ĂȂ��Ȃ�Փ˖@���𔽓]����
			collisionDepthMaxL.m_vCollisionNormal *= -1.0f;
			collisionDepthMaxL.m_pRigidBodyOne = dynamic_cast<C3DRigidbodyComponent*>(pRBR);
		}

		//�������Z�V�X�e���Ƀ��W�X�g��
		auto pCollision = new CCollision;
		*pCollision = collisionDepthMaxL;
		CMain::GetManager()->GetPhysicsSystem()->RegisterCollision(pCollision);
		info.m_listCollision.push_back(&collisionDepthMaxL);
	}

	if (bFindR)
	{
		if (pRBR->GetType() == CRigidbodyComponent::RB_3D)
		{
			collisionDepthMaxR.m_pRigidBodyOne = dynamic_cast<C3DRigidbodyComponent*>(pRBR);

			if (pRBL->GetType() == CRigidbodyComponent::RB_3D)
			{
				collisionDepthMaxR.m_pRigidBodyTwo = dynamic_cast<C3DRigidbodyComponent*>(pRBL);
			}
		}
		else
		{//��Ԃ������ĂȂ��Ȃ�Փ˖@���𔽓]����
			collisionDepthMaxR.m_vCollisionNormal *= -1.0f;
			collisionDepthMaxR.m_pRigidBodyOne = dynamic_cast<C3DRigidbodyComponent*>(pRBL);
		}

		//�������Z�V�X�e���Ƀ��W�X�g��
		auto pCollision = new CCollision;
		*pCollision = collisionDepthMaxR;
		CMain::GetManager()->GetPhysicsSystem()->RegisterCollision(pCollision);
		info.m_listCollision.push_back(&collisionDepthMaxR);
	}

	//OnCollision
	info.m_pColliderThis = &boxL;
	info.m_pCollider = &boxR;
	info.m_listCollision.push_back(&collisionDepthMaxL);
	auto& listL = boxL.GetGameObject()->GetBehaviorComponent();
	for (auto& pBehavior : listL) { pBehavior->OnCollision(info); }

	info.m_pColliderThis = &boxR;
	info.m_pCollider = &boxL;
	auto& listR = boxR.GetGameObject()->GetBehaviorComponent();
	for (auto& pBehavior : listR) { pBehavior->OnCollision(info); }
}

//--------------------------------------------------------------------------------
//
//  �t�B�[���h
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	�֐����FCheckSphereWithField
//  �֐������F�X�t�B�A�ƃt�B�[���h�̓����蔻��֐�
//			�@����������R���W�������𕨗����Z�}�l�[�W���ɓo�^����
//	�����F	sphere�F�X�t�B�A�R���C�_�[
//			field�F�t�B�[���h�R���C�_�[
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void CCollisionDetector::CheckSphereWithField(CSphereColliderComponent& sphere, CFieldColliderComponent& field)
{
	const auto& vSpherePos = sphere.GetWorldPos();
	const auto& fSphereRadius = sphere.GetRadius();
	auto info = field.GetProjectionInfo(vSpherePos);

	if (info.bInFieldRange == false) { return; }

	//�X�t�B�A���S�ƃ|���S���̋����̎Z�o
	float fDis = info.fPenetration - fSphereRadius;

	if (fDis >= 0.0f) { return; }

	if (sphere.IsTrigger())
	{
		auto& listL = sphere.GetGameObject()->GetBehaviorComponent();
		for (auto& pBehavior : listL) { pBehavior->OnTrigger(sphere, field); }

		auto& listR = field.GetGameObject()->GetBehaviorComponent();
		for (auto& pBehavior : listR) { pBehavior->OnTrigger(field, sphere); }
		return;
	}
	auto pCollision = new CCollision;

	//�Փ˓_�̎Z�o
	pCollision->m_vCollisionPos = vSpherePos + info.vFaceNormal * fDis;

	//�Փː[�x�̎Z�o
	pCollision->m_fPenetration = -fDis;

	//�Փ˖@���̎Z�o
	pCollision->m_vCollisionNormal = info.vFaceNormal;

	//���W�b�h�{�f�B�̎擾
	auto p3DRB = dynamic_cast<C3DRigidbodyComponent*>(sphere.GetGameObject()->GetRigidbodyComponent());
	pCollision->m_pRigidBodyOne = p3DRB;
	pCollision->m_pRigidBodyTwo = nullptr;

	//�������Z�V�X�e���Ƀ��W�X�g��
	CMain::GetManager()->GetPhysicsSystem()->RegisterCollision(pCollision);

	//OnCollision
	CCollisionInfo cInfo;
	cInfo.m_pColliderThis = &sphere;
	cInfo.m_pCollider = &field;
	cInfo.m_listCollision.push_back(pCollision);
	auto& listL = sphere.GetGameObject()->GetBehaviorComponent();
	for (auto& pBehavior : listL) { pBehavior->OnCollision(cInfo); }

	cInfo.m_pColliderThis = &field;
	cInfo.m_pCollider = &sphere;
	auto& listR = field.GetGameObject()->GetBehaviorComponent();
	for (auto& pBehavior : listR) { pBehavior->OnCollision(cInfo); }
}

//--------------------------------------------------------------------------------
//	�֐����FCheckBoxWithField
//  �֐������FBox�ƃt�B�[���h�̓����蔻��֐�
//			�@����������R���W�������𕨗����Z�}�l�[�W���ɓo�^����
//	�����F	box�F�{�b�N�X�R���C�_�[
//			field�F�t�B�[���h�R���C�_�[
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void CCollisionDetector::CheckBoxWithField(CBoxColliderComponent& box, CFieldColliderComponent& field)
{
	auto& listOBBVtx = box.GetWorldVertexes();

	//��]�Ȃ��̏ꍇ��Ԑ[���̒��_���ďo����
	bool bFind = false;
	auto pCollision = new CCollision;
	for (auto itr = listOBBVtx.begin(); itr != listOBBVtx.end(); ++itr)
	{
		auto info = field.GetProjectionInfo((*itr));
		if (!info.bInFieldRange) { continue; }
		if (info.fPenetration <= 0.0f) { continue; }

		if (!bFind)
		{
			bFind = true;

			//�Փ˓_�̎Z�o
			pCollision->m_vCollisionPos = *itr;

			//�Փː[�x�̎Z�o
			pCollision->m_fPenetration = info.fPenetration;

			//�Փ˖@���̎Z�o
			pCollision->m_vCollisionNormal = info.vFaceNormal;

			//���W�b�h�{�f�B�̎擾
			auto p3DRB = dynamic_cast<C3DRigidbodyComponent*>(box.GetGameObject()->GetRigidbodyComponent());
			pCollision->m_pRigidBodyOne = p3DRB;
			pCollision->m_pRigidBodyTwo = nullptr;
		}
		else
		{//�[�x���ׂ�
			if (info.fPenetration <= pCollision->m_fPenetration) { continue; }

			//�Փ˓_�̎Z�o
			pCollision->m_vCollisionPos = *itr;

			//�Փː[�x�̎Z�o
			pCollision->m_fPenetration = info.fPenetration;

			//�Փ˖@���̎Z�o
			pCollision->m_vCollisionNormal = info.vFaceNormal;
		}
	}

	if (!bFind) { return; }

	if (box.IsTrigger())
	{
		auto& listL = box.GetGameObject()->GetBehaviorComponent();
		for (auto& pBehavior : listL) { pBehavior->OnTrigger(box, field); }

		auto& listR = field.GetGameObject()->GetBehaviorComponent();
		for (auto& pBehavior : listR) { pBehavior->OnTrigger(field, box); }
		
		delete pCollision;
		return;
	}

	//�������Z�V�X�e���Ƀ��W�X�g��
	CMain::GetManager()->GetPhysicsSystem()->RegisterCollision(pCollision);

	//OnCollision
	CCollisionInfo cInfo;
	cInfo.m_pColliderThis = &box;
	cInfo.m_pCollider = &field;
	cInfo.m_listCollision.push_back(pCollision);
	auto& listL = box.GetGameObject()->GetBehaviorComponent();
	for (auto& pBehavior : listL) { pBehavior->OnCollision(cInfo); }

	cInfo.m_pColliderThis = &field;
	cInfo.m_pCollider = &box;
	auto& listR = field.GetGameObject()->GetBehaviorComponent();
	for (auto& pBehavior : listR) { pBehavior->OnCollision(cInfo); }
}

//--------------------------------------------------------------------------------
//
//  ���C
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	�֐����FCheckRayWithBox
//  �֐������F���C�ƃ{�b�N�X�̓����蔻��
//	�����F	ray�F���C
//			fDistance�F���C�̒���
//			box�F�{�b�N�X�R���C�_�[
//			infoOut�F���C�̓����蔻����(�o�͗p)
//	�߂�l�Fbool
//--------------------------------------------------------------------------------
bool CCollisionDetector::CheckRayWithBox(const CKFRay& ray, const float& fDistance, CBoxColliderComponent& box, CRaycastHitInfo& infoOut)
{
	CCollision collision;
	if (checkPointWithBox(collision, ray.m_vOrigin, box))
	{
		infoOut.m_vNormal = collision.m_vCollisionNormal;
		infoOut.m_vPos = ray.m_vOrigin;
		infoOut.m_pCollider = &box;
		infoOut.m_fDistance = collision.m_fPenetration;
		return true;
	}
	
	auto& vRayEnd = ray.m_vOrigin + ray.m_vDirection * fDistance;
	if (checkPointWithBox(collision, vRayEnd, box))
	{
		infoOut.m_vNormal = collision.m_vCollisionNormal;
		infoOut.m_vPos = vRayEnd;
		infoOut.m_pCollider = &box;
		infoOut.m_fDistance = collision.m_fPenetration;
		return true;
	}

	return false;
}

//--------------------------------------------------------------------------------
//	�֐����FCheckRayWithSphere
//  �֐������F���C�ƃX�t�B�A�̓����蔻��
//	�����F	ray�F���C
//			fDistance�F���C�̒���
//			sphere�F�X�t�B�A�R���C�_�[
//			infoOut�F���C�̓����蔻����(�o�͗p)
//	�߂�l�Fbool
//--------------------------------------------------------------------------------
bool CCollisionDetector::CheckRayWithSphere(const CKFRay& ray, const float& fDistance, CSphereColliderComponent& sphere, CRaycastHitInfo& infoOut)
{
	CKFVec3 vOriginToSphere;
	const auto& vSpherePos = sphere.GetWorldPos();
	const auto& fRadius = sphere.GetRadius();
	float fWorkA, fWorkB, fTimeA, fTimeB;
	float fDiscriminant;

	vOriginToSphere = ray.m_vOrigin - vSpherePos;
	fWorkA = 2.0f * CKFMath::Vec3Dot(ray.m_vDirection, vOriginToSphere);
	fWorkB = CKFMath::Vec3Dot(vOriginToSphere, vOriginToSphere) - fRadius * fRadius;

	fDiscriminant = fWorkA * fWorkA - 4.0f * fWorkB;

	if (fDiscriminant < 0.0f)
	{
		return false;
	}

	fDiscriminant = sqrtf(fDiscriminant);

	fTimeA = (-fWorkA + fDiscriminant) / 2.0f;
	fTimeB = (-fWorkA - fDiscriminant) / 2.0f;

	if (fTimeA < 0.0f && fTimeB < 0.0f) { return false; }
	
	//�ŒZ���Ԃ�ۑ�
	fTimeA = fTimeA < 0.0f ? fTimeB : fTimeA;
	fTimeB = fTimeB < 0.0f ? fTimeA : fTimeB;
	auto fTimingMin = fTimeA <= fTimeB ? fTimeA : fTimeB;

	if (fTimingMin > fDistance) { return false; }

	infoOut.m_fDistance = fTimingMin;
	infoOut.m_pCollider = &sphere;
	infoOut.m_vPos = ray.m_vOrigin + ray.m_vDirection * fTimingMin;
	infoOut.m_vNormal = infoOut.m_vPos - vSpherePos;
	CKFMath::VecNormalize(infoOut.m_vNormal);
	return true;
}

//--------------------------------------------------------------------------------
//	�֐����FCheckRayWithField
//  �֐������F���C�ƃt�B�[���h�̓����蔻��
//	�����F	ray�F���C
//			fDistance�F���C�̒���
//			field�F�t�B�[���h�R���C�_�[
//			infoOut�F���C�̓����蔻����(�o�͗p)
//	�߂�l�Fbool
//--------------------------------------------------------------------------------
bool CCollisionDetector::CheckRayWithField(const CKFRay& ray, const float& fDistance, CFieldColliderComponent& field, CRaycastHitInfo& infoOut)
{
	//auto vRayMax = ray.m_vOrigin + ray.m_vDirection * fDistance;
	//int nNumVtxX = 0;
	//int nNumVtxZ = 0;
	//vector<CKFVec3> vecVtx;
	//if (!field.GetVtxByRange(ray.m_vOrigin, vRayMax, nNumVtxX, nNumVtxZ, vecVtx))
	//{
	//	return false;
	//}

	//�ꎞ�̗p
	auto info = field.GetProjectionInfo(ray.m_vOrigin + ray.m_vDirection * fDistance);
	if (!info.bInFieldRange) { return false; }
	if (info.fPenetration < 0.0f) { return false; }
	infoOut.m_fDistance = info.fPenetration;
	infoOut.m_vNormal = info.vFaceNormal;
	infoOut.m_pCollider = &field;
	return true;
}

//--------------------------------------------------------------------------------
//
//  private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	�֐����FcheckPointWithAABB
//  �֐������F�_��AABB�̓����蔻��
//	�����F	collisionOut�F�Փˏ��(�o�͗p)
//			vPoint�F�_�̈ʒu
//			aabb�FAABB�R���C�_�[
//	�߂�l�Fbool
//--------------------------------------------------------------------------------
bool CCollisionDetector::checkPointWithAABB(CCollision& collisionOut, const CKFVec3 vPoint, const CAABBColliderComponent& aabb)
{
	const auto& vBPos = aabb.GetWorldPos();
	const auto& vBHalfSize = aabb.GetHalfSize();
	auto& vRealPoint = vPoint - vBPos;

	float fDepthMin = vBHalfSize.m_fX - fabsf(vRealPoint.m_fX);
	if (fDepthMin <= 0.0f) { return false; }
	collisionOut.m_vCollisionNormal = vRealPoint.m_fX < 0.0f ? CKFMath::sc_vLeft : CKFMath::sc_vRight;

	float fDepth = vBHalfSize.m_fY - fabsf(vRealPoint.m_fY);
	if (fDepth <= 0.0f) { return false; }
	else if (fDepth < fDepthMin)
	{
		fDepthMin = fDepth;
		collisionOut.m_vCollisionNormal = vRealPoint.m_fY < 0.0f ? CKFMath::sc_vDown : CKFMath::sc_vUp;
	}

	fDepth = vBHalfSize.m_fZ - fabsf(vRealPoint.m_fZ);
	if (fDepth <= 0.0f) { return false; }
	else if (fDepth < fDepthMin)
	{
		fDepthMin = fDepth;
		collisionOut.m_vCollisionNormal = vRealPoint.m_fZ < 0.0f ? CKFMath::sc_vBack : CKFMath::sc_vForward;
	}

	collisionOut.m_fPenetration = fDepthMin;
	collisionOut.m_vCollisionPos = vPoint;
	return true;
}

//--------------------------------------------------------------------------------
//	�֐����FcheckPointWithBox
//  �֐������F�_��Box�̓����蔻��
//	�����F	collisionOut�F�Փˏ��(�o�͗p)
//			vPoint�F�_�̈ʒu
//			box�Fbox�R���C�_�[
//	�߂�l�Fbool
//--------------------------------------------------------------------------------
bool CCollisionDetector::checkPointWithBox(CCollision& collisionOut, const CKFVec3 vPoint, const CBoxColliderComponent& box)
{
	const auto& mtxBox = box.GetMatrixWorld();
	const auto& vBHalfSize = box.GetHalfSize();
	auto& vRealPoint = CKFMath::TransformInverse(mtxBox, vPoint);

	float fDepthMin = vBHalfSize.m_fX - fabsf(vRealPoint.m_fX);
	if (fDepthMin <= 0.0f) { return false; }
	collisionOut.m_vCollisionNormal = CKFVec3(mtxBox.m_af[0][0], mtxBox.m_af[0][1], mtxBox.m_af[0][2])
		* (vRealPoint.m_fX < 0.0f ? -1.0f : 1.0f);

	float fDepth = vBHalfSize.m_fY - fabsf(vRealPoint.m_fY);
	if (fDepth <= 0.0f) { return false; }
	else if (fDepth < fDepthMin)
	{
		fDepthMin = fDepth;
		collisionOut.m_vCollisionNormal = CKFVec3(mtxBox.m_af[1][0], mtxBox.m_af[1][1], mtxBox.m_af[1][2])
			* (vRealPoint.m_fY < 0.0f ? -1.0f : 1.0f);
	}

	fDepth = vBHalfSize.m_fZ - fabsf(vRealPoint.m_fZ);
	if (fDepth <= 0.0f) { return false; }
	else if (fDepth < fDepthMin)
	{
		fDepthMin = fDepth;
		collisionOut.m_vCollisionNormal = CKFVec3(mtxBox.m_af[2][0], mtxBox.m_af[2][1], mtxBox.m_af[2][2])
			* (vRealPoint.m_fZ < 0.0f ? -1.0f : 1.0f);
	}

	collisionOut.m_fPenetration = fDepthMin;
	collisionOut.m_vCollisionPos = vPoint;
	return true;
}

//--------------------------------------------------------------------------------
//	�֐����FcheckLineWithLine
//  �֐������F�����ƒ����̌�������(�񎟌�)
//	�����F	vLA�FLineLeft�̎n�_
//			vLB�FLineLeft�̏I�_
//			vRA�FLineRight�̎n�_
//			vRB�FLineRight�̏I�_
//			vOut�F�����_(�o�͗p)
//	�߂�l�Fbool
//--------------------------------------------------------------------------------
bool CCollisionDetector::checkLineWithLine(const CKFVec2& vLA, const CKFVec2& vLB, const CKFVec2& vRA, const CKFVec2& vRB, CKFVec2& vOut)
{
	auto& vLineL = vLB - vLA;
	auto& vLineR = vRB - vRA;

	auto fSlopeL = (vLA.m_fX - vLB.m_fX) == 0.0f ? 0.0f
		: (vLA.m_fY - vLB.m_fY) / (vLA.m_fX - vLB.m_fX);
	auto fAddL = vLA.m_fY - fSlopeL * vLA.m_fX;
	auto fSlopeR = (vRA.m_fX - vRB.m_fX) == 0.0f ? 0.0f
		: (vRA.m_fY - vRB.m_fY) / (vRA.m_fX - vRB.m_fX);
	auto fAddR = vRA.m_fY - fSlopeL * vRA.m_fX;
	
	//���s
	if (fSlopeL == fSlopeR) { return false; }
	
	//��_�̎Z�o
	vOut.m_fX = (fAddR - fAddL) / (fSlopeL - fSlopeR);
	vOut.m_fY = fSlopeL * vOut.m_fX + fAddL;

	//��_�����C���͈͓̔��ɂ��邩���`�F�b�N
	//LineL
	auto vDirL = vOut - vLA;
	//�����`�F�b�N
	if (CKFMath::Vec2Dot(vLineL, vDirL) < 0.0f) { return false; }

	//�����`�F�b�N
	if (CKFMath::VecMagnitudeSquare(vDirL) > CKFMath::VecMagnitudeSquare(vLineL)) { return false; }

	//LineR
	auto vDirR = vOut - vRA;
	//�����`�F�b�N
	if (CKFMath::Vec2Dot(vLineR, vDirR) < 0.0f) { return false; }

	//�����`�F�b�N
	if (CKFMath::VecMagnitudeSquare(vDirR) > CKFMath::VecMagnitudeSquare(vLineR)) { return false; }

	return true;
}

//--------------------------------------------------------------------------------
//	�֐����FcheckLineWithLine
//  �֐������F�����ƒ����̌�������(�O����)
//	�����F	vLA�FLineLeft�̎n�_
//			vLB�FLineLeft�̏I�_
//			vRA�FLineRight�̎n�_
//			vRB�FLineRight�̏I�_
//			vOut�F�����_(�o�͗p)
//	�߂�l�Fbool
//--------------------------------------------------------------------------------
bool CCollisionDetector::checkLineWithLine(const CKFVec3& vLA, const CKFVec3& vLB, const CKFVec3& vRA, const CKFVec3& vRB, CKFVec3& vOut)
{
	auto vLineL = vLA - vLB;
	auto vLineR = vRA - vRB;

	if ((vLineL * vLineR) == CKFVec3(0.0f))
	{//���s
		return false;
	}

	//XY���ʂ̌�_�̎Z�o
	CKFVec2 vOutXY;
	if (!checkLineWithLine(CKFVec2(vLA.m_fX, vLA.m_fY), CKFVec2(vLA.m_fX, vLA.m_fY), CKFVec2(vLA.m_fX, vLA.m_fY), CKFVec2(vLA.m_fX, vLA.m_fY), vOutXY))
	{
		return false;
	}

	CKFMath::VecNormalize(vLineL);
	CKFMath::VecNormalize(vLineR);

	//������Z�l���Z�o����
	auto fRateL = vLineL.m_fX != 0.0f ? (vOutXY.m_fX - vLA.m_fX) / vLineL.m_fX
		: vLineL.m_fY != 0.0f ? (vOutXY.m_fY - vLA.m_fY) / vLineL.m_fY
		: 0.0f;
	auto fRateR = vLineR.m_fX != 0.0f ? (vOutXY.m_fX - vRA.m_fX) / vLineR.m_fX
		: vLineR.m_fY != 0.0f ? (vOutXY.m_fY - vRA.m_fY) / vLineR.m_fY
		: 0.0f;
	auto fZL = vLA.m_fZ + fRateL * vLineL.m_fZ;
	auto fZR = vRA.m_fZ + fRateR * vLineR.m_fZ;
	if (fZL != fZR) { return false; }

	vOut = CKFVec3(vOutXY.m_fX, vOutXY.m_fY, fZL);
	return true;
}

//--------------------------------------------------------------------------------
//	�֐����FtransformBoxToAxis
//  �֐������Fbox��^����ꂽ���ɓ��e���Ē������Z�o����֐�
//	�����F	box�F�{�b�N�X�R���C�_�[
//			vAxis�F�����
//	�߂�l�Ffloat
//--------------------------------------------------------------------------------
float CCollisionDetector::transformBoxToAxis(const CBoxColliderComponent& box, const CKFVec3& vAxis)
{
	const CKFVec3& vHalfSize = box.GetHalfSize();
	const CKFMtx44& mtxBox = box.GetMatrixWorld();
	float fAnswer = vHalfSize.m_fX * fabsf(CKFMath::Vec3Dot(vAxis, CKFVec3(mtxBox.m_af[0][0], mtxBox.m_af[0][1], mtxBox.m_af[0][2])))
		+ vHalfSize.m_fY * fabsf(CKFMath::Vec3Dot(vAxis, CKFVec3(mtxBox.m_af[1][0], mtxBox.m_af[1][1], mtxBox.m_af[1][2])))
		+ vHalfSize.m_fZ * fabsf(CKFMath::Vec3Dot(vAxis, CKFVec3(mtxBox.m_af[2][0], mtxBox.m_af[2][1], mtxBox.m_af[2][2])));
	return fAnswer;
}

//--------------------------------------------------------------------------------
//	�֐����FcheckOverlapOnAxis
//  �֐������F�{�b�N�X���m���^����ꂽ���ɏd�˂Ă邩�ǂ������`�F�b�N����
//	�����F	boxL�F�{�b�N�X�R���C�_�[
//			boxR�F�{�b�N�X�R���C�_�[
//			vAxis�F�����
//	�߂�l�Fbool
//--------------------------------------------------------------------------------
bool CCollisionDetector::checkOverlapOnAxis(const CBoxColliderComponent& boxL, const CBoxColliderComponent& boxR, const CKFVec3& vAxis)
{
	//����̒������Z�o����
	float fHalfDisL = transformBoxToAxis(boxL, vAxis);
	float fHalfDisR = transformBoxToAxis(boxR, vAxis);

	//����̒��S�Ԃ̋������Z�o����
	CKFVec3 vLtoR = boxL.GetWorldPos() - boxR.GetWorldPos();
	float fDis = fabsf(CKFMath::Vec3Dot(vLtoR, vAxis));

	//�d�˂Ă邩�ǂ������`�F�b�N����
	bool bAns = fDis < (fHalfDisL + fHalfDisR);
	return bAns;
}

//--------------------------------------------------------------------------------
//	�֐����FcheckOverlapOnAxis
//  �֐������FAABB���m���^����ꂽ���ɏd�˂Ă邩�ǂ������`�F�b�N����(�񎟌�)
//	�����F	vMinL�FAABB��ԍ���̓_���
//			vMaxL�FAABB��ԉE���̓_���
//			vMinR�FAABB��ԍ���̓_���
//			vMaxR�FAABB��ԉE���̓_���
//	�߂�l�Fbool
//--------------------------------------------------------------------------------
bool CCollisionDetector::checkOverlapOnAxis(const CKFVec2& vMinL, const CKFVec2& vMaxL, const CKFVec2& vMinR, const CKFVec2& vMaxR)
{
	bool bAnswer = vMinL.m_fX < vMaxR.m_fX && vMinR.m_fX < vMaxL.m_fX
		&& vMinL.m_fY < vMaxR.m_fY && vMinR.m_fY < vMaxL.m_fY;
	return bAnswer;
}

//--------------------------------------------------------------------------------
//	�֐����FcheckOverlapOnAxis
//  �֐������FAABB���m��XYZ���ɏd�˂Ă邩�ǂ������`�F�b�N����(�O����)
//	�����F	aabbL�FAABB�R���C�_�[
//			aabbR�FAABB�R���C�_�[
//	�߂�l�Fbool
//--------------------------------------------------------------------------------
bool CCollisionDetector::checkOverlapAABB(const CAABBColliderComponent& aabbL, const CAABBColliderComponent& aabbR)
{
	const auto& vPosL = aabbL.GetWorldPos();
	const auto& vHalfSizeL = aabbL.GetHalfSize();
	const auto& vPosR = aabbR.GetWorldPos();
	const auto& vHalfSizeR = aabbR.GetHalfSize();
	auto& vMinL = vPosL - vHalfSizeL;
	auto& vMaxL = vPosL + vHalfSizeL;
	auto& vMinR = vPosR - vHalfSizeR;
	auto& vMaxR = vPosR + vHalfSizeR;

	//AABB���m��xyz����ɏd�˂Ă邩�ǂ������`�F�b�N����
	bool bAnswer = checkOverlapOnAxis(CKFVec2(vMinL.m_fY, vMinL.m_fZ), CKFVec2(vMaxL.m_fY, vMaxL.m_fZ), CKFVec2(vMinR.m_fY, vMinR.m_fZ), CKFVec2(vMaxR.m_fY, vMaxR.m_fZ))	//X��
		&& checkOverlapOnAxis(CKFVec2(vMinL.m_fZ, vMinL.m_fX), CKFVec2(vMaxL.m_fZ, vMaxL.m_fX), CKFVec2(vMinR.m_fZ, vMinR.m_fX), CKFVec2(vMaxR.m_fZ, vMaxR.m_fX))			//Y��
		&& checkOverlapOnAxis(CKFVec2(vMinL.m_fX, vMinL.m_fY), CKFVec2(vMaxL.m_fX, vMaxL.m_fY), CKFVec2(vMinR.m_fX, vMinR.m_fY), CKFVec2(vMaxR.m_fX, vMaxR.m_fY));			//Z��

	return bAnswer;
}