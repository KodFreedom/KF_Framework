//--------------------------------------------------------------------------------
//	���b�V��
//�@mesh.h
//	Author : Xu Wenjie
//	Date   : 2017-07-15
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
class CMesh
{
public:
	CMesh() : m_nNumVtx(0), m_nNumIdx(0), m_nNumPolygon(0) {}
	~CMesh() 
	{
		SAFE_RELEASE(m_pVtxBuffer);
		SAFE_RELEASE(m_pIdxBuffer);
	}

	int				m_nNumVtx;
	int				m_nNumIdx;
	int				m_nNumPolygon;

#ifdef USING_DIRECTX
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffer;	// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9	m_pIdxBuffer;	// �C���f�b�N�X�ւ̃|�C���^
#endif
};