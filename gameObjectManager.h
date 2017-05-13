//--------------------------------------------------------------------------------
//
//�@gameObjManager.h
//	Author : Xu Wenjie
//	Date   : 2017-5-10
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _GAME_OBJECT_MANAGER_H_
#define _GAME_OBJECT_MANAGER_H_

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------
#define GOM CGameObjectManager	//GameObjectManager�̗���

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------
class CGameObject;

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class CGameObjectManager
{
public:
	enum PRIORITY
	{
		PRI_3D,
		PRI_3D_ALPHA,
		PRI_2D,
		PRI_MAX
	};

	CGameObjectManager();
	~CGameObjectManager() {}

	void	Init(void);
	void	Uninit(void);

	void	ReleaseAll(void);
	void	UpdateAll(void);
	void	LateUpdateAll(void);
	void	DrawAll(void);

	int		SaveGameObj(const PRIORITY &pri, CGameObject *pGameObj);
	void	ReleaseGameObj(const PRIORITY &pri, const int &nID);

private:
	std::vector<CGameObject*>	m_avectorGameObj[PRI_MAX];
};

#endif