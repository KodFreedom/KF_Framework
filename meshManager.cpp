//--------------------------------------------------------------------------------
//	���b�V���}�l�[�W��
//�@meshManager.cpp
//	Author : Xu Wenjie
//	Date   : 2017-07-15
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "meshManager.h"
#include "mesh.h"
#include "camera.h"

#ifdef USING_DIRECTX
#include "rendererDX.h"
#endif

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  ���g���폜����
//--------------------------------------------------------------------------------
void CMeshManager::UnloadAll(void)
{
	for (auto itr = m_umMesh.begin(); itr != m_umMesh.end();)
	{
		SAFE_RELEASE(itr->second.pMesh->m_pVtxBuffer);
		SAFE_RELEASE(itr->second.pMesh->m_pIdxBuffer);
		itr = m_umMesh.erase(itr);
	}
}

//--------------------------------------------------------------------------------
//  ���b�V���̒ǉ�
//--------------------------------------------------------------------------------
void CMeshManager::UseMesh(const string& strName)
{
	auto itr = m_umMesh.find(strName);
	if (itr != m_umMesh.end()) 
	{ //���łɑ��݂��Ă�
		itr->second.usNumUsers++;
	}

	//���b�V���̍쐬
	MESH mesh;
	mesh.usNumUsers = 1;
	mesh.pMesh = createMesh(strName);

	//�ۑ�
	m_umMesh.emplace(strName, mesh);
}

//--------------------------------------------------------------------------------
//  ���b�V���̒ǉ�
//--------------------------------------------------------------------------------
void CMeshManager::UseMesh(const string& strName, string& texName)
{
	auto itr = m_umMesh.find(strName);
	if (itr != m_umMesh.end())
	{ //���łɑ��݂��Ă�
		itr->second.usNumUsers++;
	}

	//���b�V���̍쐬
	MESH mesh;
	mesh.usNumUsers = 1;
	mesh.pMesh = createMesh(strName, texName);

	//�ۑ�
	m_umMesh.emplace(strName, mesh);
}

//--------------------------------------------------------------------------------
//  ���b�V���̔j��
//--------------------------------------------------------------------------------
void CMeshManager::DisuseMesh(const string& strName)
{
	auto itr = m_umMesh.find(strName);
	itr->second.usNumUsers--;
	if (itr->second.usNumUsers == 0)
	{//�N���g���ĂȂ��̂Ŕj������
		delete itr->second.pMesh;
		m_umMesh.erase(itr);
	}
}

//--------------------------------------------------------------------------------
//  ���b�V���̍쐬
//--------------------------------------------------------------------------------
CMesh* CMeshManager::createMesh(const string& strName)
{
	CMesh* pMesh = new CMesh;

	//�f�t�H���g�̃��b�V�����쐬
	if (strName == "cube") { createCube(pMesh); }
	else if (strName == "sphere") { createSphere(pMesh); }
	else if (strName == "skyBox") { createSkyBox(pMesh); }
	else if (strName == "field") { createField(pMesh); }
	return pMesh;
}

//--------------------------------------------------------------------------------
//  ���b�V���̍쐬
//--------------------------------------------------------------------------------
CMesh* CMeshManager::createMesh(const string& strName, string& texName)
{
	CMesh* pMesh = new CMesh;

	//���b�V���̖��O�̍Ō�̌ܕ�����.mesh���ǂ������`�F�b�N
	if (strName.find(".mesh") != string::npos)
	{//�t�@�C������ǂݍ���

	}

	else if (strName.find(".x") != string::npos)
	{//XFile
		createXFile(strName, pMesh, texName);
	}

	return pMesh;
}

//--------------------------------------------------------------------------------
//  Cube�̍쐬
//--------------------------------------------------------------------------------
void CMeshManager::createXFile(const string& strName, CMesh* pMesh, string& texName)
{
	LPDIRECT3DDEVICE9	pDevice = GetManager()->GetRenderer()->GetDevice();
	LPD3DXMESH			pDxMesh;		//���b�V�����C���^�[�t�F�C�X
	LPD3DXBUFFER		pBufferMaterial;//�}�e���A�����@���I������
	DWORD				dwNumMaterial;	//���f���̃}�e���A����

	//�n�[�h�f�B�X�N����X�t�@�C���̓ǂݍ���
	HRESULT hr = D3DXLoadMeshFromX(
		strName.c_str(),	//�t�@�C����
		D3DXMESH_MANAGED,
		pDevice,
		NULL,				//�אڏ��
		&pBufferMaterial,	//���f���̃}�e���A�����
		NULL,
		&dwNumMaterial,		//���f���̃}�e���A����
		&pDxMesh);			//���b�V�����

	if (FAILED(hr))
	{
		MessageBox(NULL, "CModel : D3DXLoadMeshFromX ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
		return;
	}

	//Mesh
	pMesh->m_nNumIdx = pDxMesh->GetNumVertices();
	pMesh->m_nNumPolygon = pDxMesh->GetNumFaces();
	pMesh->m_nNumVtx = pDxMesh->GetNumVertices();
	pDxMesh->GetIndexBuffer(&pMesh->m_pIdxBuffer);
	pDxMesh->GetVertexBuffer(&pMesh->m_pVtxBuffer);

	//texture
	D3DXMATERIAL *pMat;//�}�e���A�������󂯎���p�̃|�C���^

	pMat = (D3DXMATERIAL*)pBufferMaterial->GetBufferPointer();
	for (int nCnt = 0; nCnt < (int)dwNumMaterial; nCnt++)
	{
		if (pMat[nCnt].pTextureFilename != nullptr)
		{
			texName = pMat[nCnt].pTextureFilename;
			break;
		}
	}

	if (texName.empty())
	{//�e�N�X�`�����Ȃ��̏ꍇ���ʂȃ|���S���ɂ���
		texName = "polygon.jpg";
	}
}

//--------------------------------------------------------------------------------
//  Cube�̍쐬
//--------------------------------------------------------------------------------
void CMeshManager::createCube(CMesh* pMesh)
{
	pMesh->m_nNumVtx = 6 * 4;
	pMesh->m_nNumIdx = 6 * 4 + 5 * 2;
	pMesh->m_nNumPolygon = 6 * 2 + 5 * 4;
	if (!createBuffer(pMesh)) { return; }

#ifdef USING_DIRECTX
	//���z�A�h���X���擾���邽�߂̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	pMesh->m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);
	CKFVec3 vHalfSize = CKFVec3(1.0f) * 0.5f;
	CKFColor cColor = CKFColor(1.0f);
	int nCntVtx = 0;

	//����
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pVtx[nCntVtx].vPos = CKFVec3(
			-vHalfSize.m_fX + (nCnt % 2) * vHalfSize.m_fX * 2.0f,
			vHalfSize.m_fY - (nCnt / 2) * vHalfSize.m_fY * 2.0f,
			-vHalfSize.m_fZ);
		pVtx[nCntVtx].vUV = CKFVec2((nCnt % 2) * 1.0f, (nCnt / 2) * 1.0f);
		pVtx[nCntVtx].ulColor = cColor;
		pVtx[nCntVtx].vNormal = CKFVec3(0.0f, 0.0f, -1.0f);

		nCntVtx++;
	}

	//��
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pVtx[nCntVtx].vPos = CKFVec3(
			-vHalfSize.m_fX + (nCnt % 2) * vHalfSize.m_fX * 2.0f,
			vHalfSize.m_fY,
			vHalfSize.m_fZ - (nCnt / 2) * vHalfSize.m_fZ * 2.0f);
		pVtx[nCntVtx].vUV = CKFVec2((nCnt % 2) * 1.0f, (nCnt / 2) * 1.0f);
		pVtx[nCntVtx].ulColor = cColor;
		pVtx[nCntVtx].vNormal = CKFVec3(0.0f, 1.0f, 0.0f);

		nCntVtx++;
	}

	//��
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pVtx[nCntVtx].vPos = CKFVec3(
			-vHalfSize.m_fX,
			vHalfSize.m_fY - (nCnt / 2) * vHalfSize.m_fY * 2.0f,
			vHalfSize.m_fZ - (nCnt % 2) * vHalfSize.m_fZ * 2.0f);
		pVtx[nCntVtx].vUV = CKFVec2((nCnt % 2) * 1.0f, (nCnt / 2) * 1.0f);
		pVtx[nCntVtx].ulColor = cColor;
		pVtx[nCntVtx].vNormal = CKFVec3(-1.0f, 0.0f, 0.0f);

		nCntVtx++;
	}

	//��
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pVtx[nCntVtx].vPos = CKFVec3(
			-vHalfSize.m_fX + (nCnt % 2) * vHalfSize.m_fX * 2.0f,
			-vHalfSize.m_fY,
			-vHalfSize.m_fZ + (nCnt / 2) * vHalfSize.m_fZ * 2.0f);
		pVtx[nCntVtx].vUV = CKFVec2((nCnt % 2) * 1.0f, (nCnt / 2) * 1.0f);
		pVtx[nCntVtx].ulColor = cColor;
		pVtx[nCntVtx].vNormal = CKFVec3(0.0f, -1.0f, 0.0f);
		nCntVtx++;
	}

	//�E
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pVtx[nCntVtx].vPos = CKFVec3(
			vHalfSize.m_fX,
			vHalfSize.m_fY - (nCnt / 2) * vHalfSize.m_fY * 2.0f,
			-vHalfSize.m_fZ + (nCnt % 2) * vHalfSize.m_fZ * 2.0f);
		pVtx[nCntVtx].vUV = CKFVec2((nCnt % 2) * 1.0f, (nCnt / 2) * 1.0f);
		pVtx[nCntVtx].ulColor = cColor;
		pVtx[nCntVtx].vNormal = CKFVec3(1.0f, 0.0f, 0.0f);
		nCntVtx++;
	}

	//���
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pVtx[nCntVtx].vPos = CKFVec3(
			vHalfSize.m_fX - (nCnt % 2) * vHalfSize.m_fX * 2.0f,
			vHalfSize.m_fY - (nCnt / 2) * vHalfSize.m_fY * 2.0f,
			vHalfSize.m_fZ);
		pVtx[nCntVtx].vUV = CKFVec2((nCnt % 2) * 1.0f, (nCnt / 2) * 1.0f);
		pVtx[nCntVtx].ulColor = cColor;
		pVtx[nCntVtx].vNormal = CKFVec3(0.0f, 0.0f, 1.0f);
		nCntVtx++;
	}

	//���z�A�h���X���
	pMesh->m_pVtxBuffer->Unlock();

	//�C���f�b�N�X
	WORD *pIdx;
	pMesh->m_pIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCnt = 0; nCnt < pMesh->m_nNumIdx; nCnt++)
	{
		if (nCnt % 6 < 4)
		{
			pIdx[nCnt] = (WORD)((nCnt / 6) * 4 + (nCnt % 6) % 4);
		}
		else
		{//�k��
			pIdx[nCnt] = (WORD)((nCnt / 6) * 4 + (nCnt % 2) + 3);
		}
	}

	pMesh->m_pIdxBuffer->Unlock();

	//FILE *pFile;

	////file open
	//fopen_s(&pFile, "cube.mesh", "rb");
	//VERTEX_3D aV[100];
	//int aI[100];
	////�����ۑ�
	//fread(&m_meshInfo.nNumVtx, sizeof(int), 1, pFile);
	//fread(&m_meshInfo.nNumIdx, sizeof(int), 1, pFile);
	//fread(&m_meshInfo.nNumPolygon, sizeof(int), 1, pFile);
	//fread(aV, sizeof(VERTEX_3D), m_meshInfo.nNumVtx, pFile);
	//fread(aI, sizeof(int), m_meshInfo.nNumIdx, pFile);

	//fclose(pFile);
	//int n = 1;
#endif
}

//--------------------------------------------------------------------------------
//  Sphere�̍쐬
//--------------------------------------------------------------------------------
void CMeshManager::createSphere(CMesh* pMesh)
{

}

//--------------------------------------------------------------------------------
//  SkyBox�̍쐬
//--------------------------------------------------------------------------------
void CMeshManager::createSkyBox(CMesh* pMesh)
{
	pMesh->m_nNumVtx = 6 * 4;
	pMesh->m_nNumIdx = 6 * 4 + 5 * 2;
	pMesh->m_nNumPolygon = 6 * 2 + 5 * 4;
	if (!createBuffer(pMesh)) { return; }

#ifdef USING_DIRECTX
	//���z�A�h���X���擾���邽�߂̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	pMesh->m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);
	float fLength = (float)CCamera::DEFAULT_FAR * 0.5f;
	CKFColor cColor = CKFColor(1.0f);
	int nCntVtx = 0;
	float fUVtweens = 1.0f / 1024.0f;	//���Ԃ𖳂������߂�UV��1px�k�߂�

	//����
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pVtx[nCntVtx].vPos = CKFVec3(
			fLength - (nCnt % 2) * fLength * 2.0f,
			fLength - (nCnt / 2) * fLength * 2.0f,
			-fLength);
		pVtx[nCntVtx].vUV = CKFVec2((nCnt % 2) * 0.25f + 0.25f + fUVtweens - (nCnt % 2) * fUVtweens * 2.0f,
			(nCnt / 2) * 1.0f / 3.0f + 1.0f / 3.0f + fUVtweens - (nCnt / 2) * fUVtweens * 2.0f);
		pVtx[nCntVtx].ulColor = cColor;
		pVtx[nCntVtx].vNormal = CKFVec3(0.0f, 0.0f, 1.0f);
		nCntVtx++;
	}

	//��
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pVtx[nCntVtx].vPos = CKFVec3(
			fLength - (nCnt % 2) * fLength * 2.0f,
			fLength,
			fLength - (nCnt / 2) * fLength * 2.0f);
		pVtx[nCntVtx].vUV = CKFVec2((nCnt % 2) * 0.25f + 0.25f + fUVtweens - (nCnt % 2) * fUVtweens * 2.0f,
			(nCnt / 2) * 1.0f / 3.0f + fUVtweens - (nCnt / 2) * fUVtweens * 2.0f);
		pVtx[nCntVtx].ulColor = cColor;
		pVtx[nCntVtx].vNormal = CKFVec3(0.0f, -1.0f, 0.0f);
		nCntVtx++;
	}

	//��
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pVtx[nCntVtx].vPos = CKFVec3(
			-fLength,
			fLength - (nCnt / 2) * fLength * 2.0f,
			-fLength + (nCnt % 2) * fLength * 2.0f);
		pVtx[nCntVtx].vUV = CKFVec2((nCnt % 2) * 0.25f + 0.5f + fUVtweens - (nCnt % 2) * fUVtweens * 2.0f,
			(nCnt / 2) * 1.0f / 3.0f + 1.0f / 3.0f + fUVtweens - (nCnt / 2) * fUVtweens * 2.0f);
		pVtx[nCntVtx].ulColor = cColor;
		pVtx[nCntVtx].vNormal = CKFVec3(1.0f, 0.0f, 0.0f);
		nCntVtx++;
	}

	//��
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pVtx[nCntVtx].vPos = CKFVec3(
			fLength - (nCnt % 2) * fLength * 2.0f,
			-fLength,
			-fLength + (nCnt / 2) * fLength * 2.0f);
		pVtx[nCntVtx].vUV = CKFVec2((nCnt % 2) * 0.25f + 0.25f + fUVtweens - (nCnt % 2) * fUVtweens * 2.0f,
			(nCnt / 2) * 1.0f / 3.0f + 2.0f / 3.0f + fUVtweens - (nCnt / 2) * fUVtweens * 2.0f);
		pVtx[nCntVtx].ulColor = cColor;
		pVtx[nCntVtx].vNormal = CKFVec3(0.0f, 1.0f, 0.0f);
		nCntVtx++;
	}

	//�E
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pVtx[nCntVtx].vPos = CKFVec3(
			fLength,
			fLength - (nCnt / 2) * fLength * 2.0f,
			fLength - (nCnt % 2) * fLength * 2.0f);
		pVtx[nCntVtx].vUV = CKFVec2((nCnt % 2) * 0.25f + fUVtweens - (nCnt % 2) * fUVtweens * 2.0f,
			(nCnt / 2) * 1.0f / 3.0f + 1.0f / 3.0f + fUVtweens - (nCnt / 2) * fUVtweens * 2.0f);
		pVtx[nCntVtx].ulColor = cColor;
		pVtx[nCntVtx].vNormal = CKFVec3(-1.0f, 0.0f, 0.0f);
		nCntVtx++;
	}

	//���
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pVtx[nCntVtx].vPos = CKFVec3(
			-fLength + (nCnt % 2) * fLength * 2.0f,
			fLength - (nCnt / 2) * fLength * 2.0f,
			fLength);
		pVtx[nCntVtx].vUV = CKFVec2((nCnt % 2) * 0.25f + 0.75f + fUVtweens - (nCnt % 2) * fUVtweens * 2.0f,
			(nCnt / 2) * 1.0f / 3.0f + 1.0f / 3.0f + fUVtweens - (nCnt / 2) * fUVtweens * 2.0f);
		pVtx[nCntVtx].ulColor = cColor;
		pVtx[nCntVtx].vNormal = CKFVec3(0.0f, 0.0f, -1.0f);
		nCntVtx++;
	}

	//���z�A�h���X���
	pMesh->m_pVtxBuffer->Unlock();

	//�C���f�b�N�X
	WORD *pIdx;
	pMesh->m_pIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCnt = 0; nCnt < 6 * 4 + 5 * 2; nCnt++)
	{
		if (nCnt % 6 < 4)
		{
			pIdx[nCnt] = (nCnt / 6) * 4 + (nCnt % 6) % 4;
		}
		else
		{//�k��
			pIdx[nCnt] = (nCnt / 6) * 4 + (nCnt % 2) + 3;
		}
	}

	pMesh->m_pIdxBuffer->Unlock();
#endif
}

//--------------------------------------------------------------------------------
//  �o�b�t�@�̍쐬
//--------------------------------------------------------------------------------
bool CMeshManager::createBuffer(CMesh* pMesh)
{
#ifdef USING_DIRECTX
	LPDIRECT3DDEVICE9 pDevice = GetManager()->GetRenderer()->GetDevice();
	HRESULT hr;

	//���_�o�b�t�@
	hr = pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * pMesh->m_nNumVtx,	//�쐬���������_�o�b�t�@�̃T�C�Y
		D3DUSAGE_WRITEONLY,						//���_�o�b�t�@�̎g�p���@
		FVF_VERTEX_3D,							//�����Ȃ��Ă����v
		D3DPOOL_MANAGED,						//�������Ǘ����@(managed�F�f�o�C�X�ɂ��C��)
		&pMesh->m_pVtxBuffer,					//���_�o�b�t�@�̃|�C���^
		NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "CMeshManager : CreateVertexBuffer ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
		return false;
	}

	//�C���f�b�N�X�o�b�t�@�̍쐬
	hr = pDevice->CreateIndexBuffer(
		sizeof(WORD) * pMesh->m_nNumIdx,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&pMesh->m_pIdxBuffer,
		NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "CMeshManager : CreateIndexBuffer ERROR!!", "�G���[", MB_OK | MB_ICONWARNING);
		return false;
	}
#endif

	return true;
}

//--------------------------------------------------------------------------------
//  SkyBox�̍쐬
//--------------------------------------------------------------------------------
void CMeshManager::createField(CMesh* pMesh)
{
	int nNumBlockX = 100;
	int nNumBlockZ = 100;
	pMesh->m_nNumVtx = (nNumBlockX + 1) * (nNumBlockZ + 1);
	pMesh->m_nNumIdx = ((nNumBlockX + 1) * 2 + 2) * nNumBlockZ - 1;
	pMesh->m_nNumPolygon = (nNumBlockX + 2) * 2 * nNumBlockZ - 4;
	if (!createBuffer(pMesh)) { return; }

#ifdef USING_DIRECTX

	//���z�A�h���X���擾���邽�߂̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	float fHeight = 0.0f;
	CKFVec2 vSize = CKFVec2(2.0f, 2.0f);
	pMesh->m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);
	CKFVec3 vStartPos = CKFVec3(-nNumBlockX * 0.5f * vSize.m_fX, 0.0f, nNumBlockZ * 0.5f * vSize.m_fY);
	for (int nCntZ = 0; nCntZ < nNumBlockZ + 1; nCntZ++)
	{
		if (nCntZ <= (nNumBlockZ + 1) / 4)
		{
			fHeight += 0.25f;
		}
		else if (nCntZ <= (nNumBlockZ + 1) / 2)
		{
			fHeight -= 0.25f;
		}
		else if (nCntZ <= (nNumBlockZ + 1) * 3 / 4)
		{
			fHeight += 0.5f;
		}
		else
		{
			fHeight -= 0.5f;
		}
		for (int nCntX = 0; nCntX < nNumBlockX + 1; nCntX++)
		{
			pVtx[nCntZ * (nNumBlockX + 1) + nCntX].vPos = vStartPos
				+ CKFVec3(nCntX * vSize.m_fX, fHeight, -nCntZ * vSize.m_fY);
			pVtx[nCntZ * (nNumBlockX + 1) + nCntX].vUV = CKFVec2(nCntX * 1.0f, nCntZ * 1.0f);
			pVtx[nCntZ * (nNumBlockX + 1) + nCntX].ulColor = CKFColor(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[nCntZ * (nNumBlockX + 1) + nCntX].vNormal = CKFVec3(0.0f, 1.0f, 0.0f);
		}
	}

	//�@���v�Z
	for (int nCntZ = 0; nCntZ < nNumBlockX + 1; nCntZ++)
	{
		for (int nCntX = 0; nCntX < nNumBlockX + 1; nCntX++)
		{
			CKFVec3 vNormal;
			CKFVec3 vPosThis = pVtx[nCntZ * (nNumBlockX + 1) + nCntX].vPos;
			CKFVec3 vPosLeft;
			CKFVec3 vPosRight;
			CKFVec3 vPosTop;
			CKFVec3 vPosBottom;

			if (nCntX == 0)
			{
				vPosLeft = vPosThis;
				vPosRight = pVtx[nCntZ * (nNumBlockX + 1) + nCntX + 1].vPos;
			}
			else if (nCntX < nNumBlockX)
			{
				vPosLeft = pVtx[nCntZ * (nNumBlockX + 1) + nCntX - 1].vPos;
				vPosRight = pVtx[nCntZ * (nNumBlockX + 1) + nCntX + 1].vPos;
			}
			else
			{
				vPosLeft = pVtx[nCntZ * (nNumBlockX + 1) + nCntX - 1].vPos;
				vPosRight = vPosThis;
			}

			if (nCntZ == 0)
			{
				vPosTop = vPosThis;
				vPosBottom = pVtx[(nCntZ + 1) * (nNumBlockX + 1) + nCntX].vPos;
			}
			else if (nCntZ < nNumBlockZ)
			{
				vPosTop = pVtx[(nCntZ - 1) * (nNumBlockX + 1) + nCntX].vPos;
				vPosBottom = pVtx[(nCntZ + 1) * (nNumBlockX + 1) + nCntX].vPos;
			}
			else
			{
				vPosTop = pVtx[(nCntZ - 1) * (nNumBlockX + 1) + nCntX].vPos;
				vPosBottom = vPosThis;
			}

			vNormal = (vPosRight - vPosLeft) * (vPosBottom - vPosTop);
			CKFMath::VecNormalize(vNormal);

			pVtx[nCntZ * (nNumBlockX + 1) + nCntX].vNormal = vNormal;
		}
	}

	//���z�A�h���X���
	pMesh->m_pVtxBuffer->Unlock();

	//�C���f�b�N�X
	WORD *pIdx;
	pMesh->m_pIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntZ = 0; nCntZ < nNumBlockZ; nCntZ++)
	{
		for (int nCntX = 0; nCntX < (nNumBlockX + 1) * 2 + 2; nCntX++)
		{
			if (nCntX < (nNumBlockX + 1) * 2)
			{
				pIdx[nCntZ * ((nNumBlockX + 1) * 2 + 2) + nCntX] = nCntX / 2 + (nCntX + 1) % 2 * (nNumBlockX + 1) + nCntZ * (nNumBlockX + 1);
			}
			else if (nCntZ * ((nNumBlockX + 1) * 2 + 2) + nCntX < (((nNumBlockX + 1) * 2 + 2) * nNumBlockZ - 1))//�k�ރ|���S��
			{
				pIdx[nCntZ * ((nNumBlockX + 1) * 2 + 2) + nCntX] = (nCntX - 1) / 2 % (nNumBlockX + 1) + nCntX % 2 * 2 * (nNumBlockX + 1) + nCntZ * (nNumBlockX + 1);
			}
		}
	}

	pMesh->m_pIdxBuffer->Unlock();
#endif
}