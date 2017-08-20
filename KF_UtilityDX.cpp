//--------------------------------------------------------------------------------
//	DX�p�֗��֐�
//�@KF_UtilityDX.cpp
//	Author : Xu Wenjie
//	Date   : 2016-07-24
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "KF_UtilityDX.h"
#include "manager.h"

#ifdef USING_DIRECTX
#include "rendererDX.h"

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  ���_����
//--------------------------------------------------------------------------------
bool CKFUtilityDX::MakeVertex(LPDIRECT3DVERTEXBUFFER9& pVtxBuffer, const CKFVec2& vPosCenter, const CKFVec2& vSize, const CKFVec2& vUVBegin, const CKFVec2& vUVSize, const float& fRot, const CKFColor& cColor)
{
	LPDIRECT3DDEVICE9 pDevice = CMain::GetManager()->GetRenderer()->GetDevice();
	HRESULT hr;

	//���_�o�b�t�@
	hr = pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,		//�쐬���������_�o�b�t�@�̃T�C�Y
		D3DUSAGE_WRITEONLY,			//���_�o�b�t�@�̎g�p���@
		FVF_VERTEX_2D,				//�����Ȃ��Ă����v
		D3DPOOL_MANAGED,			//�������Ǘ����@(managed�F�f�o�C�X�ɂ��C��)
		&pVtxBuffer,				//���_�o�b�t�@�̃A�h���X
		NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "CKFUtilityDX : CreateVertexBuffer ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
		return false;
	}

	// ���_����ݒ�
	//���z�A�h���X���擾���邽�߂̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	//���_�ʒu�ݒ�
	float fLength = CKFMath::VecMagnitude(vSize) * 0.5f;
	float fAngle = atan2f(vSize.m_fY, vSize.m_fX);

	//���_���W�̐ݒ�i2D���W�A�E���j
	pVtx[0].vPos = CKFVec3(vPosCenter.m_fX + cosf(fRot + KF_PI + fAngle) * fLength,
		vPosCenter.m_fY + sinf(fRot + KF_PI + fAngle) * fLength, 0.0f);
	pVtx[1].vPos = CKFVec3(vPosCenter.m_fX + cosf(fRot - fAngle) * fLength,
		vPosCenter.m_fY + sinf(fRot - fAngle) * fLength, 0.0f);
	pVtx[2].vPos = CKFVec3(vPosCenter.m_fX + cosf(fRot - KF_PI - fAngle) * fLength,
		vPosCenter.m_fY + sinf(fRot - KF_PI - fAngle) * fLength, 0.0f);
	pVtx[3].vPos = CKFVec3(vPosCenter.m_fX + cosf(fRot + fAngle) * fLength,
		vPosCenter.m_fY + sinf(fRot + fAngle) * fLength, 0.0f);

	//���_�J���[�ݒ�
	pVtx[0].ulColor = cColor;
	pVtx[1].ulColor = cColor;
	pVtx[2].ulColor = cColor;
	pVtx[3].ulColor = cColor;

	//���_UV�ݒ�
	pVtx[0].vUV = vUVBegin;
	pVtx[1].vUV = vUVBegin + CKFVec2(vUVSize.m_fX, 0.0f);
	pVtx[2].vUV = vUVBegin + CKFVec2(0.0f, vUVSize.m_fY);
	pVtx[3].vUV = vUVBegin + vUVSize;

	//rhw�̐ݒ�(�K��1.0f)
	pVtx[0].fRhw = 1.0f;
	pVtx[1].fRhw = 1.0f;
	pVtx[2].fRhw = 1.0f;
	pVtx[3].fRhw = 1.0f;

	//���z�A�h���X���
	pVtxBuffer->Unlock();

	return true;
}

//--------------------------------------------------------------------------------
//  ���_����(�Q�[�W)
//--------------------------------------------------------------------------------
bool CKFUtilityDX::MakeVertexGauge(LPDIRECT3DVERTEXBUFFER9& pVtxBuffer, const CKFVec2& vPosLeftTop, const CKFVec2& vSize, const CKFColor& cColor, const CKFVec2& vUVBegin, const CKFVec2& vUVSize)
{
	LPDIRECT3DDEVICE9 pDevice = CMain::GetManager()->GetRenderer()->GetDevice();
	HRESULT hr;

	//���_�o�b�t�@
	hr = pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,		//�쐬���������_�o�b�t�@�̃T�C�Y
		D3DUSAGE_WRITEONLY,			//���_�o�b�t�@�̎g�p���@
		FVF_VERTEX_2D,				//�����Ȃ��Ă����v
		D3DPOOL_MANAGED,			//�������Ǘ����@(managed�F�f�o�C�X�ɂ��C��)
		&pVtxBuffer,				//���_�o�b�t�@�̃A�h���X
		NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "CKFUtilityDX : CreateVertexBuffer ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
		return false;
	}

	// ���_����ݒ�
	//���z�A�h���X���擾���邽�߂̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�i2D���W�A�E���j
	pVtx[0].vPos = CKFVec3(vPosLeftTop.m_fX, vPosLeftTop.m_fY, 0.0f);
	pVtx[1].vPos = CKFVec3(vPosLeftTop.m_fX, vPosLeftTop.m_fY, 0.0f) + CKFVec3(vSize.m_fX, 0.0f, 0.0f);
	pVtx[2].vPos = CKFVec3(vPosLeftTop.m_fX, vPosLeftTop.m_fY, 0.0f) + CKFVec3(0.0f, vSize.m_fY, 0.0f);
	pVtx[3].vPos = CKFVec3(vPosLeftTop.m_fX, vPosLeftTop.m_fY, 0.0f) + CKFVec3(vSize.m_fX, vSize.m_fY, 0.0f);

	//���_�J���[�ݒ�
	pVtx[0].ulColor = cColor;
	pVtx[1].ulColor = cColor;
	pVtx[2].ulColor = cColor;
	pVtx[3].ulColor = cColor;

	//���_UV�ݒ�
	pVtx[0].vUV = vUVBegin;
	pVtx[1].vUV = vUVBegin + CKFVec2(vUVSize.m_fX, 0.0f);
	pVtx[2].vUV = vUVBegin + CKFVec2(0.0f, vUVSize.m_fY);
	pVtx[3].vUV = vUVBegin + vUVSize;

	//rhw�̐ݒ�(�K��1.0f)
	pVtx[0].fRhw = 1.0f;
	pVtx[1].fRhw = 1.0f;
	pVtx[2].fRhw = 1.0f;
	pVtx[3].fRhw = 1.0f;

	//���z�A�h���X���
	pVtxBuffer->Unlock();

	return true;
}

//--------------------------------------------------------------------------------
//  ���_�X�V(�Q�[�W)
//--------------------------------------------------------------------------------
void CKFUtilityDX::UpdateVertexGauge(LPDIRECT3DVERTEXBUFFER9& pVtxBuffer, const CKFVec2& vPosLeftTop, const CKFVec2& vSize, const float& fRate, const CKFColor& cColor, const CKFVec2& vUVBegin, const CKFVec2& vUVSize)
{
	// ���_����ݒ�
	//���z�A�h���X���擾���邽�߂̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�i2D���W�A�E���j
	pVtx[0].vPos = CKFVec3(vPosLeftTop.m_fX, vPosLeftTop.m_fY, 0.0f);
	pVtx[1].vPos = CKFVec3(vPosLeftTop.m_fX, vPosLeftTop.m_fY, 0.0f) + CKFVec3(vSize.m_fX * fRate, 0.0f, 0.0f);
	pVtx[2].vPos = CKFVec3(vPosLeftTop.m_fX, vPosLeftTop.m_fY, 0.0f) + CKFVec3(0.0f, vSize.m_fY, 0.0f);
	pVtx[3].vPos = CKFVec3(vPosLeftTop.m_fX, vPosLeftTop.m_fY, 0.0f) + CKFVec3(vSize.m_fX * fRate, vSize.m_fY, 0.0f);

	//���_�J���[�ݒ�
	pVtx[0].ulColor = cColor;
	pVtx[1].ulColor = cColor;
	pVtx[2].ulColor = cColor;
	pVtx[3].ulColor = cColor;

	//���_UV�ݒ�
	pVtx[0].vUV = vUVBegin;
	pVtx[1].vUV = vUVBegin + CKFVec2(vUVSize.m_fX * fRate, 0.0f);
	pVtx[2].vUV = vUVBegin + CKFVec2(0.0f, vUVSize.m_fY);
	pVtx[3].vUV = vUVBegin + CKFVec2(vUVSize.m_fX * fRate, vUVSize.m_fY);

	//���z�A�h���X���
	pVtxBuffer->Unlock();
}

//--------------------------------------------------------------------------------
//  ���_�X�V(UV)
//--------------------------------------------------------------------------------
void CKFUtilityDX::UpdateUV(LPDIRECT3DVERTEXBUFFER9& pVtxBuffer, const CKFVec2& vUVBegin, const CKFVec2& vUVSize)
{
	// ���_����ݒ�
	//���z�A�h���X���擾���邽�߂̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	//���_UV�ݒ�
	pVtx[0].vUV = vUVBegin;
	pVtx[1].vUV = vUVBegin + CKFVec2(vUVSize.m_fX, 0.0f);
	pVtx[2].vUV = vUVBegin + CKFVec2(0.0f, vUVSize.m_fY);
	pVtx[3].vUV = vUVBegin + vUVSize;

	//���z�A�h���X���
	pVtxBuffer->Unlock();
}
#endif