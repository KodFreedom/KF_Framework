//--------------------------------------------------------------------------------
//
//�@gameObject3D.h
//	Author : Xu Wenjie
//	Date   : 2017-04-27
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _GAMEOBJECT_3D_H_
#define _GAMEOBJECT_3D_H_

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "gameObject.h"
#include "textureManager.h"
#include "materialManager.h"
#include "modelManager.h"

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class CGameObject3D : public CGameObject
{
public:
	CGameObject3D();
	CGameObject3D(const GOM::PRIORITY &pri);
	~CGameObject3D() {}

	//Get�֐�
	//const CGameObject3D& GetParents(void) const { return *m_pParents; }

	//�����֐�
	static CGameObject3D* CreateSkyBox(const CKFVec3 &vPos, const CKFVec3 &vRot, const CKFVec3 &vScale);
	static CGameObject3D* CreateField(const int &nNumBlockX, const int &nNumBlockZ, const CKFVec2 &vBlockSize, const CKFVec3 &vPos, const CKFVec3 &vRot, const CKFVec3 &vScale);
	static CGameObject3D* CreateCube(const CKFVec3 &vSize, const CKFColor &cColor, const CKFVec3 &vPos, const CKFVec3 &vRot, const CKFVec3 &vScale);

protected:

	//CGameObject3D*	m_pParents;	//�e�p�[�c
};

#endif