//--------------------------------------------------------------------------------
//
//　modelCharacter.h
//	Author : Xu Wenjie
//	Date   : 2017-01-23
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _MODEL_CHARACTOR_H_
#define _MODEL_CHARACTOR_H_

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
		bool					bLoop;			//ループフラッグ
		std::vector<KEY_FRAME>	vectorKeyFrame;	//Key Frame情報
	};

	typedef std::vector<MOTION>	VEC_MOTION;		//モーションの動的配列

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

	KFRESULT	Init(const LPCSTR &pTxtPath);
	void		Uninit(void) override;
	void		Draw(const CKFMtx44 &mtxWorldParents, std::vector<PARTS_INFO> &vectorParts);
	void		Draw(const CKFMtx44 &mtxWorldParents, std::vector<PARTS_INFO> &vectorParts, const CMM::MATERIAL &matType);
	
	//Get関数
	std::vector<PARTS_INFO>			GetDefaultPartsInfo(void) const;
	const std::vector<VEC_MOTION>	&GetPartsMotionInfo(void) const;

	static CModelActorX *Create(const LPCSTR &pTxtPath);
private:
	//--------------------------------------------------------------------------------
	//  構造体定義
	//--------------------------------------------------------------------------------
	struct ACTOR
	{
		std::vector<PARTS_INFO> vectorPartsInfoDefault;
		std::vector<VEC_MOTION>	vectorPartsMotionInfo;
		std::vector<XFILE>		vectorPartsXFileInfo;
	};

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	ACTOR		m_actorInfo;	//初期状態のパーツ
	float		m_fMoveSpeed;
	float		m_fJumpSpeed;
	float		m_fRadius;
};

#endif