//--------------------------------------------------------------------------------
//	�A�N�^�[
//�@gameObjectActor.h
//	Author : Xu Wenjie
//	Date   : 2017-05-22
//--------------------------------------------------------------------------------
#ifndef _GAMEOBJECT_ACTOR_H_
#define _GAMEOBJECT_ACTOR_H_

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "gameObject3D.h"
#include "modelManager.h"

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class CGameObjectActor : public CGameObject3D
{
public:
	CGameObjectActor(const GOM::PRIORITY &pri, const OBJ_TYPE& type);
	~CGameObjectActor() {}

	//Get�֐�
	
	//Set�֐�

	//�����֐�
	static CGameObjectActor* CreatePlayer(const CMOM::MODEL_NAME &modelName, const CKFVec3 &vPos, const CKFVec3 &vRot, const CKFVec3 &vScale);
	static CGameObjectActor* CreateEnemy(const CMOM::MODEL_NAME &modelName, const CKFVec3 &vPos, const CKFVec3 &vRot, const CKFVec3 &vScale);
private:
};

#endif