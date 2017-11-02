//--------------------------------------------------------------------------------
//	風車ビヘイビアコンポネント
//　windmillBehaviorComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-09-12
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "windmillBehaviorComponent.h"
#include "gameObject.h"
#include "transformComponent.h"

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------
bool CWindmillBehaviorComponent::Init(void)
{
	m_pFan = findFan(m_pGameObj);

	if(!m_pFan)
	{
		assert("No Fan!!");
		return false;
	}
	return true;
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
void CWindmillBehaviorComponent::Update(void)
{
	m_pFan->GetTransformComponent()->RotByRoll(RotSpeed);
}

//--------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------
GameObject* CWindmillBehaviorComponent::findFan(GameObject* pParent)
{
	auto& listChild = pParent->GetTransformComponent()->GetChildren();
	for (auto pTrans : listChild)
	{
		auto pObj = pTrans->GetGameObject();
		if (pObj->GetName() == "Fan") { return pObj; }
		else
		{
			auto pFan = findFan(pObj);
			if (pFan) { return pFan; }
		}
	}

	return nullptr;
}