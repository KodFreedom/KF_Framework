//--------------------------------------------------------------------------------
//	�g�����X�t�H�[���R���|�l���g
//�@transformComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-07-05
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "transformComponent.h"

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//
//  public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
CTransformComponent::CTransformComponent(CGameObject* const pGameObj) : CComponent(pGameObj)
	, m_vPos(CKFMath::sc_vZero)
	, m_vPosNext(CKFMath::sc_vZero)
	, m_vScale(CKFMath::sc_vZero)
	, m_vScaleNext(CKFMath::sc_vZero)
	, m_vForward(CKFMath::sc_vForward)
	, m_vUp(CKFMath::sc_vUp)
	, m_vRight(CKFMath::sc_vRight)
	, m_vForwardNext(CKFMath::sc_vForward)
	, m_vUpNext(CKFMath::sc_vUp)
	, m_vRightNext(CKFMath::sc_vRight)
	, m_pParent(nullptr)
{
	m_listChildren.clear();
	CKFMath::MtxIdentity(m_mtxThis);
}

//--------------------------------------------------------------------------------
//	�֐����FUpdateMatrix
//  �֐������F�}�g���N�X�Z�o(�e���Ȃ��̏ꍇ�Ăяo�����)
//	�����F	�Ȃ�
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void  CTransformComponent::UpdateMatrix(void)
{
	//�e������ꍇ�v�Z���Ȃ�
	if (m_pParent) { return; }

	calculateMtxThis();

	//�q�������̍s����Z�o
	for (auto pChild : m_listChildren)
	{
		pChild->UpdateMatrix(m_mtxThis);
	}
}

//--------------------------------------------------------------------------------
//	�֐����FUpdateMatrix
//  �֐������F�}�g���N�X�Z�o(�e������̏ꍇ�Ăяo�����)
//	�����F	mtxParent
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void  CTransformComponent::UpdateMatrix(const CKFMtx44& mtxParent)
{
	calculateMtxThis(mtxParent);

	//�q�������̍s����Z�o
	for (auto pChild : m_listChildren)
	{
		pChild->UpdateMatrix(m_mtxThis);
	}
}

//--------------------------------------------------------------------------------
//	�֐����FUpdateMatrix
//  �֐������F�p�����[�^�[��������
//	�����F	�Ȃ�
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void CTransformComponent::SwapParam(void)
{
	m_vPos = m_vPosNext;
	m_vScale = m_vScaleNext;
	m_vForward = m_vForwardNext;
	m_vRight = m_vRightNext;
	m_vUp = m_vUpNext;
}

//--------------------------------------------------------------------------------
//	�֐����FRegisterChild
//  �֐������FChild���X�g�ɒǉ�
//	�����F	pChild
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void CTransformComponent::RegisterChild(CTransformComponent* pChild)
{
	m_listChildren.push_back(pChild);
}

//--------------------------------------------------------------------------------
//	�֐����FRegisterChild
//  �֐������FChild���X�g����폜
//	�����F	pChild
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void CTransformComponent::DeregisterChild(CTransformComponent* pChild)
{
	m_listChildren.remove(pChild);
}

//--------------------------------------------------------------------------------
//	�֐����FRegisterParent
//  �֐������F�e�̒ǉ�
//	�����F	pParent
//			vOffsetPos
//			vOffsetRot
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void CTransformComponent::RegisterParent(CTransformComponent* pParent, const CKFVec3& vOffsetPos, const CKFVec3& vOffsetRot)
{
	if (m_pParent)
	{//�e������̏ꍇ�O�̐e����폜
		m_pParent->DeregisterChild(this);
	}
	m_pParent = pParent;
	m_pParent->RegisterChild(this);
	m_vOffsetPos = vOffsetPos;
	m_vOffsetRot = vOffsetRot;
}

//--------------------------------------------------------------------------------
//	�֐����FGetRot
//  �֐������F���̃t���[���̉�]�̎擾
//	�����F	�Ȃ�
//	�߂�l�FCKFQuaternion
//--------------------------------------------------------------------------------
const CKFQuaternion CTransformComponent::GetRot(void) const
{
	return CKFMath::MtxToQuaternion(GetMatrixRot());
}

//--------------------------------------------------------------------------------
//	�֐����FGetRotNext
//  �֐������F���̃t���[���̉�]�̎擾
//	�����F	�Ȃ�
//	�߂�l�FCKFQuaternion
//--------------------------------------------------------------------------------
const CKFQuaternion CTransformComponent::GetRotNext(void) const
{
	return CKFMath::MtxToQuaternion(GetMatrixRotNext());
}

//--------------------------------------------------------------------------------
//	�֐����FGetMatrixRot
//  �֐������F���̃t���[���̉�]�s��̎擾
//	�����F	�Ȃ�
//	�߂�l�FCKFMtx44
//--------------------------------------------------------------------------------
const CKFMtx44 CTransformComponent::GetMatrixRot(void) const
{
	CKFMtx44 mtxRot;
	mtxRot.m_af[0][0] = m_vRight.m_fX;
	mtxRot.m_af[0][1] = m_vRight.m_fY;
	mtxRot.m_af[0][2] = m_vRight.m_fZ;
	mtxRot.m_af[1][0] = m_vUp.m_fX;
	mtxRot.m_af[1][1] = m_vUp.m_fY;
	mtxRot.m_af[1][2] = m_vUp.m_fZ;
	mtxRot.m_af[2][0] = m_vForward.m_fX;
	mtxRot.m_af[2][1] = m_vForward.m_fY;
	mtxRot.m_af[2][2] = m_vForward.m_fZ;
	return mtxRot;
}

//--------------------------------------------------------------------------------
//	�֐����FGetMatrixRotNext
//  �֐������F���̃t���[���̉�]�s��̎擾
//	�����F	�Ȃ�
//	�߂�l�FCKFMtx44
//--------------------------------------------------------------------------------
const CKFMtx44 CTransformComponent::GetMatrixRotNext(void) const
{
	CKFMtx44 mtxRot;
	mtxRot.m_af[0][0] = m_vRightNext.m_fX;
	mtxRot.m_af[0][1] = m_vRightNext.m_fY;
	mtxRot.m_af[0][2] = m_vRightNext.m_fZ;
	mtxRot.m_af[1][0] = m_vUpNext.m_fX;
	mtxRot.m_af[1][1] = m_vUpNext.m_fY;
	mtxRot.m_af[1][2] = m_vUpNext.m_fZ;
	mtxRot.m_af[2][0] = m_vForwardNext.m_fX;
	mtxRot.m_af[2][1] = m_vForwardNext.m_fY;
	mtxRot.m_af[2][2] = m_vForwardNext.m_fZ;
	return mtxRot;
}

//--------------------------------------------------------------------------------
//	�֐����FGetMatrixWorldNext
//  �֐������F���̃t���[���̐��E�s��̎擾
//	�����F	�Ȃ�
//	�߂�l�FCKFMtx44
//--------------------------------------------------------------------------------
const CKFMtx44 CTransformComponent::GetMatrixWorldNext(void) const
{
	//��]
	auto mtxWorld = GetMatrixRotNext();

	//���s�ړ�
	mtxWorld.m_af[3][0] = m_vPosNext.m_fX;
	mtxWorld.m_af[3][1] = m_vPosNext.m_fY;
	mtxWorld.m_af[3][2] = m_vPosNext.m_fZ;

	//�e�̃}�g���N�X�擾
	if (m_pParent)
	{
		//�e�ɑ΂��鑊�Έʒu�s��
		//��]
		CKFMtx44 mtxRot;
		CKFMath::MtxRotationYawPitchRoll(mtxRot, m_vOffsetRot);
		mtxWorld *= mtxRot;

		//���s�ړ�
		CKFMtx44 mtxPos;
		CKFMath::MtxTranslation(mtxPos, m_vOffsetPos);
		mtxWorld *= mtxPos;

		mtxWorld *= m_pParent->GetMatrixWorldNext();
	}

	return mtxWorld;
}

//--------------------------------------------------------------------------------
//	�֐����FSetRot
//  �֐������F���̉�]�̐ݒ�
//	�����F	qRot�F��]���
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void CTransformComponent::SetRot(const CKFQuaternion& qRot)
{
	auto mtxRot = CKFMath::QuaternionToMtx(qRot);
	m_vRight.m_fX = mtxRot.m_af[0][0];
	m_vRight.m_fY = mtxRot.m_af[0][1];
	m_vRight.m_fZ = mtxRot.m_af[0][2];
	m_vUp.m_fX = mtxRot.m_af[1][0];
	m_vUp.m_fY = mtxRot.m_af[1][1];
	m_vUp.m_fZ = mtxRot.m_af[1][2];
	m_vForward.m_fX = mtxRot.m_af[2][0];
	m_vForward.m_fY = mtxRot.m_af[2][1];
	m_vForward.m_fZ = mtxRot.m_af[2][2];
}

//--------------------------------------------------------------------------------
//	�֐����FSetRotNext
//  �֐������F���̉�]�̐ݒ�
//	�����F	qRotNext�F��]���
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void CTransformComponent::SetRotNext(const CKFQuaternion& qRotNext)
{
	auto mtxRot = CKFMath::QuaternionToMtx(qRotNext);
	m_vRightNext.m_fX = mtxRot.m_af[0][0];
	m_vRightNext.m_fY = mtxRot.m_af[0][1];
	m_vRightNext.m_fZ = mtxRot.m_af[0][2];
	m_vUpNext.m_fX = mtxRot.m_af[1][0];
	m_vUpNext.m_fY = mtxRot.m_af[1][1];
	m_vUpNext.m_fZ = mtxRot.m_af[1][2];
	m_vForwardNext.m_fX = mtxRot.m_af[2][0];
	m_vForwardNext.m_fY = mtxRot.m_af[2][1];
	m_vForwardNext.m_fZ = mtxRot.m_af[2][2];
}

//--------------------------------------------------------------------------------
//	�֐����FRotByUp
//  �֐������F������ɂ���]
//	�����F	vUp�F�����
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void CTransformComponent::RotByUp(const CKFVec3& vUp)
{
	m_vUpNext = vUp;
	m_vRightNext = m_vUpNext * m_vForwardNext;
	m_vForwardNext = m_vRightNext * m_vUpNext;
	CKFMath::VecNormalize(m_vUpNext);
	CKFMath::VecNormalize(m_vForwardNext);
	CKFMath::VecNormalize(m_vRightNext);
}

//--------------------------------------------------------------------------------
//	�֐����FRotByForward
//  �֐������F�O�����ɂ���]
//	�����F	vForward�F�O����
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void CTransformComponent::RotByForward(const CKFVec3& vForward)
{
	m_vForwardNext = vForward;
	m_vUpNext = m_vForwardNext * m_vRightNext;
	m_vRightNext = m_vUpNext * m_vForwardNext;
	CKFMath::VecNormalize(m_vUpNext);
	CKFMath::VecNormalize(m_vForwardNext);
	CKFMath::VecNormalize(m_vRightNext);
}

//--------------------------------------------------------------------------------
//	�֐����FRotByRight
//  �֐������F�E�����ɂ���]
//	�����F	vRight�F�E����
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void CTransformComponent::RotByRight(const CKFVec3& vRight)
{
	m_vRightNext = vRight;
	m_vForwardNext = m_vRightNext * m_vUpNext;
	m_vUpNext = m_vForwardNext * m_vRightNext;
	CKFMath::VecNormalize(m_vUpNext);
	CKFMath::VecNormalize(m_vForwardNext);
	CKFMath::VecNormalize(m_vRightNext);
}

//--------------------------------------------------------------------------------
//	�֐����FRotByEuler
//  �֐������F�I�[���[�p����]
//	�����F	vRot�F�I�[���[�p
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void CTransformComponent::RotByEuler(const CKFVec3& vRot)
{
	CKFMtx44 mtxRot;
	CKFMath::MtxRotationYawPitchRoll(mtxRot, vRot);
	m_vUpNext = CKFMath::Vec3TransformNormal(m_vUpNext, mtxRot);
	m_vForwardNext = CKFMath::Vec3TransformNormal(m_vForwardNext, mtxRot);
	m_vRightNext = CKFMath::Vec3TransformNormal(m_vRightNext, mtxRot);
}

//--------------------------------------------------------------------------------
//	�֐����FRotByPitch
//  �֐������F������X��(vVecRight)����]
//	�����F	fRadian�F���W�A���p
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void CTransformComponent::RotByPitch(const float& fRadian)
{
	CKFMtx44 mtxPitch;
	CKFMath::MtxRotAxis(mtxPitch, m_vRightNext, fRadian);
	m_vUpNext = CKFMath::Vec3TransformNormal(m_vUpNext, mtxPitch);
	m_vForwardNext = CKFMath::Vec3TransformNormal(m_vForwardNext, mtxPitch);
}

//--------------------------------------------------------------------------------
//	�֐����FRotByYaw
//  �֐������F������Y��(vVecUp)����]
//	�����F	fRadian�F���W�A���p
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void CTransformComponent::RotByYaw(const float& fRadian)
{
	CKFMtx44 mtxYaw;
	CKFMath::MtxRotAxis(mtxYaw, m_vUpNext, fRadian);
	m_vRightNext = CKFMath::Vec3TransformNormal(m_vRightNext, mtxYaw);
	m_vForwardNext = CKFMath::Vec3TransformNormal(m_vForwardNext, mtxYaw);
}

//--------------------------------------------------------------------------------
//	�֐����FRotByRoll
//  �֐������F������Z��(vLook)����]
//	�����F	fRadian�F���W�A���p
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void CTransformComponent::RotByRoll(const float& fRadian)
{
	CKFMtx44 mtxRoll;
	CKFMath::MtxRotAxis(mtxRoll, m_vForwardNext, fRadian);
	m_vForwardNext = CKFMath::Vec3TransformNormal(m_vForwardNext, mtxRoll);
	m_vRightNext = CKFMath::Vec3TransformNormal(m_vRightNext, mtxRoll);
}

//--------------------------------------------------------------------------------
//	�֐����FTransformDirectionToLocal
//  �֐������F���E���̕����x�N�g���������̎��ɕϊ�����
//	�����F	vDirection�F�����x�N�g��
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
CKFVec3 CTransformComponent::TransformDirectionToLocal(const CKFVec3& vDirection)
{
	auto mtxInverse = CKFMath::MtxTranspose(m_mtxThis);
	mtxInverse.m_af[3][0] = 0.0f;
	mtxInverse.m_af[3][1] = 0.0f;
	mtxInverse.m_af[3][2] = 0.0f;
	auto vAnswer = CKFMath::Vec3TransformNormal(vDirection, mtxInverse);
	return vAnswer;
}

//--------------------------------------------------------------------------------
//
//  private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	�֐����FcalculateMtxThis
//  �֐������F�s��̎Z�o(�e�Ȃ�)
//	�����F	�Ȃ�
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void  CTransformComponent::calculateMtxThis(void)
{
	//�P�ʍs��ɏ�����
	CKFMath::MtxIdentity(m_mtxThis);

	//��]
	m_mtxThis *= GetMatrixRot();

	//���s�ړ�
	CKFMtx44 mtxPos;
	CKFMath::MtxTranslation(mtxPos, m_vPos);
	m_mtxThis *= mtxPos;
}

//--------------------------------------------------------------------------------
//	�֐����FcalculateMtxThis
//  �֐������F�s��̎Z�o(�e����)
//	�����F	mtxParent
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void  CTransformComponent::calculateMtxThis(const CKFMtx44& mtxParent)
{
	//�����̍s��Z�o
	calculateMtxThis();

	//�e�ɑ΂��鑊�Έʒu�s��
	//��]
	CKFMtx44 mtxRot;
	CKFMath::MtxRotationYawPitchRoll(mtxRot, m_vOffsetRot);
	m_mtxThis *= mtxRot;

	//���s�ړ�
	CKFMtx44 mtxPos;
	CKFMath::MtxTranslation(mtxPos, m_vOffsetPos);
	m_mtxThis *= mtxPos;

	//�e�̍s��Ƃ�����
	m_mtxThis *= mtxParent;
}