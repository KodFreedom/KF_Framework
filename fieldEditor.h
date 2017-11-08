//--------------------------------------------------------------------------------
//	�v���C���[�r�w�C�r�A�R���|�l���g
//�@FieldEditor.h
//	Author : Xu Wenjie
//	Date   : 2017-07-17
//--------------------------------------------------------------------------------
#pragma once
#if defined(_DEBUG) || defined(EDITOR)
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "behavior.h"

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class FieldEditor : public Behavior
{
public:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	FieldEditor(GameObject* const owner);
	~FieldEditor() {}

	bool	Init(void) override;
	void	Uninit(void) override;
	void	Update(void) override;
	void	LateUpdate(void) override {}

	//Get�֐�
	Vector3	AdjustPosInField(const Vector3& position, const bool& isAdjustHeight);
	bool	GetActive(void) const { return isActive; }

	//Set�֐�
	void	SetActive(const bool& value);
	void	SetPosition(const Vector3& value) { editorPosition = value; }

	//Save
	void	SaveAs(const string& fileName);

private:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	float			getHeight(const Vector3& position);
	list<int>		getChoosenIndexes(void);
	void			showMainWindow(void);

	//--------------------------------------------------------------------------------
	//  �ϐ���`
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