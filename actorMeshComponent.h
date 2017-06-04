//--------------------------------------------------------------------------------
//	アクターメッシュコンポネント
//　actorMeshComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-05-22
//--------------------------------------------------------------------------------
#ifndef _ACTOR_MESH_COMPONENT_H_
#define _ACTOR_MESH_COMPONENT_H_

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "modelMeshComponent.h"
#include "modelActorX.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
// アクターメッシュコンポネントクラス
//--------------------------------------------------------------------------------
class CActorMeshComponent : public CModelMeshComponent
{
public:
	//--------------------------------------------------------------------------------
	//  構造体定義
	//--------------------------------------------------------------------------------
	enum MOTION
	{
		MOTION_NEUTAL = 0,
		MOTION_MOVE,
		MOTION_ATTACK,
		MOTION_JUMP,
		MOTION_LANDING,
		MOTION_MAX
	};

	struct ACTOR_MOTION_INFO
	{
		MOTION	motionNow;		//今のモーション
		MOTION	motionNext;		//次のモーション
		int		nKeyNow;		//今のキーフレーム
		int		nCntFrame;		//今のフレーム
		std::vector<CModelActorX::PARTS_INFO>	vectorPartsInfo;	// パーツ情報
	};

	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	CActorMeshComponent(CGameObject* const pGameObj) : CModelMeshComponent(pGameObj, MESH_ACTOR)
	{
		m_motionInfo.nCntFrame = 0;
		m_motionInfo.nKeyNow = 0;
		m_motionInfo.motionNow = MOTION_NEUTAL;
		m_motionInfo.vectorPartsInfo.clear();
	}

	~CActorMeshComponent() {}

	virtual KFRESULT		Init(void) override;
	virtual void			Uninit(void) override;
	virtual void			Update(void) override;

	//Get関数
	ACTOR_MOTION_INFO& GetMotionInfo(void) { return m_motionInfo; }
	MOTION				GetMotionNow(void) const { return m_motionInfo.motionNow; }

	//Set関数
	void SetMotionNext(const MOTION& motion);
	void SetMotionAtNow(const MOTION& motion);

private:
	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	ACTOR_MOTION_INFO m_motionInfo;
};

#endif