//--------------------------------------------------------------------------------
//
//　modelCharacter.h
//	Author : Xu Wenjie
//	Date   : 2017-01-23
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "model.h"
#include "materialManager.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class CModelActorX : public CModel
{
public:
	//--------------------------------------------------------------------------------
	//  構造体定義
	//--------------------------------------------------------------------------------
	struct KEY_FRAME
	{
		int		nFrame;
		CKFVec3 vPos;
		CKFVec3 vRot;
	};

	struct MOTION
	{
		bool				bLoop;			//ループフラッグ
		vector<KEY_FRAME>	vectorKeyFrame;	//Key Frame情報
	};

	typedef vector<MOTION>	VEC_MOTION;		//モーションの動的配列

	struct PARTS_INFO
	{
		CKFVec3				vPos;			//位置
		CKFVec3				vRot;			//回転
		CKFMtx44			mtxWorld;		//自分のワールドマトリクス
		int					nParentID;		//親のID
	};

	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	CModelActorX();
	~CModelActorX() {}

	bool	Init(const LPCSTR &pTxtPath);
	void	Uninit(void) override;
	void	Draw(const CKFMtx44 &mtxWorldParents, const CMM::MATERIAL &matType, const string& strTexName) override;
	void	Draw(const CKFMtx44 &mtxWorldParents, vector<PARTS_INFO> &vectorParts, const CMM::MATERIAL &matType, const string& strTexName);
	
	//Get関数
	vector<PARTS_INFO>			GetDefaultPartsInfo(void) const;
	const vector<VEC_MOTION>	&GetPartsMotionInfo(void) const;

	static CModelActorX *Create(const LPCSTR &pTxtPath);
private:
	//--------------------------------------------------------------------------------
	//  構造体定義
	//--------------------------------------------------------------------------------
	struct ACTOR
	{
		vector<PARTS_INFO>	vectorPartsInfoDefault;
		vector<VEC_MOTION>	vectorPartsMotionInfo;
		vector<XFILE>		vectorPartsXFileInfo;
	};

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	ACTOR		m_actorInfo;	//初期状態のパーツ
	float		m_fMoveSpeed;
	float		m_fJumpSpeed;
	float		m_fRadius;
};