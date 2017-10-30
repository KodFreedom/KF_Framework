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
bool CKFUtilityDX::MakeVertex(LPDIRECT3DVERTEXBUFFER9& pVtxBuffer, const Vector2& vPosCenter, const Vector2& vSize, const Vector2& vUVBegin, const Vector2& vUVSize, const float& fRot, const Color& cColor)
{
	auto pDevice = CMain::GetManager()->GetRenderer()->GetDevice();
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
	float fAngle = atan2f(vSize.Y, vSize.X);

	//���_���W�̐ݒ�i2D���W�A�E���j
	pVtx[0].vPos = Vector3(vPosCenter.X + cosf(fRot + KF_PI + fAngle) * fLength,
		vPosCenter.Y + sinf(fRot + KF_PI + fAngle) * fLength, 0.0f);
	pVtx[1].vPos = Vector3(vPosCenter.X + cosf(fRot - fAngle) * fLength,
		vPosCenter.Y + sinf(fRot - fAngle) * fLength, 0.0f);
	pVtx[2].vPos = Vector3(vPosCenter.X + cosf(fRot - KF_PI - fAngle) * fLength,
		vPosCenter.Y + sinf(fRot - KF_PI - fAngle) * fLength, 0.0f);
	pVtx[3].vPos = Vector3(vPosCenter.X + cosf(fRot + fAngle) * fLength,
		vPosCenter.Y + sinf(fRot + fAngle) * fLength, 0.0f);

	//���_�J���[�ݒ�
	pVtx[0].ulColor = cColor;
	pVtx[1].ulColor = cColor;
	pVtx[2].ulColor = cColor;
	pVtx[3].ulColor = cColor;

	//���_UV�ݒ�
	pVtx[0].vUV = vUVBegin;
	pVtx[1].vUV = vUVBegin + Vector2(vUVSize.X, 0.0f);
	pVtx[2].vUV = vUVBegin + Vector2(0.0f, vUVSize.Y);
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
bool CKFUtilityDX::MakeVertexGauge(LPDIRECT3DVERTEXBUFFER9& pVtxBuffer, const Vector2& vPosLeftTop, const Vector2& vSize, const Color& cColor, const Vector2& vUVBegin, const Vector2& vUVSize)
{
	auto pDevice = CMain::GetManager()->GetRenderer()->GetDevice();
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
	pVtx[0].vPos = Vector3(vPosLeftTop.X, vPosLeftTop.Y, 0.0f);
	pVtx[1].vPos = Vector3(vPosLeftTop.X, vPosLeftTop.Y, 0.0f) + Vector3(vSize.X, 0.0f, 0.0f);
	pVtx[2].vPos = Vector3(vPosLeftTop.X, vPosLeftTop.Y, 0.0f) + Vector3(0.0f, vSize.Y, 0.0f);
	pVtx[3].vPos = Vector3(vPosLeftTop.X, vPosLeftTop.Y, 0.0f) + Vector3(vSize.X, vSize.Y, 0.0f);

	//���_�J���[�ݒ�
	pVtx[0].ulColor = cColor;
	pVtx[1].ulColor = cColor;
	pVtx[2].ulColor = cColor;
	pVtx[3].ulColor = cColor;

	//���_UV�ݒ�
	pVtx[0].vUV = vUVBegin;
	pVtx[1].vUV = vUVBegin + Vector2(vUVSize.X, 0.0f);
	pVtx[2].vUV = vUVBegin + Vector2(0.0f, vUVSize.Y);
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
void CKFUtilityDX::UpdateVertexGauge(LPDIRECT3DVERTEXBUFFER9& pVtxBuffer, const Vector2& vPosLeftTop, const Vector2& vSize, const float& fRate, const Color& cColor, const Vector2& vUVBegin, const Vector2& vUVSize)
{
	// ���_����ݒ�
	//���z�A�h���X���擾���邽�߂̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�i2D���W�A�E���j
	pVtx[0].vPos = Vector3(vPosLeftTop.X, vPosLeftTop.Y, 0.0f);
	pVtx[1].vPos = Vector3(vPosLeftTop.X, vPosLeftTop.Y, 0.0f) + Vector3(vSize.X * fRate, 0.0f, 0.0f);
	pVtx[2].vPos = Vector3(vPosLeftTop.X, vPosLeftTop.Y, 0.0f) + Vector3(0.0f, vSize.Y, 0.0f);
	pVtx[3].vPos = Vector3(vPosLeftTop.X, vPosLeftTop.Y, 0.0f) + Vector3(vSize.X * fRate, vSize.Y, 0.0f);

	//���_�J���[�ݒ�
	pVtx[0].ulColor = cColor;
	pVtx[1].ulColor = cColor;
	pVtx[2].ulColor = cColor;
	pVtx[3].ulColor = cColor;

	//���_UV�ݒ�
	pVtx[0].vUV = vUVBegin;
	pVtx[1].vUV = vUVBegin + Vector2(vUVSize.X * fRate, 0.0f);
	pVtx[2].vUV = vUVBegin + Vector2(0.0f, vUVSize.Y);
	pVtx[3].vUV = vUVBegin + Vector2(vUVSize.X * fRate, vUVSize.Y);

	//���z�A�h���X���
	pVtxBuffer->Unlock();
}

//--------------------------------------------------------------------------------
//  ���_�X�V(UV)
//--------------------------------------------------------------------------------
void CKFUtilityDX::UpdateUV(LPDIRECT3DVERTEXBUFFER9& pVtxBuffer, const Vector2& vUVBegin, const Vector2& vUVSize)
{
	// ���_����ݒ�
	//���z�A�h���X���擾���邽�߂̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	//���_UV�ݒ�
	pVtx[0].vUV = vUVBegin;
	pVtx[1].vUV = vUVBegin + Vector2(vUVSize.X, 0.0f);
	pVtx[2].vUV = vUVBegin + Vector2(0.0f, vUVSize.Y);
	pVtx[3].vUV = vUVBegin + vUVSize;

	//���z�A�h���X���
	pVtxBuffer->Unlock();
}
#endif