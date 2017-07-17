//--------------------------------------------------------------------------------
//
//　modelManager.h
//	Author : Xu Wenjie
//	Date   : 2017-1-24
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _MODEL_MANAGER_H_
#define _MODEL_MANAGER_H_

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  定数定義
//--------------------------------------------------------------------------------
#define CMOM CModelManager //CModelManagerの略称

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class CModel;

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class CModelManager
{
public:
	enum MODEL_TYPE
	{
		XFILE,
		XFILE_MOTION,
		MESH,
	};

	enum MODEL_NAME
	{
		MODEL_NONE,
		MODEL_CUBE,
		MODEL_PLAYER,
		//MODEL_ENEMY,
		MODEL_MAX
	};

	CModelManager();
	~CModelManager() {}

	void	Init(void);
	void	LoadAll(void);
	void	UnloadAll(void);
	CModel	*GetModel(const MODEL_NAME &modelName);

	CModel* GetModel(const string& modelPath) { return m_umapModel.at(modelPath); }
	void	BindModel(const string& modelPath);

private:
	struct MODEL_INFO
	{
		MODEL_TYPE	type;
		LPCSTR		path;
	};

	void Load(const MODEL_NAME &modelBegin, const MODEL_NAME &modelEnd);

	vector<CModel*>	m_vectorModel;
	static MODEL_INFO		m_apModelInfo[MODEL_MAX];

	unordered_map<string, CModel*> m_umapModel;
};

#endif