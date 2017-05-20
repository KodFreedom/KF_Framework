//--------------------------------------------------------------------------------
//
//�@gameObject2D.h
//	Author : Xu Wenjie
//	Date   : 2017-04-26
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _GAMEOBJECT_2D_H_
#define _GAMEOBJECT_2D_H_

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "gameObject.h"
#include "textureManager.h"

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class CGameObject2D : public CGameObject
{
public:
	CGameObject2D() : CGameObject(GOM::PRI_2D) {}
	~CGameObject2D() {}

	static CGameObject2D* Create(const CKFVec3 &vPos, const CKFVec3 &vRot, const CKFVec3 &vSize, const CTM::TEX_NAME &texName);
};

#endif