//--------------------------------------------------------------------------------
//	3Dメッシュコンポネント
//　3DmeshComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-05-18
//--------------------------------------------------------------------------------
#ifndef _3D_MESH_COMPONENT_H_
#define _3D_MESH_COMPONENT_H_

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "meshComponent.h"
#include "textureManager.h"
#include "materialManager.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  3Dメッシュコンポネントクラス
//--------------------------------------------------------------------------------
class C3DMeshComponent : public CMeshComponent
{
public:
	//--------------------------------------------------------------------------------
	//  構造体定義
	//--------------------------------------------------------------------------------
	struct MESH3D
	{
		int						nNumVtx;
		int						nNumIdx;
		int						nNumPolygon;

#ifdef USING_DIRECTX9
		LPDIRECT3DVERTEXBUFFER9 pVtxBuffer;	// 頂点バッファへのポインタ
		LPDIRECT3DINDEXBUFFER9	pIdxBuffer;	// インデックスへのポインタ
#endif
	};

	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	C3DMeshComponent() : CMeshComponent()
		, m_texName(CTM::TEX_NONE)
		, m_matType(CMM::MAT_NORMAL)
	{
		m_meshInfo.nNumIdx = 0;
		m_meshInfo.nNumPolygon = 0;
		m_meshInfo.nNumVtx = 0;
		m_meshInfo.pIdxBuffer = NULL;
		m_meshInfo.pVtxBuffer = NULL;
	}

	~C3DMeshComponent() {}

	virtual KFRESULT		Init(void) override = 0;
	void					Uninit(void) override;
	void					Update(CGameObject &gameObj) override;

	//Get関数
	const CTM::TEX_NAME&	GetTexName(void) const { return m_texName; }
	const CMM::MATERIAL&	GetMatType(void) const { return m_matType; }
	const MESH3D&			GetMeshInfo(void) const { return m_meshInfo; }

	//Set関数
	void					SetTexName(const CTM::TEX_NAME& texName) { m_texName = texName; }
	void					SetMatType(const CMM::MATERIAL& matType) { m_matType = matType; }

protected:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	KFRESULT			CreateBuffer(void);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	CTM::TEX_NAME		m_texName;		//テクスチャ
	CMM::MATERIAL		m_matType;		//マテリアル
	MESH3D				m_meshInfo;		//メッシュ情報
};

#endif