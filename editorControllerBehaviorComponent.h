//--------------------------------------------------------------------------------
//	エディタビヘイビアコンポネント
//　editorControllerBehaviorComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-07-17
//--------------------------------------------------------------------------------
#pragma once
#if defined(_DEBUG) || defined(EDITOR)
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "behaviorComponent.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class CFieldEditorBehaviorComponent;
class ModelEditorBehaviorComponent;

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class CEditorControllerBehaviorComponent : public CBehaviorComponent
{
public:
	//--------------------------------------------------------------------------------
	//  関数定義
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
	//  関数定義
	//--------------------------------------------------------------------------------
	void	save(void);
	void	showMainWindow(void);
	void	showPosWindow(void);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	bool							m_bAutoHeight;
	CFieldEditorBehaviorComponent*	m_pFieldEditor;
	ModelEditorBehaviorComponent*	m_pModelEditor;
	float							m_fMoveSpeed;
};
#endif // _DEBUG