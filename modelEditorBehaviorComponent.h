//--------------------------------------------------------------------------------
//	���f���G�f�B�^�r�w�C�r�A�R���|�l���g
//�@modelEditorBehaviorComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-08-23
//--------------------------------------------------------------------------------
#pragma once
#ifdef _DEBUG
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "behaviorComponent.h"

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class CModelEditorBehaviorComponent : public CBehaviorComponent
{
public:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	CModelEditorBehaviorComponent(CGameObject* const pGameObj);
	~CModelEditorBehaviorComponent() {}

	bool	Init(void) override;
	void	Uninit(void) override;
	void	Update(void) override;
	void	LateUpdate(void) override;

	void	OnTrigger(CColliderComponent& colliderThis, CColliderComponent& collider) override {}
	void	OnCollision(CCollisionInfo& collisionInfo) override {}

	//Get�֐�
	bool	GetActive(void) const { return m_bActive; }

	//Set�֐�
	void	SetActive(const bool& bActive) { m_bActive = bActive; }
	void	SetPos(const CKFVec3& vPos);

	//Save
	void	SaveAs(const string& strFileName);

private:
	//--------------------------------------------------------------------------------
	//  �񋓌^��`
	//--------------------------------------------------------------------------------
	enum MODEL_TYPE
	{
		MT_BOX,
		MT_TREE,
		MT_PINE_TREE,
		MT_WINDMILL,
		MT_ROCK_1,
		MT_ROCK_2,
		MT_ROCK_3,
		MT_BAKER_HOUSE,
		MT_BARREL,
		MT_BRIDGE,
		MT_FENCE,
		MT_MAX
	};

	//--------------------------------------------------------------------------------
	//  �\���̒�`
	//--------------------------------------------------------------------------------
	struct INFO
	{
		INFO() : pTransform(nullptr), vRot(CKFMath::sc_vZero) {}
		CTransformComponent*	pTransform;
		CKFVec3					vRot;
	};

	//--------------------------------------------------------------------------------
	//  �萔��`
	//--------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	void	create(void);
	void	showMainWindow(void);
	void	showTypeListBox(void);
	void	showCreatedList(void);

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	string			m_aStrName[MT_MAX];
	INFO			m_aObjInfoDemo[MT_MAX];
	list<INFO>		m_alistCreated[MT_MAX];
	MODEL_TYPE		m_modelType;
	bool			m_bActive;
	bool			m_bShowCreatedList;
	float			m_fMoveSpeed;
	float			m_fRotSpeed;
};
#endif // _DEBUG