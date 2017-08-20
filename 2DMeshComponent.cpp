//--------------------------------------------------------------------------------
//	���b�V���R���|�l���g
//�@meshComponent.cpp
//	Author : Xu Wenjie
//	Date   : 2017-05-21
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "2DmeshComponent.h"
#include "gameObject.h"

#ifdef USING_DIRECTX
#include "manager.h"
#include "rendererDX.h"
#endif

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
bool C2DMeshComponent::Init(void)
{
#ifdef USING_DIRECTX
	LPDIRECT3DDEVICE9 pDevice = CMain::GetManager()->GetRenderer()->GetDevice();
	HRESULT hr;

	//���_�o�b�t�@
	hr = pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * s_nNumVtx,		//�쐬���������_�o�b�t�@�̃T�C�Y
		D3DUSAGE_WRITEONLY,					//���_�o�b�t�@�̎g�p���@
		FVF_VERTEX_2D,						//�����Ȃ��Ă����v
		D3DPOOL_MANAGED,					//�������Ǘ����@(managed�F�f�o�C�X�ɂ��C��)
		&m_pVtxBuffer,						//���_�o�b�t�@�̃A�h���X
		NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "C2DMeshComponent : CreateVertexBuffer ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
		return false;
	}
#endif
	return true;
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void C2DMeshComponent::Uninit(void)
{
#ifdef USING_DIRECTX
	if (m_pVtxBuffer != NULL)
	{
		m_pVtxBuffer->Release();
		m_pVtxBuffer = NULL;
	}
#endif
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void C2DMeshComponent::Update(void)
{
#ifdef USING_DIRECTX
	// ���_����ݒ�
	//���z�A�h���X���擾���邽�߂̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	//���_�ʒu�ݒ�
	CKFVec2 vSize = m_pGameObj->GetTransformComponent()->GetScale();
	CKFVec3 vPos = m_pGameObj->GetTransformComponent()->GetPos();
	CKFVec3 vRot = CKFVec3(0.0f);
	float fLength = CKFMath::VecMagnitude(vSize) * 0.5f;
	float fAngle = atan2f(vSize.m_fY, vSize.m_fX);

	//���_���W�̐ݒ�i2D���W�A�E���j
	pVtx[0].vPos = CKFVec3(vPos.m_fX + cosf(vRot.m_fZ + KF_PI + fAngle) * fLength, vPos.m_fY + sinf(vRot.m_fZ + KF_PI + fAngle) * fLength, 0.0f);
	pVtx[1].vPos = CKFVec3(vPos.m_fX + cosf(vRot.m_fZ - fAngle) * fLength, vPos.m_fY + sinf(vRot.m_fZ - fAngle) * fLength, 0.0f);
	pVtx[2].vPos = CKFVec3(vPos.m_fX + cosf(vRot.m_fZ - KF_PI - fAngle) * fLength, vPos.m_fY + sinf(vRot.m_fZ - KF_PI - fAngle) * fLength, 0.0f);
	pVtx[3].vPos = CKFVec3(vPos.m_fX + cosf(vRot.m_fZ + fAngle) * fLength, vPos.m_fY + sinf(vRot.m_fZ + fAngle) * fLength, 0.0f);

	//���_�J���[�ݒ�
	pVtx[0].ulColor = m_cColor;
	pVtx[1].ulColor = m_cColor;
	pVtx[2].ulColor = m_cColor;
	pVtx[3].ulColor = m_cColor;

	//���_UV�ݒ�
	pVtx[0].vUV = CKFVec2(0.0f, 0.0f);
	pVtx[1].vUV = CKFVec2(1.0f, 0.0f);
	pVtx[2].vUV = CKFVec2(0.0f, 1.0f);
	pVtx[3].vUV = CKFVec2(1.0f, 1.0f);

	//rhw�̐ݒ�(�K��1.0f)
	pVtx[0].fRhw = 1.0f;
	pVtx[1].fRhw = 1.0f;
	pVtx[2].fRhw = 1.0f;
	pVtx[3].fRhw = 1.0f;

	//���z�A�h���X���
	m_pVtxBuffer->Unlock();
#endif
}