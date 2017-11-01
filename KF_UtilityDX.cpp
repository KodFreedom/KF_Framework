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
bool UtilityDX::MakeVertex(LPDIRECT3DVERTEXBUFFER9& pVtxBuffer, const Vector2& PositionCenter, const Vector2& vSize, const Vector2& UVBegin, const Vector2& UVSize, const float& fRot, const Color& cColor)
{
	auto pDevice = Main::GetManager()->GetRenderer()->GetDevice();
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
		MessageBox(NULL, "UtilityDX : CreateVertexBuffer ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
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
	pVtx[0].Position = Vector3(PositionCenter.X + cosf(fRot + KF_PI + fAngle) * fLength,
		PositionCenter.Y + sinf(fRot + KF_PI + fAngle) * fLength, 0.0f);
	pVtx[1].Position = Vector3(PositionCenter.X + cosf(fRot - fAngle) * fLength,
		PositionCenter.Y + sinf(fRot - fAngle) * fLength, 0.0f);
	pVtx[2].Position = Vector3(PositionCenter.X + cosf(fRot - KF_PI - fAngle) * fLength,
		PositionCenter.Y + sinf(fRot - KF_PI - fAngle) * fLength, 0.0f);
	pVtx[3].Position = Vector3(PositionCenter.X + cosf(fRot + fAngle) * fLength,
		PositionCenter.Y + sinf(fRot + fAngle) * fLength, 0.0f);

	//���_�J���[�ݒ�
	pVtx[0].Color = cColor;
	pVtx[1].Color = cColor;
	pVtx[2].Color = cColor;
	pVtx[3].Color = cColor;

	//���_UV�ݒ�
	pVtx[0].UV = UVBegin;
	pVtx[1].UV = UVBegin + Vector2(UVSize.X, 0.0f);
	pVtx[2].UV = UVBegin + Vector2(0.0f, UVSize.Y);
	pVtx[3].UV = UVBegin + UVSize;

	//rhw�̐ݒ�(�K��1.0f)
	pVtx[0].Rhw = 1.0f;
	pVtx[1].Rhw = 1.0f;
	pVtx[2].Rhw = 1.0f;
	pVtx[3].Rhw = 1.0f;

	//���z�A�h���X���
	pVtxBuffer->Unlock();

	return true;
}

//--------------------------------------------------------------------------------
//  ���_����(�Q�[�W)
//--------------------------------------------------------------------------------
bool UtilityDX::MakeVertexGauge(LPDIRECT3DVERTEXBUFFER9& pVtxBuffer, const Vector2& PositionLeftTop, const Vector2& vSize, const Color& cColor, const Vector2& UVBegin, const Vector2& UVSize)
{
	auto pDevice = Main::GetManager()->GetRenderer()->GetDevice();
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
		MessageBox(NULL, "UtilityDX : CreateVertexBuffer ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
		return false;
	}

	// ���_����ݒ�
	//���z�A�h���X���擾���邽�߂̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�i2D���W�A�E���j
	pVtx[0].Position = Vector3(PositionLeftTop.X, PositionLeftTop.Y, 0.0f);
	pVtx[1].Position = Vector3(PositionLeftTop.X, PositionLeftTop.Y, 0.0f) + Vector3(vSize.X, 0.0f, 0.0f);
	pVtx[2].Position = Vector3(PositionLeftTop.X, PositionLeftTop.Y, 0.0f) + Vector3(0.0f, vSize.Y, 0.0f);
	pVtx[3].Position = Vector3(PositionLeftTop.X, PositionLeftTop.Y, 0.0f) + Vector3(vSize.X, vSize.Y, 0.0f);

	//���_�J���[�ݒ�
	pVtx[0].Color = cColor;
	pVtx[1].Color = cColor;
	pVtx[2].Color = cColor;
	pVtx[3].Color = cColor;

	//���_UV�ݒ�
	pVtx[0].UV = UVBegin;
	pVtx[1].UV = UVBegin + Vector2(UVSize.X, 0.0f);
	pVtx[2].UV = UVBegin + Vector2(0.0f, UVSize.Y);
	pVtx[3].UV = UVBegin + UVSize;

	//rhw�̐ݒ�(�K��1.0f)
	pVtx[0].Rhw = 1.0f;
	pVtx[1].Rhw = 1.0f;
	pVtx[2].Rhw = 1.0f;
	pVtx[3].Rhw = 1.0f;

	//���z�A�h���X���
	pVtxBuffer->Unlock();

	return true;
}

//--------------------------------------------------------------------------------
//  ���_�X�V(�Q�[�W)
//--------------------------------------------------------------------------------
void UtilityDX::UpdateVertexGauge(LPDIRECT3DVERTEXBUFFER9& pVtxBuffer, const Vector2& PositionLeftTop, const Vector2& vSize, const float& fRate, const Color& cColor, const Vector2& UVBegin, const Vector2& UVSize)
{
	// ���_����ݒ�
	//���z�A�h���X���擾���邽�߂̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�i2D���W�A�E���j
	pVtx[0].Position = Vector3(PositionLeftTop.X, PositionLeftTop.Y, 0.0f);
	pVtx[1].Position = Vector3(PositionLeftTop.X, PositionLeftTop.Y, 0.0f) + Vector3(vSize.X * fRate, 0.0f, 0.0f);
	pVtx[2].Position = Vector3(PositionLeftTop.X, PositionLeftTop.Y, 0.0f) + Vector3(0.0f, vSize.Y, 0.0f);
	pVtx[3].Position = Vector3(PositionLeftTop.X, PositionLeftTop.Y, 0.0f) + Vector3(vSize.X * fRate, vSize.Y, 0.0f);

	//���_�J���[�ݒ�
	pVtx[0].Color = cColor;
	pVtx[1].Color = cColor;
	pVtx[2].Color = cColor;
	pVtx[3].Color = cColor;

	//���_UV�ݒ�
	pVtx[0].UV = UVBegin;
	pVtx[1].UV = UVBegin + Vector2(UVSize.X * fRate, 0.0f);
	pVtx[2].UV = UVBegin + Vector2(0.0f, UVSize.Y);
	pVtx[3].UV = UVBegin + Vector2(UVSize.X * fRate, UVSize.Y);

	//���z�A�h���X���
	pVtxBuffer->Unlock();
}

//--------------------------------------------------------------------------------
//  ���_�X�V(UV)
//--------------------------------------------------------------------------------
void UtilityDX::UpdateUV(LPDIRECT3DVERTEXBUFFER9& pVtxBuffer, const Vector2& UVBegin, const Vector2& UVSize)
{
	// ���_����ݒ�
	//���z�A�h���X���擾���邽�߂̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	//���_UV�ݒ�
	pVtx[0].UV = UVBegin;
	pVtx[1].UV = UVBegin + Vector2(UVSize.X, 0.0f);
	pVtx[2].UV = UVBegin + Vector2(0.0f, UVSize.Y);
	pVtx[3].UV = UVBegin + UVSize;

	//���z�A�h���X���
	pVtxBuffer->Unlock();
}
#endif