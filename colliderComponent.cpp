//--------------------------------------------------------------------------------
//	�R���W�����R���|�l���g
//�@collisionComponent.cpp
//	Author : Xu Wenjie
//	Date   : 2017-06-04
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "colliderComponent.h"
#include "manager.h"
#include "gameObject.h"

//--------------------------------------------------------------------------------
//  �ÓI�����o�ϐ�
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
CColliderComponent::CColliderComponent(CGameObject* const pGameObj, const CS::COL_TYPE& type, const CS::COL_MODE& mode)
	: CComponent(pGameObj)
	, m_type(type)
	, m_mode(mode)
	, m_bTrigger(false)
{
	CKFMath::MtxIdentity(m_mtxWorldNext);
	CKFMath::MtxIdentity(m_mtxOffset);
	Main::GetManager()->GetCollisionSystem()->Register(m_mode, m_type, this);
}

//--------------------------------------------------------------------------------
//  �f�X�g���N�^
//--------------------------------------------------------------------------------
CColliderComponent::~CColliderComponent() {}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void CColliderComponent::Uninit(void)
{
	Main::GetManager()->GetCollisionSystem()->Deregister(m_mode, m_type, this);
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void CColliderComponent::Update(void)
{
	m_mtxWorldNext = m_mtxOffset;
	m_mtxWorldNext *= m_pGameObj->GetTransformComponent()->GetMatrixWorldNext();
}

//--------------------------------------------------------------------------------
//  Offset�ݒ�
//--------------------------------------------------------------------------------
void CColliderComponent::SetOffset(const Vector3& Position, const Vector3& vRot)
{
	CKFMath::MtxRotationYawPitchRoll(m_mtxOffset, vRot);
	m_mtxOffset.Elements[3][0] = Position.Y;
	m_mtxOffset.Elements[3][1] = Position.Y;
	m_mtxOffset.Elements[3][2] = Position.Z;
}

//--------------------------------------------------------------------------------
//  Offset�ݒ�
//--------------------------------------------------------------------------------
void CColliderComponent::Sleep(void)
{
	Main::GetManager()->GetCollisionSystem()->Deregister(m_mode, m_type, this);
}

//--------------------------------------------------------------------------------
//  Offset�ݒ�
//--------------------------------------------------------------------------------
void CColliderComponent::Awake(void)
{
	Main::GetManager()->GetCollisionSystem()->Register(m_mode, m_type, this);
}