//--------------------------------------------------------------------------------
//	�A�N�^�[
//�@gameObjectActor.h
//	Author : Xu Wenjie
//	Date   : 2017-05-22
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "gameObject.h"

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------
class CAnimatorComponent;

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class CGameObjectActor : public CGameObject
{
public:
	CGameObjectActor(const GOM::PRIORITY &pri, const OBJ_TYPE& type);
	~CGameObjectActor() {}

	bool Init(void) override;
	void Uninit(void) override;
	void LateUpdate(void) override;

	//Get�֐�
	CAnimatorComponent* GetAnimatorComponent(void) { return m_pAnimator; }
	
	//Set�֐�

	//�����֐�
	static CGameObjectActor* CreatePlayer(const string &modelPath, const CKFVec3 &vPos, const CKFVec3 &vRot, const CKFVec3 &vScale);
	static CGameObjectActor* CreateEnemy(const string &modelPath, const CKFVec3 &vPos, const CKFVec3 &vRot, const CKFVec3 &vScale);
private:
	CAnimatorComponent* m_pAnimator;
};
