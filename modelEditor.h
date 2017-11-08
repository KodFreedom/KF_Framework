//--------------------------------------------------------------------------------
//	モデルエディタビヘイビアコンポネント
//　ModelEditor.h
//	Author : Xu Wenjie
//	Date   : 2017-08-23
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
class Transform;

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class ModelEditor : public Behavior
{
public:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	ModelEditor(GameObject* const owner);
	~ModelEditor() {}

	bool	Init(void) override;
	void	Uninit(void) override;
	void	Update(void) override;
	void	LateUpdate(void) override;

	//Get関数
	bool	GetActive(void) const { return isActive; }

	//Set関数
	void	SetActive(const bool& value) { isActive = value; }
	void	SetPosition(const Vector3& position);

	//Save
	void	SaveAs(const string& fileName);

private:
	//--------------------------------------------------------------------------------
	//  列挙型定義
	//--------------------------------------------------------------------------------
	enum ModelType
	{
		Box,
		Tree,
		PineTree,
		Windmill,
		Rock1,
		Rock2,
		Rock3,
		BakerHouse,
		Barrel,
		Bridge,
		Fence,
		FancyTavern,
		MedHouse1,
		MedHouse2,
		MedHouseFantasy,
		House,
		Well,
		MedBridge,
		Max
	};

	//--------------------------------------------------------------------------------
	//  構造体定義
	//--------------------------------------------------------------------------------
	struct Info
	{
		Info() : MyTransform(nullptr), Rotation(Vector3::Zero) {}
		Transform* MyTransform;
		Vector3	   Rotation;
	};

	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	void create(void);
	void showMainWindow(void);
	void showTypeListBox(void);
	void showCreatedList(void);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	string			modelNames[ModelType::Max];
	Info			demoModelInfos[ModelType::Max];
	list<Info>		createdModelInfos[ModelType::Max];
	ModelType		currentModelType;
	bool			isActive;
	bool			isShowCreatedList;
};
#endif // _DEBUG