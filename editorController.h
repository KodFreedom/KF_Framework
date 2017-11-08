//--------------------------------------------------------------------------------
//	�G�f�B�^�r�w�C�r�A�R���|�l���g
//�@EditorController.h
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
class FieldEditor;
class ModelEditor;

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class EditorController : public Behavior
{
public:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	EditorController(GameObject* const owner);
	~EditorController() {}

	bool	Init(void) override;
	void	Uninit(void) override;
	void	Update(void) override;
	void	LateUpdate(void) override {}

	void	SetFieldEditor(GameObject* fieldEditorObject);
	void	SetModelEditor(ModelEditor* value) { modelEditor = value; }

private:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	void	save(void);
	void	showMainWindow(void);
	void	showPositonWindow(void);

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	bool			isAutoHeight;
	FieldEditor*	fieldEditor;
	ModelEditor*	modelEditor;
	float			moveSpeed;
};
#endif // _DEBUG