//--------------------------------------------------------------------------------
//
//　gameObjManager.h
//	Author : Xu Wenjie
//	Date   : 2017-5-10
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _GAME_OBJECT_MANAGER_H_
#define _GAME_OBJECT_MANAGER_H_

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  定数定義
//--------------------------------------------------------------------------------
#define GOM CGameObjectManager	//GameObjectManagerの略称

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class CGameObject;

//--------------------------------------------------------------------------------
//  クラス宣言
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