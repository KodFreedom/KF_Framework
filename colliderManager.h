//--------------------------------------------------------------------------------
//	�R���W�����}�l�[�W��
//�@colliderManager.h
//	Author : Xu Wenjie
//	Date   : 2017-06-04
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------
class CColliderComponent;
class CSphereColliderComponent;
class CFieldColliderComponent;

//--------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------
#define CM CColliderManager	//CColliderManager�̗���

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class CColliderManager
{
public:
	//--------------------------------------------------------------------------------
	//  �\���̒�`
	//--------------------------------------------------------------------------------
	enum MODE
	{
		STATIC = 0,
		DYNAMIC,
		MODE_MAX
	};

	enum COL_TYPE
	{
		COL_SPHERE = 0,
		COL_CYLINDER,
		COL_MAX,		//�ȉ��͓��ʂ�Collider(Static����Dynamic�̂ݎg��)
		COL_FIELD
	};

	typedef list<CColliderComponent*>::iterator COL_ITR;

	struct COL_INFO
	{//�܂��g��Ȃ��A�����蔻��^�C�v���w��ł���d�l
		CColliderComponent* pCollider;		//Collider�̃|�C���^
		list<COL_TYPE> listTargetType;	//Collider���ǂ̃^�C�v�Ɠ����蔻�肷�邩
	};

	struct HIT_INFO
	{
		CKFVec3 vPos;
		CKFVec3	vNormal;
	};

	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	CColliderManager();
	~CColliderManager() {}

	void		Init(void);
	void		Uninit(void);
	void		Update(void);

#ifdef _DEBUG
	void		DrawCollider(void);
#endif

	COL_ITR		SaveCollider(const MODE& mode, const COL_TYPE& type, CColliderComponent* pCollider);
	void		ReleaseCollider(const MODE& mode, const COL_TYPE& type, const COL_ITR& itr);
	COL_ITR		SaveField(CColliderComponent* pCollider);
	void		ReleaseField(const COL_ITR& itr);

	//�Վ��̗p
	bool		SphereCast(const CKFVec3& vCenter, const float& fRadius, HIT_INFO& hitInfoOut);
	bool		RayCast(const CKFRay& ray, const float& fLength, HIT_INFO& hitInfoOut);

private:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	void		CheckWithDynamicSphere(const COL_ITR& itrBegin, CSphereColliderComponent& sphere);
	void		CheckWithStaticSphere(CSphereColliderComponent& sphere);
	void		CheckWithField(CSphereColliderComponent& sphere);

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	list<CColliderComponent*> m_alistCollider[MODE_MAX][COL_MAX];
	list<CColliderComponent*> m_listField;
#ifdef _DEBUG
	LPD3DXMESH m_pMeshSphere;
#endif
};