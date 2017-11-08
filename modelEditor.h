//--------------------------------------------------------------------------------
//	���f���G�f�B�^�r�w�C�r�A�R���|�l���g
//�@ModelEditor.h
//	Author : Xu Wenjie
//	Date   : 2017-08-23
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
class Transform;

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class ModelEditor : public Behavior
{
public:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	ModelEditor(GameObject* const owner);
	~ModelEditor() {}

	bool	Init(void) override;
	void	Uninit(void) override;
	void	Update(void) override;
	void	LateUpdate(void) override;

	//Get�֐�
	bool	GetActive(void) const { return isActive; }

	//Set�֐�
	void	SetActive(const bool& value) { isActive = value; }
	void	SetPosition(const Vector3& position);

	//Save
	void	SaveAs(const string& fileName);

private:
	//--------------------------------------------------------------------------------
	//  �񋓌^��`
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
	//  �\���̒�`
	//--------------------------------------------------------------------------------
	struct Info
	{
		Info() : MyTransform(nullptr), Rotation(Vector3::Zero) {}
		Transform* MyTransform;
		Vector3	   Rotation;
	};

	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	void create(void);
	void showMainWindow(void);
	void showTypeListBox(void);
	void showCreatedList(void);

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	string			modelNames[ModelType::Max];
	Info			demoModelInfos[ModelType::Max];
	list<Info>		createdModelInfos[ModelType::Max];
	ModelType		currentModelType;
	bool			isActive;
	bool			isShowCreatedList;
};
#endif // _DEBUG