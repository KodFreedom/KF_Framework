//--------------------------------------------------------------------------------
//
//�@gameObject2D.cpp
//	Author : Xu Wenjie
//	Date   : 2017-04-26
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
#include "gameObject2D.h"

//--------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
CGameObject2D::CGameObject2D()
	: m_fRot(0.0f)
	, m_vSize(CKFVec2(0.0f, 0.0f))
	, m_cColor(CKFColor(1.0f))
	, m_texName(CTM::TEX_MAX)
{
}

//--------------------------------------------------------------------------------
//  �f�X�g���N�^
//--------------------------------------------------------------------------------
CGameObject2D::~CGameObject2D()
{
}

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
KFRESULT CGameObject2D::Init(const CKFVec3 &vPos, const float &fRot, const CKFVec2 &vSize, const CTM::TEX_NAME &texName)
{
	m_vPos = vPos;
	m_fRot = fRot;
	m_vSize = vSize;
	m_texName = texName;

	if (CreateBuffer() == KF_FAILED)
	{
		return KF_FAILED;
	}

	UpdateVertex();

	return KF_SUCCEEDED;
}

//--------------------------------------------------------------------------------
// �I������
//--------------------------------------------------------------------------------
void CGameObject2D::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuffer != NULL)
	{
		m_pVtxBuffer->Release();
		m_pVtxBuffer = NULL;
	}
}

//--------------------------------------------------------------------------------
// �X�V����
//--------------------------------------------------------------------------------
void CGameObject2D::Update(void)
{

}

//--------------------------------------------------------------------------------
// �X�V����(�`�撼�O)
//--------------------------------------------------------------------------------
void CGameObject2D::LateUpdate(void)
{
	UpdateVertex();
}

//--------------------------------------------------------------------------------
// �`�揈��
//--------------------------------------------------------------------------------
void CGameObject2D::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetManager()->GetRenderer()->GetDevice();

	// �����_�[�X�e�[�g�ݒ�
	SetRenderState();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(
		0,						//�X�g���[���ԍ�
		m_pVtxBuffer,			//���_�o�b�t�@
		0,						//�I�t�Z�b�g�i�J�n�ʒu�j
		sizeof(VERTEX_2D));		//�X�g���C�h��

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	LPDIRECT3DTEXTURE9 pTexture = GetManager()->GetTextureManager()->GetTexture(m_texName);
	pDevice->SetTexture(0, pTexture);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,						//�I�t�Z�b�g�i���_���j
		m_nNumPolygon2D);		//�|���S����

	// �����_�[�X�e�[�g�߂�
	ResetRenderState();
}

//--------------------------------------------------------------------------------
// �����_�[�X�e�[�g�ݒ�
//--------------------------------------------------------------------------------
void CGameObject2D::SetRenderState(void)
{

}

//--------------------------------------------------------------------------------
// �����_�[�X�e�[�g�߂�
//--------------------------------------------------------------------------------
void CGameObject2D::ResetRenderState(void)
{

}

//--------------------------------------------------------------------------------
//  ���_�X�V
//--------------------------------------------------------------------------------
void CGameObject2D::UpdateVertex(void)
{
	// ���_����ݒ�
	//���z�A�h���X���擾���邽�߂̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	//���_�ʒu�ݒ�
	SetVtxPos(pVtx);

	//���_�J���[�ݒ�
	SetVtxColor(pVtx);

	//���_UV�ݒ�
	SetVtxUV(pVtx);

	//rhw�̐ݒ�(�K��1.0f)
	pVtx[0].fRhw = 1.0f;
	pVtx[1].fRhw = 1.0f;
	pVtx[2].fRhw = 1.0f;
	pVtx[3].fRhw = 1.0f;

	//���z�A�h���X���
	m_pVtxBuffer->Unlock();
}

//--------------------------------------------------------------------------------
//  ���_�o�b�t�@��������
//--------------------------------------------------------------------------------
KFRESULT CGameObject2D::CreateBuffer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetManager()->GetRenderer()->GetDevice();
	HRESULT hr;

	//���_�o�b�t�@
	hr = pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * m_nNumVtx2D,	//�쐬���������_�o�b�t�@�̃T�C�Y
		D3DUSAGE_WRITEONLY,					//���_�o�b�t�@�̎g�p���@
		FVF_VERTEX_2D,						//�����Ȃ��Ă����v
		D3DPOOL_MANAGED,					//�������Ǘ����@(managed�F�f�o�C�X�ɂ��C��)
		&m_pVtxBuffer,						//���_�o�b�t�@�̃A�h���X
		NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "CGameObject2D : CreateVertexBuffer ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
		return KF_FAILED;
	}

	return KF_SUCCEEDED;
}

//--------------------------------------------------------------------------------
//  ���_���W�ݒ�
//--------------------------------------------------------------------------------
void CGameObject2D::SetVtxPos(VERTEX_2D *pVtx)
{
	float fLength = CKFMath::VecMagnitude(m_vSize) * 0.5f;
	float fAngle = atan2f(m_vSize.m_fY, m_vSize.m_fX);

	//���_���W�̐ݒ�i2D���W�A�E���j
	pVtx[0].vPos = CKFVec3(m_vPos.m_fX + cosf(m_fRot + KF_PI + fAngle) * fLength, m_vPos.m_fY + sinf(m_fRot + KF_PI + fAngle) * fLength, 0.0f);
	pVtx[1].vPos = CKFVec3(m_vPos.m_fX + cosf(m_fRot - fAngle) * fLength, m_vPos.m_fY + sinf(m_fRot - fAngle) * fLength, 0.0f);
	pVtx[2].vPos = CKFVec3(m_vPos.m_fX + cosf(m_fRot - KF_PI - fAngle) * fLength, m_vPos.m_fY + sinf(m_fRot - KF_PI - fAngle) * fLength, 0.0f);
	pVtx[3].vPos = CKFVec3(m_vPos.m_fX + cosf(m_fRot + fAngle) * fLength, m_vPos.m_fY + sinf(m_fRot + fAngle) * fLength, 0.0f);
}

//--------------------------------------------------------------------------------
//  ���_�J���[�ݒ�
//--------------------------------------------------------------------------------
void CGameObject2D::SetVtxColor(VERTEX_2D *pVtx)
{
	//���_�J���[�̐ݒ�(0�`255�̐����l)
	pVtx[0].ulColor = m_cColor;
	pVtx[1].ulColor = m_cColor;
	pVtx[2].ulColor = m_cColor;
	pVtx[3].ulColor = m_cColor;
}

//--------------------------------------------------------------------------------
//  ���_UV�ݒ�
//--------------------------------------------------------------------------------
void CGameObject2D::SetVtxUV(VERTEX_2D *pVtx)
{
	//texture���_
	pVtx[0].vUV = CKFVec2(0.0f, 0.0f);
	pVtx[1].vUV = CKFVec2(1.0f, 0.0f);
	pVtx[2].vUV = CKFVec2(0.0f, 1.0f);
	pVtx[3].vUV = CKFVec2(1.0f, 1.0f);
}

//--------------------------------------------------------------------------------
//  �I�u�W�F�N�g�̐���
//--------------------------------------------------------------------------------
CGameObject2D* CGameObject2D::Create(const CKFVec3 &vPos, const float &fRot, const CKFVec2 &vSize, const CTM::TEX_NAME &texName)
{
	CGameObject2D* pObj = NULL;
	pObj = new CGameObject2D;
	pObj->Init(vPos, fRot, vSize, texName);
	pObj->m_pri = GOM::PRI_2D;
	pObj->m_nID = GetManager()->GetGameObjectManager()->SaveGameObj(pObj->m_pri, pObj);
	return pObj;
}