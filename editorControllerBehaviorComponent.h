//--------------------------------------------------------------------------------
//	エディタビヘイビアコンポネント
//　editorControllerBehaviorComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-07-17
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "behaviorComponent.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class CFieldEditorBehaviorComponent;
class CModelEditorBehaviorComponent;

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  プレイヤー行動コンポネントクラス
//--------------------------------------------------------------------------------
class CEditorControllerBehaviorComponent : public CBehaviorComponent
{
public:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	CEditorControllerBehaviorComponent(CGameObject* const pGameObj);
	~CEditorControllerBehaviorComponent() {}

	bool	Init(void) override;
	void	Uninit(void) override;
	void	Update(void) override;
	void	LateUpdate(void) override {}

	void	OnTrigger(CColliderComponent& colliderThis, CColliderComponent& collider) override {}
	void	OnCollision(CCollisionInfo& collisionInfo) override {}

	void	SetFieldEditor(CGameObject* pFieldEditor);
	void	SetModelEditor(CModelEditorBehaviorComponent* pModelEditor) { m_pModelEditor = pModelEditor; }

private:
	//--------------------------------------------------------------------------------
	//  列挙型定義
	//--------------------------------------------------------------------------------
	enum CONTROL_MODE
	{
		CM_FIELD,
		CM_MODEL,
		CM_MAX
	};

	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	void	save(void);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	CONTROL_MODE					m_mode;
	CFieldEditorBehaviorComponent*	m_pFieldEditor;
	CModelEditorBehaviorComponent*	m_pModelEditor;
};