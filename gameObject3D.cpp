//--------------------------------------------------------------------------------
//
//�@gameObject3D.cpp
//	Author : Xu Wenjie
//	Date   : 2017-04-19
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "rendererDX.h"
#include "gameObject3D.h"

//--------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
CGameObject3D::CGameObject3D() :CGameObject()
	, m_pIdxBuffer(NULL)
	, m_texName(CTM::TEX_MAX)
	, m_matType(CMM::MAT_NORMAL)
	, m_nIdxNum(0)
	, m_nPolygonNum(0)
	, m_nVtxNum(0)
	, m_vRot(CKFVec3(0.0f))
	, m_vScale(CKFVec3(1.0f))
{
}

//--------------------------------------------------------------------------------
//  �f�X�g���N�^
//--------------------------------------------------------------------------------
CGameObject3D::~CGameObject3D()
{
}

//--------------------------------------------------------------------------------
//  ������
//--------------------------------------------------------------------------------
KFRESULT CGameObject3D::Init(const int &nVtxNum, const int &nIdxNum, const int &nPolygonNum)
{
	m_nVtxNum = nVtxNum;
	m_nIdxNum = nIdxNum;
	m_nPolygonNum = nPolygonNum;

	KFRESULT result = CreateBuffer();
	
	return result;
}

//--------------------------------------------------------------------------------
// �I������
//--------------------------------------------------------------------------------
void CGameObject3D::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuffer != NULL)
	{
		m_pVtxBuffer->Release();
		m_pVtxBuffer = NULL;
	}

	// �C���f�b�N�X�o�b�t�@�̔j��
	if (m_pIdxBuffer != NULL)
	{
		m_pIdxBuffer->Release();
		m_pIdxBuffer = NULL;
	}
}

//--------------------------------------------------------------------------------
// �X�V����
//--------------------------------------------------------------------------------
void CGameObject3D::Update(void)
{

}

//--------------------------------------------------------------------------------
// �X�V����(�`�撼�O)
//--------------------------------------------------------------------------------
void CGameObject3D::LateUpdate(void)
{

}

//--------------------------------------------------------------------------------
// �`�揈��
//--------------------------------------------------------------------------------
void CGameObject3D::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetManager()->GetRenderer()->GetDevice();

	//RenderState�ݒ�
	SetRenderState();

	//�}�g���b�N�X�ݒ�
	SetMatrix();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(
		0,						//�X�g���[���ԍ�
		m_pVtxBuffer,			//���_�o�b�t�@
		0,						//�I�t�Z�b�g�i�J�n�ʒu�j
		sizeof(VERTEX_3D));		//�X�g���C�h��

	// ���_�C���f�b�N�X�̐ݒ�
	pDevice->SetIndices(m_pIdxBuffer);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	LPDIRECT3DTEXTURE9 pTexture = GetManager()->GetTextureManager()->GetTexture(m_texName);
	pDevice->SetTexture(0, pTexture);

	// �}�e���A���̐ݒ�
	D3DMATERIAL9 mat = GetManager()->GetMaterialManager()->GetMaterial(m_matType);
	pDevice->SetMaterial(&mat);

	//�v���~�e�B�u�`��
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		0,
		m_nVtxNum,
		0,
		m_nPolygonNum);

	//RenderState�߂�
	ResetRenderState();
}

//--------------------------------------------------------------------------------
// �o�b�t�@����
//--------------------------------------------------------------------------------
KFRESULT CGameObject3D::CreateBuffer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetManager()->GetRenderer()->GetDevice();
	HRESULT hr;

	//���_�o�b�t�@
	hr = pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * m_nVtxNum,	//�쐬���������_�o�b�t�@�̃T�C�Y
		D3DUSAGE_WRITEONLY,				//���_�o�b�t�@�̎g�p���@
		FVF_VERTEX_3D,					//�����Ȃ��Ă����v
		D3DPOOL_MANAGED,				//�������Ǘ����@(managed�F�f�o�C�X�ɂ��C��)
		&m_pVtxBuffer,					//���_�o�b�t�@�̃|�C���^
		NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "GameObject3D : CreateVertexBuffer ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
		return KF_FAILED;
	}

	//�C���f�b�N�X�o�b�t�@�̍쐬
	hr = pDevice->CreateIndexBuffer(
		sizeof(WORD) * m_nIdxNum,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuffer,
		NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "GameObject3D : CreateIndexBuffer ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
		return KF_FAILED;
	}

	return KF_SUCCEEDED;
}

//--------------------------------------------------------------------------------
// SetWorldMatrix
//--------------------------------------------------------------------------------
void CGameObject3D::SetMatrix(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetManager()->GetRenderer()->GetDevice();

	//���[���h���΍��W
	D3DXMATRIX mtxWorld;
	D3DXMATRIX mtxRot;
	D3DXMATRIX mtxPos;

	//�P�ʍs��ɏ�����
	D3DXMatrixIdentity(&mtxWorld);

	//��](Y->X->Z)
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_vRot.m_fY, m_vRot.m_fX, m_vRot.m_fZ);
	mtxWorld *= mtxRot;

	//���s�ړ�
	D3DXMatrixTranslation(&mtxPos, m_vPos.m_fX, m_vPos.m_fY, m_vPos.m_fZ);
	mtxWorld *= mtxPos;

	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
}

//--------------------------------------------------------------------------------
// SetRenderState
//--------------------------------------------------------------------------------
void CGameObject3D::SetRenderState(void)
{

}

//--------------------------------------------------------------------------------
// ResetRenderState
//--------------------------------------------------------------------------------
void CGameObject3D::ResetRenderState(void)
{

}