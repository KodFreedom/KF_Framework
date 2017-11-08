//--------------------------------------------------------------------------------
//	プレイヤービヘイビアコンポネント
//　FieldEditor.h
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
class FieldEditor : public Behavior
{
public:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	FieldEditor(GameObject* const owner);
	~FieldEditor() {}

	bool	Init(void) override;
	void	Uninit(void) override;
	void	Update(void) override;
	void	LateUpdate(void) override {}

	//Get関数
	Vector3	AdjustPosInField(const Vector3& position, const bool& isAdjustHeight);
	bool	GetActive(void) const { return isActive; }

	//Set関数
	void	SetActive(const bool& value);
	void	SetPosition(const Vector3& value) { editorPosition = value; }

	//Save
	void	SaveAs(const string& fileName);

private:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	float			getHeight(const Vector3& position);
	list<int>		getChoosenIndexes(void);
	void			showMainWindow(void);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	int				blockXNumber;
	int				blockZNumber;
	Vector2			blockSize;
	vector<Vector3>	vertexes;
	Vector3			minPosition;
	Vector3			maxPosition;
	Vector3			editorPosition;
	float			editorRadius;
	float			raiseSpeed;
	float			extendSpeed;
	bool			isActive;
};
#endif // _DEBUG