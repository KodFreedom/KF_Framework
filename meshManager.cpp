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
#include "KF_Utility.h"
#include "renderManager.h"

#ifdef USING_DIRECTX
#include "rendererDX.h"
#endif

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//
//  Public
//
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
		return;
	}

	//���b�V���̍쐬
	MESH mesh;
	if (strName.find(".mesh") != string::npos) { mesh = loadFromMesh(strName); }
	else if (strName.find(".x") != string::npos) { mesh = loadFromXFile(strName); }
	else if (strName._Equal("cube")) { mesh = createCube(); }
	else if (strName._Equal("sphere")) { mesh = createSphere(); }
	else if (strName._Equal("skyBox")) { mesh = createSkyBox(); }
	else { assert("wrong type!!"); }

	//�ۑ�
	m_umMesh.emplace(strName, mesh);
}

//--------------------------------------------------------------------------------
//	�֐����FDisuseMesh
//  �֐������F���b�V���̔j��
//	�����F	strName�F���b�V���̖��O
//	�߂�l�F�Ȃ�
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
//	�֐����FCreateEditorField
//  �֐������F�G�f�B�^�p�t�B�[���h�̍쐬
//	�����F	nNumBlockX�FX�����̃u���b�N��
//			nNumBlockZ�FZ�����̃u���b�N��
//			vBlockSize�F�u���b�N�̃T�C�Y
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void CMeshManager::CreateEditorField(const int nNumBlockX, const int nNumBlockZ, const Vector2& vBlockSize)
{
	auto itr = m_umMesh.find("field");
	if (itr != m_umMesh.end())
	{ //���łɑ��݂��Ă�
		//itr->second.usNumUsers++;
		return;
	}

	auto pMesh = new CMesh;
	pMesh->m_nNumVtx = (nNumBlockX + 1) * (nNumBlockZ + 1);
	pMesh->m_nNumIdx = ((nNumBlockX + 1) * 2 + 2) * nNumBlockZ - 1;
	pMesh->m_nNumPolygon = (nNumBlockX + 2) * 2 * nNumBlockZ - 4;
	if (!createBuffer(pMesh)) { return; }

#ifdef USING_DIRECTX
	//���z�A�h���X���擾���邽�߂̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	pMesh->m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);
	Vector3 vStartPos = Vector3(-nNumBlockX * 0.5f * vBlockSize.X, 0.0f, nNumBlockZ * 0.5f * vBlockSize.Y);
	for (int nCntZ = 0; nCntZ < nNumBlockZ + 1; nCntZ++)
	{
		for (int nCntX = 0; nCntX < nNumBlockX + 1; nCntX++)
		{
			pVtx[nCntZ * (nNumBlockX + 1) + nCntX].Position = vStartPos
				+ Vector3(nCntX * vBlockSize.X, 0.0f, -nCntZ * vBlockSize.Y);
			pVtx[nCntZ * (nNumBlockX + 1) + nCntX].UV = Vector2(nCntX * 1.0f / (float)nNumBlockX, nCntZ * 1.0f / (float)nNumBlockX);
			pVtx[nCntZ * (nNumBlockX + 1) + nCntX].Color = Color(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[nCntZ * (nNumBlockX + 1) + nCntX].Normal = Vector3(0.0f, 1.0f, 0.0f);
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

	//���b�V���̍쐬
	MESH mesh;
	mesh.pMesh = pMesh;

	//�ۑ�
	m_umMesh.emplace("field", mesh);
}

//--------------------------------------------------------------------------------
//	�֐����FUpdateEditorField
//  �֐������F�G�f�B�^�p�t�B�[���h�f�[�^�̍X�V
//	�����F	vecVtx�F���_�f�[�^
//			listChoosenIdx�F�I�����ꂽ���_�̃C���f�b�N�X
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void CMeshManager::UpdateEditorField(const vector<Vector3>& vecVtx, const list<int>& listChoosenIdx)
{
#ifdef USING_DIRECTX

	//���z�A�h���X���擾���邽�߂̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	auto pMesh = GetMesh("field");
	pMesh->m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);
	auto cColor = Color(1.0f);
	for (int nCnt = 0; nCnt < (int)vecVtx.size(); ++nCnt)
	{
		pVtx[nCnt].Position = vecVtx[nCnt];
		pVtx[nCnt].Color = cColor;
	}

	//Choosen Color
	int nNumBlockX = 100;
	int nNumBlockZ = 100;
	auto cRed = Color(1.0f, 0.0f, 0.0f, 1.0f);
	for (auto nIdx : listChoosenIdx)
	{
		pVtx[nIdx].Color = cRed;

		//�@���v�Z
		int nCntZ = nIdx / (nNumBlockZ + 1);
		int nCntX = nIdx - nCntZ * (nNumBlockZ + 1);
		Vector3 Normal;
		Vector3 PositionThis = pVtx[nIdx].Position;
		Vector3 PositionLeft;
		Vector3 PositionRight;
		Vector3 PositionTop;
		Vector3 PositionBottom;

		if (nCntX == 0)
		{
			PositionLeft = PositionThis;
			PositionRight = pVtx[nCntZ * (nNumBlockX + 1) + nCntX + 1].Position;
		}
		else if (nCntX < nNumBlockX)
		{
			PositionLeft = pVtx[nCntZ * (nNumBlockX + 1) + nCntX - 1].Position;
			PositionRight = pVtx[nCntZ * (nNumBlockX + 1) + nCntX + 1].Position;
		}
		else
		{
			PositionLeft = pVtx[nCntZ * (nNumBlockX + 1) + nCntX - 1].Position;
			PositionRight = PositionThis;
		}

		if (nCntZ == 0)
		{
			PositionTop = PositionThis;
			PositionBottom = pVtx[(nCntZ + 1) * (nNumBlockX + 1) + nCntX].Position;
		}
		else if (nCntZ < nNumBlockZ)
		{
			PositionTop = pVtx[(nCntZ - 1) * (nNumBlockX + 1) + nCntX].Position;
			PositionBottom = pVtx[(nCntZ + 1) * (nNumBlockX + 1) + nCntX].Position;
		}
		else
		{
			PositionTop = pVtx[(nCntZ - 1) * (nNumBlockX + 1) + nCntX].Position;
			PositionBottom = PositionThis;
		}
		Normal = (PositionRight - PositionLeft) * (PositionBottom - PositionTop);
		CKFMath::VecNormalize(Normal);
		pVtx[nIdx].Normal = Normal;
	}

	//���z�A�h���X���
	pMesh->m_pVtxBuffer->Unlock();
#endif
}

//--------------------------------------------------------------------------------
//	�֐����FSaveEditorFieldAs
//  �֐������F�G�f�B�^�p�t�B�[���h�f�[�^�̕ۑ�
//	�����F	strFileName�F�t�@�C����
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void CMeshManager::SaveEditorFieldAs(const string& strFileName)
{
#ifdef USING_DIRECTX
	auto pMesh = GetMesh("field");

	//file open
	string strName = "data/MESH/" + strFileName + ".mesh";
	FILE *pFile;
	fopen_s(&pFile, strName.c_str(), "wb");

	//DrawType
	int nDrawType = (int)DT_TRIANGLESTRIP;
	fwrite(&nDrawType, sizeof(int), 1, pFile);

	//NumVtx
	fwrite(&pMesh->m_nNumVtx, sizeof(int), 1, pFile);

	//NumIdx
	fwrite(&pMesh->m_nNumIdx, sizeof(int), 1, pFile);

	//NumPolygon
	fwrite(&pMesh->m_nNumPolygon, sizeof(int), 1, pFile);

	//Vtx&Idx
	//���_�f�[�^
	VERTEX_3D* pVtx;
	pMesh->m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);
	//�F�𔒂ɖ߂�
	unsigned long Color = Color(1.0f);
	for (int nCnt = 0; nCnt < pMesh->m_nNumVtx; ++nCnt)
	{
		pVtx[nCnt].Color = Color;
	}

	//�ۑ�
	fwrite(pVtx, sizeof(VERTEX_3D), pMesh->m_nNumVtx, pFile);
	pMesh->m_pVtxBuffer->Unlock();
	
	//�C���f�b�N�X
	WORD* pIdx;
	pMesh->m_pIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);
	fwrite(pIdx, sizeof(WORD), pMesh->m_nNumIdx, pFile);
	pMesh->m_pIdxBuffer->Unlock();
	
	//Texture
	int nTexSize = 0;
	fwrite(&nTexSize, sizeof(int), 1, pFile);

	//RenderPriority
	auto rp = RP_3D;
	fwrite(&rp, sizeof(rp), 1, pFile);

	//RenderState
	auto rs = RS_LIGHTON_CULLFACEON_MUL;
	fwrite(&rs, sizeof(rs), 1, pFile);

	fclose(pFile);
#endif
}

//--------------------------------------------------------------------------------
//
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	�֐����FloadFromMesh
//  �֐������F���b�V���t�@�C������f�[�^�̓Ǎ�
//	�����F	strFileName�F�t�@�C���̖��O 
//	�߂�l�FMESH
//--------------------------------------------------------------------------------
CMeshManager::MESH CMeshManager::loadFromMesh(const string& strFileName)
{
	string strName = "data/MESH/" + strFileName;
	MESH mesh;
	FILE *pFile;

	//file open
	fopen_s(&pFile, strName.c_str(), "rb");

	if (!pFile) 
	{
		assert("failed to open file!!");
		return mesh;
	}
	
	mesh.pMesh = new CMesh;

	//DrawType
	fread_s(&mesh.pMesh->m_drawType, sizeof(int), sizeof(int), 1, pFile);

	//NumVtx
	fread_s(&mesh.pMesh->m_nNumVtx, sizeof(int), sizeof(int), 1, pFile);

	//NumIdx
	fread_s(&mesh.pMesh->m_nNumIdx, sizeof(int), sizeof(int), 1, pFile);

	//NumPolygon
	fread_s(&mesh.pMesh->m_nNumPolygon, sizeof(int), sizeof(int), 1, pFile);

#ifdef USING_DIRECTX
	if (!createBuffer(mesh.pMesh))
	{
		assert("failed to create buffer!!");
		return mesh;
	}

	//���_�f�[�^
	VERTEX_3D* pVtx;
	mesh.pMesh->m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);
	fread_s(pVtx, sizeof(VERTEX_3D) * mesh.pMesh->m_nNumVtx, sizeof(VERTEX_3D), mesh.pMesh->m_nNumVtx, pFile);
	mesh.pMesh->m_pVtxBuffer->Unlock();

	//�C���f�b�N�X
	WORD *pIdx;
	mesh.pMesh->m_pIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);
	fread_s(pIdx, sizeof(WORD) * mesh.pMesh->m_nNumIdx, sizeof(WORD), mesh.pMesh->m_nNumIdx, pFile);
	mesh.pMesh->m_pIdxBuffer->Unlock();
#endif

	//Texture
	int nSize;
	fread_s(&nSize, sizeof(int), sizeof(int), 1, pFile);
	mesh.renderInfo.strTex.resize(nSize);
	fread_s(&mesh.renderInfo.strTex[0], nSize, sizeof(char), nSize, pFile);

	//Render Priority
	fread_s(&mesh.renderInfo.renderPriority, sizeof(RENDER_PRIORITY), sizeof(RENDER_PRIORITY), 1, pFile);

	//Render State
	fread_s(&mesh.renderInfo.renderState, sizeof(RENDER_STATE), sizeof(RENDER_STATE), 1, pFile);

	fclose(pFile);

	return mesh;
}

//--------------------------------------------------------------------------------
//	�֐����FloadFromXFile
//  �֐������FX�t�@�C������f�[�^�̓Ǎ�
//	�����F	strPath�F�t�@�C���̃p�X
//	�߂�l�FMESH
//--------------------------------------------------------------------------------
CMeshManager::MESH CMeshManager::loadFromXFile(const string& strPath)
{
	MESH mesh;
	FILE* pFile;

	//file open
	fopen_s(&pFile, strPath.c_str(), "r");

	if (!pFile)
	{
		assert("failed to open file!!");
		return mesh;
	}

	mesh.pMesh = new CMesh;
	mesh.pMesh->m_drawType = DRAW_TYPE::DT_TRIANGLELIST;
	vector<Vector3>		vecVtx;
	vector<Vector3>		vecNormal;
	vector<Vector2>		vecUV;
	vector<Color>	vecColor;
	vector<int>			vecIdx;
	vector<int>			vecNormalIdx;
	vector<int>			vecColorIdx;

	string strBuf;
	while (CKFUtility::GetStrToken(pFile, "\n", strBuf) >= 0)
	{
		if (strBuf.compare("Mesh {") == 0)
		{
			//���_���̓Ǎ�
			CKFUtility::GetStrToken(pFile, ";", strBuf);
			stringstream ss;
			int nNumVtx;
			ss << strBuf;
			ss >> nNumVtx;

			//���_�f�[�^�̓Ǎ�
			vecVtx.resize(nNumVtx);
			for (int nCnt = 0; nCnt < nNumVtx; ++nCnt)
			{
				CKFUtility::GetStrToken(pFile, ";", strBuf);
				ss.clear();
				ss << strBuf;
				ss >> vecVtx[nCnt].X;
				CKFUtility::GetStrToken(pFile, ";", strBuf);
				ss.clear();
				ss << strBuf;
				ss >> vecVtx[nCnt].Y;
				CKFUtility::GetStrToken(pFile, ";", strBuf);
				ss.clear();
				ss << strBuf;
				ss >> vecVtx[nCnt].Z;
				CKFUtility::GetStrToken(pFile, "\n", strBuf);
			}

			//�|���S�����̓Ǎ�
			CKFUtility::GetStrToken(pFile, ";", strBuf);
			ss.clear();
			ss << strBuf;
			ss >> mesh.pMesh->m_nNumPolygon;

			//�C���f�b�N�X�̓Ǎ�
			mesh.pMesh->m_nNumVtx =
				mesh.pMesh->m_nNumIdx = mesh.pMesh->m_nNumPolygon * 3;
			vecIdx.resize(mesh.pMesh->m_nNumIdx);
			for (int nCnt = 0; nCnt < mesh.pMesh->m_nNumPolygon; ++nCnt)
			{
				//3;���΂�
				CKFUtility::GetStrToken(pFile, ";", strBuf);
				CKFUtility::GetStrToken(pFile, ",", strBuf);
				ss.clear();
				ss << strBuf;
				ss >> vecIdx[nCnt * 3];
				CKFUtility::GetStrToken(pFile, ",", strBuf);
				ss.clear();
				ss << strBuf;
				ss >> vecIdx[nCnt * 3 + 1];
				CKFUtility::GetStrToken(pFile, ";", strBuf);
				ss.clear();
				ss << strBuf;
				ss >> vecIdx[nCnt * 3 + 2];
				CKFUtility::GetStrToken(pFile, "\n", strBuf);
			}
		}

		if (strBuf.compare(" MeshMaterialList {") == 0)
		{
			//Material��
			CKFUtility::GetStrToken(pFile, ";", strBuf);
			stringstream ss;
			int nNumColor;
			ss << strBuf;
			ss >> nNumColor;
			vecColor.resize(nNumColor);

			//�C���f�b�N�X��
			CKFUtility::GetStrToken(pFile, ";", strBuf);
			ss.clear();
			int nNumPolygon;
			ss << strBuf;
			ss >> nNumPolygon;
			vecColorIdx.resize(nNumPolygon * 3);

			//�C���f�b�N�X�̓ǂݍ���
			for (int nCnt = 0; nCnt < nNumPolygon; ++nCnt)
			{
				CKFUtility::GetStrToken(pFile, ",;", strBuf);
				ss.clear();
				ss << strBuf;
				ss >> vecColorIdx[nCnt * 3];
				vecColorIdx[nCnt * 3 + 1] = 
					vecColorIdx[nCnt * 3 + 2] =
					vecColorIdx[nCnt * 3];
			}

			//�}�e���A���̓ǂݍ���
			for (int nCnt = 0; nCnt < nNumColor; ++nCnt)
			{
				while (CKFUtility::GetStrToken(pFile, "\n", strBuf) >= 0)
				{
					if (strBuf.compare("  Material {") == 0)
					{
						//R
						CKFUtility::GetStrToken(pFile, ";", strBuf);
						ss.clear();
						ss << strBuf;
						ss >> vecColor[nCnt].R;

						//G
						CKFUtility::GetStrToken(pFile, ";", strBuf);
						ss.clear();
						ss << strBuf;
						ss >> vecColor[nCnt].G;

						//B
						CKFUtility::GetStrToken(pFile, ";", strBuf);
						ss.clear();
						ss << strBuf;
						ss >> vecColor[nCnt].B;

						//A
						CKFUtility::GetStrToken(pFile, ";", strBuf);
						ss.clear();
						ss << strBuf;
						ss >> vecColor[nCnt].A;

						break;
					}
				}
			}
		}

		if (strBuf.compare(" MeshNormals {") == 0)
		{
			//�@�����̓Ǎ�
			CKFUtility::GetStrToken(pFile, ";", strBuf);
			stringstream ss;
			int nNumNormal;
			ss << strBuf;
			ss >> nNumNormal;

			//�@���f�[�^�̓Ǎ�
			vecNormal.resize(nNumNormal);
			for (int nCnt = 0; nCnt < nNumNormal; ++nCnt)
			{
				CKFUtility::GetStrToken(pFile, ";", strBuf);
				ss.clear();
				ss << strBuf;
				ss >> vecNormal[nCnt].X;
				CKFUtility::GetStrToken(pFile, ";", strBuf);
				ss.clear();
				ss << strBuf;
				ss >> vecNormal[nCnt].Y;
				CKFUtility::GetStrToken(pFile, ";", strBuf);
				ss.clear();
				ss << strBuf;
				ss >> vecNormal[nCnt].Z;
				CKFUtility::GetStrToken(pFile, "\n", strBuf);
			}

			//�@���C���f�b�N�X����Ǎ�(���_���Ɠ����̂���)
			CKFUtility::GetStrToken(pFile, ";", strBuf);
			ss.clear();
			int nNumPolygonNormalIdx;
			ss << strBuf;
			ss >> nNumPolygonNormalIdx;
			if (nNumPolygonNormalIdx != mesh.pMesh->m_nNumPolygon)
			{//�G���[
				MessageBox(NULL,"CMeshManager:�@�����ƒ��_�����Ⴄ�I", "�G���[", MB_OK | MB_ICONWARNING);
			}

			//�@���C���f�b�N�X�̓Ǎ�
			vecNormalIdx.resize(nNumPolygonNormalIdx * 3);
			for (int nCnt = 0; nCnt < mesh.pMesh->m_nNumPolygon; ++nCnt)
			{
				//3;���΂�
				CKFUtility::GetStrToken(pFile, ";", strBuf);
				CKFUtility::GetStrToken(pFile, ",", strBuf);
				ss.clear();
				ss << strBuf;
				ss >> vecNormalIdx[nCnt * 3];
				CKFUtility::GetStrToken(pFile, ",", strBuf);
				ss.clear();
				ss << strBuf;
				ss >> vecNormalIdx[nCnt * 3 + 1];
				CKFUtility::GetStrToken(pFile, ";", strBuf);
				ss.clear();
				ss << strBuf;
				ss >> vecNormalIdx[nCnt * 3 + 2];
				CKFUtility::GetStrToken(pFile, "\n", strBuf);
			}
		}

		if (strBuf.compare(" MeshTextureCoords {") == 0)
		{
			//UV���̓Ǎ�
			CKFUtility::GetStrToken(pFile, ";", strBuf);
			stringstream ss;
			int nNumUV;
			ss << strBuf;
			ss >> nNumUV;
			if (nNumUV != vecVtx.size())
			{//�G���[
				MessageBox(NULL, "CMeshManager:UV���ƒ��_�����Ⴄ�I", "�G���[", MB_OK | MB_ICONWARNING);
			}

			//UV�f�[�^�̓Ǎ�
			vecUV.resize(nNumUV);
			for (int nCnt = 0; nCnt < nNumUV; ++nCnt)
			{
				CKFUtility::GetStrToken(pFile, ";", strBuf);
				ss.clear();
				ss << strBuf;
				ss >> vecUV[nCnt].X;
				CKFUtility::GetStrToken(pFile, ";", strBuf);
				ss.clear();
				ss << strBuf;
				ss >> vecUV[nCnt].Y;
				CKFUtility::GetStrToken(pFile, "\n", strBuf);
			}
		}
	}

	fclose(pFile);
	
#ifdef USING_DIRECTX
	if (!createBuffer(mesh.pMesh))
	{
		assert("failed to create buffer!!");
		return mesh;
	}

	//���z�A�h���X���擾���邽�߂̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	mesh.pMesh->m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < mesh.pMesh->m_nNumIdx; ++nCnt)
	{
		pVtx[nCnt].Position = vecVtx[vecIdx[nCnt]];
		pVtx[nCnt].Normal = vecNormal[vecNormalIdx[nCnt]];
		pVtx[nCnt].UV = vecUV[vecIdx[nCnt]];
		pVtx[nCnt].Color = vecColor[vecColorIdx[nCnt]];
	}

	//���z�A�h���X���
	mesh.pMesh->m_pVtxBuffer->Unlock();

	//�C���f�b�N�X
	WORD *pIdx;
	mesh.pMesh->m_pIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCnt = 0; nCnt < mesh.pMesh->m_nNumIdx; ++nCnt)
	{
		pIdx[nCnt] = nCnt;
	}

	mesh.pMesh->m_pIdxBuffer->Unlock();
#endif

	return mesh;
}

//--------------------------------------------------------------------------------
//  Cube�̍쐬
//--------------------------------------------------------------------------------
CMeshManager::MESH CMeshManager::createCube(void)
{
	MESH mesh;
	mesh.pMesh = new CMesh;
	mesh.pMesh->m_nNumVtx = 6 * 4;
	mesh.pMesh->m_nNumIdx = 6 * 4 + 5 * 2;
	mesh.pMesh->m_nNumPolygon = 6 * 2 + 5 * 4;

#ifdef USING_DIRECTX
	if (!createBuffer(mesh.pMesh))
	{
		assert("failed to create buffer!!");
		return mesh;
	}

	//���z�A�h���X���擾���邽�߂̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	mesh.pMesh->m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);
	Vector3 vHalfSize = Vector3(1.0f) * 0.5f;
	Color cColor = Color(1.0f);
	int nCntVtx = 0;

	//����
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pVtx[nCntVtx].Position = Vector3(
			-vHalfSize.X + (nCnt % 2) * vHalfSize.X * 2.0f,
			vHalfSize.Y - (nCnt / 2) * vHalfSize.Y * 2.0f,
			-vHalfSize.Z);
		pVtx[nCntVtx].UV = Vector2((nCnt % 2) * 1.0f, (nCnt / 2) * 1.0f);
		pVtx[nCntVtx].Color = cColor;
		pVtx[nCntVtx].Normal = Vector3(0.0f, 0.0f, -1.0f);

		nCntVtx++;
	}

	//��
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pVtx[nCntVtx].Position = Vector3(
			-vHalfSize.X + (nCnt % 2) * vHalfSize.X * 2.0f,
			vHalfSize.Y,
			vHalfSize.Z - (nCnt / 2) * vHalfSize.Z * 2.0f);
		pVtx[nCntVtx].UV = Vector2((nCnt % 2) * 1.0f, (nCnt / 2) * 1.0f);
		pVtx[nCntVtx].Color = cColor;
		pVtx[nCntVtx].Normal = Vector3(0.0f, 1.0f, 0.0f);

		nCntVtx++;
	}

	//��
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pVtx[nCntVtx].Position = Vector3(
			-vHalfSize.X,
			vHalfSize.Y - (nCnt / 2) * vHalfSize.Y * 2.0f,
			vHalfSize.Z - (nCnt % 2) * vHalfSize.Z * 2.0f);
		pVtx[nCntVtx].UV = Vector2((nCnt % 2) * 1.0f, (nCnt / 2) * 1.0f);
		pVtx[nCntVtx].Color = cColor;
		pVtx[nCntVtx].Normal = Vector3(-1.0f, 0.0f, 0.0f);

		nCntVtx++;
	}

	//��
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pVtx[nCntVtx].Position = Vector3(
			-vHalfSize.X + (nCnt % 2) * vHalfSize.X * 2.0f,
			-vHalfSize.Y,
			-vHalfSize.Z + (nCnt / 2) * vHalfSize.Z * 2.0f);
		pVtx[nCntVtx].UV = Vector2((nCnt % 2) * 1.0f, (nCnt / 2) * 1.0f);
		pVtx[nCntVtx].Color = cColor;
		pVtx[nCntVtx].Normal = Vector3(0.0f, -1.0f, 0.0f);
		nCntVtx++;
	}

	//�E
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pVtx[nCntVtx].Position = Vector3(
			vHalfSize.X,
			vHalfSize.Y - (nCnt / 2) * vHalfSize.Y * 2.0f,
			-vHalfSize.Z + (nCnt % 2) * vHalfSize.Z * 2.0f);
		pVtx[nCntVtx].UV = Vector2((nCnt % 2) * 1.0f, (nCnt / 2) * 1.0f);
		pVtx[nCntVtx].Color = cColor;
		pVtx[nCntVtx].Normal = Vector3(1.0f, 0.0f, 0.0f);
		nCntVtx++;
	}

	//���
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pVtx[nCntVtx].Position = Vector3(
			vHalfSize.X - (nCnt % 2) * vHalfSize.X * 2.0f,
			vHalfSize.Y - (nCnt / 2) * vHalfSize.Y * 2.0f,
			vHalfSize.Z);
		pVtx[nCntVtx].UV = Vector2((nCnt % 2) * 1.0f, (nCnt / 2) * 1.0f);
		pVtx[nCntVtx].Color = cColor;
		pVtx[nCntVtx].Normal = Vector3(0.0f, 0.0f, 1.0f);
		nCntVtx++;
	}

	//���z�A�h���X���
	mesh.pMesh->m_pVtxBuffer->Unlock();

	//�C���f�b�N�X
	WORD *pIdx;
	mesh.pMesh->m_pIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCnt = 0; nCnt < mesh.pMesh->m_nNumIdx; nCnt++)
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

	mesh.pMesh->m_pIdxBuffer->Unlock();

	//Model�t�@�C���̕ۑ�
	string strFileName = "data/MODEL/cube.model";
	FILE *pFile;

	//file open
	fopen_s(&pFile, strFileName.c_str(), "wb");

	//Node��
	string strNodeName = "cube";
	int nSize = (int)strNodeName.size();
	fwrite(&nSize, sizeof(int), 1, pFile);
	fwrite(&strNodeName[0], sizeof(char), nSize, pFile);

	//Offset
	fwrite(&CKFMath::sc_vZero, sizeof(Vector3), 1, pFile);
	fwrite(&CKFMath::sc_vZero, sizeof(Vector3), 1, pFile);
	fwrite(&CKFMath::sc_vOne, sizeof(Vector3), 1, pFile);

	//Collider
	int nNumCollider = 1;
	fwrite(&nNumCollider, sizeof(int), 1, pFile);

	int nType = 1;
	fwrite(&nType, sizeof(int), 1, pFile);
	fwrite(&CKFMath::sc_vZero, sizeof(Vector3), 1, pFile);
	fwrite(&CKFMath::sc_vZero, sizeof(Vector3), 1, pFile);
	fwrite(&CKFMath::sc_vOne, sizeof(Vector3), 1, pFile);

	//Texture
	int nNumTexture = 1;
	fwrite(&nNumTexture, sizeof(int), 1, pFile);
	string strTexture = "nomal_cube.jpg";
	nSize = strTexture.size();
	fwrite(&nSize, sizeof(int), 1, pFile);
	fwrite(&strTexture[0], sizeof(char), nSize, pFile);

	//Mesh
	int nNumMesh = 1;
	fwrite(&nNumMesh, sizeof(int), 1, pFile);
	string strMeshName = strNodeName + ".mesh";
	nSize = (int)strMeshName.size();
	fwrite(&nSize, sizeof(int), 1, pFile);
	fwrite(&strMeshName[0], sizeof(char), nSize, pFile);

	fclose(pFile);

	//Mesh
	strFileName = "data/MESH/cube.mesh";

	//file open
	fopen_s(&pFile, strFileName.c_str(), "wb");

	//DrawType
	int nDrawType = (int)mesh.pMesh->m_drawType;
	fwrite(&nDrawType, sizeof(int), 1, pFile);

	//NumVtx
	fwrite(&mesh.pMesh->m_nNumVtx, sizeof(int), 1, pFile);

	//NumIdx
	fwrite(&mesh.pMesh->m_nNumIdx, sizeof(int), 1, pFile);

	//NumPolygon
	fwrite(&mesh.pMesh->m_nNumPolygon, sizeof(int), 1, pFile);

	//Vtx&Idx
	mesh.pMesh->m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);
	fwrite(pVtx, sizeof(VERTEX_3D), mesh.pMesh->m_nNumVtx, pFile);
	mesh.pMesh->m_pVtxBuffer->Unlock();
	mesh.pMesh->m_pIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);
	fwrite(pIdx, sizeof(WORD), mesh.pMesh->m_nNumIdx, pFile);
	mesh.pMesh->m_pIdxBuffer->Unlock();

	//Texture
	strTexture = "nomal_cube.jpg";
	nSize = strTexture.size();
	fwrite(&nSize, sizeof(int), 1, pFile);
	fwrite(&strTexture[0], sizeof(char), nSize, pFile);

	//RenderPriority
	auto rp = RP_3D;
	fwrite(&rp, sizeof(rp), 1, pFile);

	//RenderState
	auto rs = RS_LIGHTON_CULLFACEON_MUL;
	fwrite(&rs, sizeof(rs), 1, pFile);

	fclose(pFile);
#endif

	return mesh;
}

//--------------------------------------------------------------------------------
//  Sphere�̍쐬
//--------------------------------------------------------------------------------
CMeshManager::MESH CMeshManager::createSphere(void)
{
	MESH mesh;
	assert("������!!");
	return mesh;
}

//--------------------------------------------------------------------------------
//  SkyBox�̍쐬
//--------------------------------------------------------------------------------
CMeshManager::MESH CMeshManager::createSkyBox(void)
{
	MESH mesh;
	mesh.pMesh = new CMesh;
	mesh.pMesh->m_nNumVtx = 6 * 4;
	mesh.pMesh->m_nNumIdx = 6 * 4 + 5 * 2;
	mesh.pMesh->m_nNumPolygon = 6 * 2 + 5 * 4;

#ifdef USING_DIRECTX
	if (!createBuffer(mesh.pMesh))
	{
		assert("failed to create buffer!!");
		return mesh;
	}

	//���z�A�h���X���擾���邽�߂̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	mesh.pMesh->m_pVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);
	float fLength = (float)CCamera::DEFAULT_FAR * 0.5f;
	auto cColor = CKFMath::sc_cWhite;
	int nCntVtx = 0;
	float fUVtweens = 1.0f / 1024.0f;	//���Ԃ𖳂������߂�UV��1px�k�߂�

	//����
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pVtx[nCntVtx].Position = Vector3(
			fLength - (nCnt % 2) * fLength * 2.0f,
			fLength - (nCnt / 2) * fLength * 2.0f,
			-fLength);
		pVtx[nCntVtx].UV = Vector2((nCnt % 2) * 0.25f + 0.25f + fUVtweens - (nCnt % 2) * fUVtweens * 2.0f,
			(nCnt / 2) * 1.0f / 3.0f + 1.0f / 3.0f + fUVtweens - (nCnt / 2) * fUVtweens * 2.0f);
		pVtx[nCntVtx].Color = cColor;
		pVtx[nCntVtx].Normal = Vector3(0.0f, 0.0f, 1.0f);
		nCntVtx++;
	}

	//��
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pVtx[nCntVtx].Position = Vector3(
			fLength - (nCnt % 2) * fLength * 2.0f,
			fLength,
			fLength - (nCnt / 2) * fLength * 2.0f);
		pVtx[nCntVtx].UV = Vector2((nCnt % 2) * 0.25f + 0.25f + fUVtweens - (nCnt % 2) * fUVtweens * 2.0f,
			(nCnt / 2) * 1.0f / 3.0f + fUVtweens - (nCnt / 2) * fUVtweens * 2.0f);
		pVtx[nCntVtx].Color = cColor;
		pVtx[nCntVtx].Normal = Vector3(0.0f, -1.0f, 0.0f);
		nCntVtx++;
	}

	//��
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pVtx[nCntVtx].Position = Vector3(
			-fLength,
			fLength - (nCnt / 2) * fLength * 2.0f,
			-fLength + (nCnt % 2) * fLength * 2.0f);
		pVtx[nCntVtx].UV = Vector2((nCnt % 2) * 0.25f + 0.5f + fUVtweens - (nCnt % 2) * fUVtweens * 2.0f,
			(nCnt / 2) * 1.0f / 3.0f + 1.0f / 3.0f + fUVtweens - (nCnt / 2) * fUVtweens * 2.0f);
		pVtx[nCntVtx].Color = cColor;
		pVtx[nCntVtx].Normal = Vector3(1.0f, 0.0f, 0.0f);
		nCntVtx++;
	}

	//��
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pVtx[nCntVtx].Position = Vector3(
			fLength - (nCnt % 2) * fLength * 2.0f,
			-fLength,
			-fLength + (nCnt / 2) * fLength * 2.0f);
		pVtx[nCntVtx].UV = Vector2((nCnt % 2) * 0.25f + 0.25f + fUVtweens - (nCnt % 2) * fUVtweens * 2.0f,
			(nCnt / 2) * 1.0f / 3.0f + 2.0f / 3.0f + fUVtweens - (nCnt / 2) * fUVtweens * 2.0f);
		pVtx[nCntVtx].Color = cColor;
		pVtx[nCntVtx].Normal = Vector3(0.0f, 1.0f, 0.0f);
		nCntVtx++;
	}

	//�E
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pVtx[nCntVtx].Position = Vector3(
			fLength,
			fLength - (nCnt / 2) * fLength * 2.0f,
			fLength - (nCnt % 2) * fLength * 2.0f);
		pVtx[nCntVtx].UV = Vector2((nCnt % 2) * 0.25f + fUVtweens - (nCnt % 2) * fUVtweens * 2.0f,
			(nCnt / 2) * 1.0f / 3.0f + 1.0f / 3.0f + fUVtweens - (nCnt / 2) * fUVtweens * 2.0f);
		pVtx[nCntVtx].Color = cColor;
		pVtx[nCntVtx].Normal = Vector3(-1.0f, 0.0f, 0.0f);
		nCntVtx++;
	}

	//���
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		pVtx[nCntVtx].Position = Vector3(
			-fLength + (nCnt % 2) * fLength * 2.0f,
			fLength - (nCnt / 2) * fLength * 2.0f,
			fLength);
		pVtx[nCntVtx].UV = Vector2((nCnt % 2) * 0.25f + 0.75f + fUVtweens - (nCnt % 2) * fUVtweens * 2.0f,
			(nCnt / 2) * 1.0f / 3.0f + 1.0f / 3.0f + fUVtweens - (nCnt / 2) * fUVtweens * 2.0f);
		pVtx[nCntVtx].Color = cColor;
		pVtx[nCntVtx].Normal = Vector3(0.0f, 0.0f, -1.0f);
		nCntVtx++;
	}

	//���z�A�h���X���
	mesh.pMesh->m_pVtxBuffer->Unlock();

	//�C���f�b�N�X
	WORD *pIdx;
	mesh.pMesh->m_pIdxBuffer->Lock(0, 0, (void**)&pIdx, 0);

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

	mesh.pMesh->m_pIdxBuffer->Unlock();
#endif
	
	//Render State
	mesh.renderInfo.renderState = RS_LIGHTOFF_CULLFACEON_MUL;

	return mesh;
}

//--------------------------------------------------------------------------------
//  �o�b�t�@�̍쐬
//--------------------------------------------------------------------------------
bool CMeshManager::createBuffer(CMesh* pMesh)
{
#ifdef USING_DIRECTX
	auto pDevice = Main::GetManager()->GetRenderer()->GetDevice();
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