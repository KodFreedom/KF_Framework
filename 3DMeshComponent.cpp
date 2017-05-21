//--------------------------------------------------------------------------------
//	3D���b�V���R���|�l���g
//�@3DmeshComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-05-18
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "3DMeshComponent.h"
#include "gameObject.h"
#include "manager.h"

#ifdef USING_DIRECTX9
#include "rendererDX.h"
#endif

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void C3DMeshComponent::Uninit(void)
{
#ifdef USING_DIRECTX9
	if (m_meshInfo.pIdxBuffer != NULL)
	{
		m_meshInfo.pIdxBuffer->Release();
		m_meshInfo.pIdxBuffer = NULL;
	}

	if (m_meshInfo.pVtxBuffer != NULL)
	{
		m_meshInfo.pVtxBuffer->Release();
		m_meshInfo.pVtxBuffer = NULL;
	}
#endif
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void C3DMeshComponent::Update(CGameObject &gameObj)
{
	//�}�g���N�X�Z�o
	CKFMtx44 mtxWorld;
	CKFMtx44 mtxRot;
	CKFMtx44 mtxPos;

	//�P�ʍs��ɏ�����
	CKFMath::MtxIdentity(&mtxWorld);

	//��](Y->X->Z)
	CKFMath::MtxRotationYawPitchRoll(&mtxRot, gameObj.GetRot());
	mtxWorld *= mtxRot;

	//���s�ړ�
	CKFMath::MtxTranslation(&mtxPos, gameObj.GetPos());
	mtxWorld *= mtxPos;

	gameObj.SetMatrix(mtxWorld);
}

//--------------------------------------------------------------------------------
//  �o�b�t�@�쐬
//--------------------------------------------------------------------------------
KFRESULT C3DMeshComponent::CreateBuffer(void)
{
#ifdef USING_DIRECTX9
	LPDIRECT3DDEVICE9 pDevice = GetManager()->GetRenderer()->GetDevice();
	HRESULT hr;

	//���_�o�b�t�@
	hr = pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * m_meshInfo.nNumVtx,	//�쐬���������_�o�b�t�@�̃T�C�Y
		D3DUSAGE_WRITEONLY,						//���_�o�b�t�@�̎g�p���@
		FVF_VERTEX_3D,							//�����Ȃ��Ă����v
		D3DPOOL_MANAGED,						//�������Ǘ����@(managed�F�f�o�C�X�ɂ��C��)
		&m_meshInfo.pVtxBuffer,					//���_�o�b�t�@�̃|�C���^
		NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "C3DMeshComponent : CreateVertexBuffer ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
		return KF_FAILED;
	}

	//�C���f�b�N�X�o�b�t�@�̍쐬
	hr = pDevice->CreateIndexBuffer(
		sizeof(WORD) * m_meshInfo.nNumIdx,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_meshInfo.pIdxBuffer,
		NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "C3DMeshComponent : CreateIndexBuffer ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
		return KF_FAILED;
	}
#endif

	return KF_SUCCEEDED;
}
