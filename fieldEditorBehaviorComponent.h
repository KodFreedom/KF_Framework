//--------------------------------------------------------------------------------
//	プレイヤービヘイビアコンポネント
//　fieldEditorBehaviorComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-07-17
//--------------------------------------------------------------------------------
#pragma once
#if defined(_DEBUG) || defined(EDITOR)
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "behavior.h"

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class CFieldEditorBehaviorComponent : public Behavior
{
public:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	CFieldEditorBehaviorComponent(GameObject* const pGameObj);
	~CFieldEditorBehaviorComponent() {}

	bool	Init(void) override;
	void	Uninit(void) override;
	void	Update(void) override;
	void	LateUpdate(void) override {}

	void	OnTrigger(Collider& colliderThis, Collider& collider) override {}
	void	OnCollision(CollisionInfo& collisionInfo) override {}

	//Get関数
	void	AdjustPosInField(Vector3& Position, const bool& bAdjustHeight);
	bool	GetActive(void) const { return m_bActive; }

	//Set関数
	void	SetActive(const bool& bActive);
	void	SetPos(const Vector3& Position) { m_vEditorPos = Position; }

	//Save
	void	SaveAs(const string& strFileName);

private:
	//--------------------------------------------------------------------------------
	//  構造体定義
	//--------------------------------------------------------------------------------
	struct EINFO
	{
		list<int> listChoosenIdx;
	};

	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	float			getHeight(const Vector3& Position);
	EINFO			getInfo(void);
	void			showMainWindow(void);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	int				blockXNumber;
	int				blockZNumber;
	Vector2			blockSize;
	vector<Vector3>	vertexes;
	Vector3			PositionMin;
	Vector3			PositionMax;
	Vector3			m_vEditorPos;
	float			m_fEditorRadius;
	float			RaiseSpeed;
	float			m_fExtendSpeed;
	bool			m_bActive;
};
#endif // _DEBUG