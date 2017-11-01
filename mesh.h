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
//  �\���̒�`
//--------------------------------------------------------------------------------
#ifdef USING_DIRECTX
enum DrawType
{
	PointList = 1,
	LineList = 2,
	LineStrip = 3,
	TriangleList = 4,
	TriangleStrip = 5,
	TriangleFan = 6,
};
#else
enum DrawType
{
	PointList = 0,
	LineList = 1,
	LineStrip = 3,
	TriangleList = 4,
	TriangleStrip = 5,
	TriangleFan = 6,
};
#endif

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
class Mesh
{
public:
	Mesh()
		: CurrentType(TriangleStrip)
		, VertexNumber(0)
		, IndexNumber(0)
		, PolygonNumber(0) {}
	~Mesh() 
	{
		SAFE_RELEASE(m_pVtxBuffer);
		SAFE_RELEASE(m_pIdxBuffer);
	}

	DrawType		CurrentType;
	int				VertexNumber;
	int				IndexNumber;
	int				PolygonNumber;

#ifdef USING_DIRECTX
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffer;	// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9	m_pIdxBuffer;	// �C���f�b�N�X�ւ̃|�C���^
#endif
};