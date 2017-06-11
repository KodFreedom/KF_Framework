//--------------------------------------------------------------------------------
//	�Q�[���I�u�W�F�N�g2D
//�@gameObject2D.cpp
//	Author : Xu Wenjie
//	Date   : 2017-04-26
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "gameObject2D.h"
#include "2DMeshComponent.h"
#include "2DDrawComponent.h"

//--------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �I�u�W�F�N�g�̐���
//--------------------------------------------------------------------------------
CGameObject2D* CGameObject2D::Create(const CKFVec3& vPos, const CKFVec3& vRot, const CKFVec3& vSize, const CTM::TEX_NAME& texName)
{
	//����
	CGameObject2D* pObj = new CGameObject2D;

	//�R���|�l���g
	C2DMeshComponent* pMesh = new C2DMeshComponent(pObj);
	pMesh->SetTexName(CTM::TEX_TEST);
	pObj->m_pMesh = pMesh;
	pObj->m_pDraw = new C2DDrawComponent(pObj);

	//�p�����[�^�[
	pObj->m_vPos = pObj->m_vPosNext = vPos;
	pObj->m_vScale = pObj->m_vScaleNext = vSize;
	pObj->RotByEuler(vRot);

	//������
	pObj->Init();

	return pObj;
}