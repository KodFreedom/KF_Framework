//--------------------------------------------------------------------------------
//	�G�f�B�^�r�w�C�r�A�R���|�l���g
//�@editorControllerBehaviorComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-07-17
//--------------------------------------------------------------------------------
#pragma once
#if defined(_DEBUG) || defined(EDITOR)
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "behaviorComponent.h"

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------
class CFieldEditorBehaviorComponent;
class ModelEditorBehaviorComponent;

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class CEditorControllerBehaviorComponent : public CBehaviorComponent
{
public:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	CEditorControllerBehaviorComponent(GameObject* const pGameObj);
	~CEditorControllerBehaviorComponent() {}

	bool	Init(void) override;
	void	Uninit(void) override;
	void	Update(void) override;
	void	LateUpdate(void) override {}

	void	OnTrigger(Collider& colliderThis, Collider& collider) override {}
	void	OnCollision(CollisionInfo& collisionInfo) override {}

	void	SetFieldEditor(GameObject* pFieldEditor);
	void	SetModelEditor(ModelEditorBehaviorComponent* pModelEditor) { m_pModelEditor = pModelEditor; }

private:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	void	save(void);
	void	showMainWindow(void);
	void	showPosWindow(void);

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	bool							m_bAutoHeight;
	CFieldEditorBehaviorComponent*	m_pFieldEditor;
	ModelEditorBehaviorComponent*	m_pModelEditor;
	float							m_fMoveSpeed;
};
#endif // _DEBUG