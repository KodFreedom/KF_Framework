//--------------------------------------------------------------------------------
//	エディタビヘイビアコンポネント
//　editorControllerBehaviorComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-07-17
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "inputManager.h"
#include "mode.h"
#include "camera.h"
#include "gameObject.h"
#include "editorControllerBehaviorComponent.h"
#include "fieldEditorBehaviorComponent.h"
#include "transformComponent.h"

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
CEditorControllerBehaviorComponent::CEditorControllerBehaviorComponent(CGameObject* const pGameObj)
	: CBehaviorComponent(pGameObj)
	, m_mode(CM_FIELD)
	, m_pFieldEditor(nullptr)
{

}

//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
bool CEditorControllerBehaviorComponent::Init(void)
{
	return true;
}

//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
void CEditorControllerBehaviorComponent::Uninit(void)
{

}

//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
void CEditorControllerBehaviorComponent::Update(void)
{
	auto pInput = CMain::GetManager()->GetInputManager();

	//モードの切り替え
	m_mode = (CONTROL_MODE)((int)m_mode + (int)pInput->GetKeyTrigger(CInputManager::K_UP) - (int)pInput->GetKeyTrigger(CInputManager::K_DOWN));
	m_mode = (CONTROL_MODE)((int)m_mode < 0 ? (int)CM_MAX - 1 : m_mode >= CM_MAX ? 0 : m_mode);

	//今アクティブのモードをオンにする
	m_pFieldEditor->SetActive(m_mode == CM_FIELD);

	//標的操作
	auto pTrans = m_pGameObj->GetTransformComponent();
	auto vPos = pTrans->GetPos();
	auto vAxis = CKFVec2(pInput->GetMoveHorizontal(), pInput->GetMoveVertical());
	auto pCamera = CMain::GetManager()->GetMode()->GetCamera();
	auto vCamForward = CKFMath::Vec3Scale(pCamera->GetVecLook(), CKFMath::VecNormalize(CKFVec3(1.0f, 0.0f, 1.0f)));
	auto vMove = pCamera->GetVecRight() * vAxis.m_fX + vCamForward * vAxis.m_fY;
	vPos += vMove;
	m_pFieldEditor->AdjustPosInField(vPos);
	pTrans->SetPosNext(vPos);

	//FieldEditorの操作中心の更新
	m_pFieldEditor->SetPos(vPos);

	//カメラの移動
	auto vMovement = vPos - pTrans->GetPos();
	pCamera->MoveCamera(vMovement);
}

//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
void CEditorControllerBehaviorComponent::SetFieldEditor(CGameObject* pFieldEditor)
{
	auto listBehavior = pFieldEditor->GetBehaviorComponent();
	if (listBehavior.empty()) { return; }
	auto pBehavior = listBehavior.front();
	m_pFieldEditor = static_cast<CFieldEditorBehaviorComponent*>(pBehavior);
}