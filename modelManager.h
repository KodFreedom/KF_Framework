//--------------------------------------------------------------------------------
//
//�@modelManager.h
//	Author : Xu Wenjie
//	Date   : 2017-1-24
//--------------------------------------------------------------------------------
//  Update : 
//	
//--------------------------------------------------------------------------------
#ifndef _MODEL_MANAGER_H_
#define _MODEL_MANAGER_H_

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------
#define CMOM CModelManager //CModelManager�̗���

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------
class CModel;

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class CModelManager
{
public:
	enum MODEL_TYPE
	{
		XFILE,
		XFILE_MOTION,
	};

	enum MODEL_NAME
	{
		MODEL_NONE,
		MODEL_CUBE,
		MODEL_ROBOT,
		MODEL_PLAYER,
		MODEL_MAX
	};

	CModelManager();
	~CModelManager() {}

	void	Init(void);
	void	LoadAll(void);
	void	UnloadAll(void);
	CModel	*GetModel(const MODEL_NAME &modelName);
private:
	struct MODEL_INFO
	{
		MODEL_TYPE	type;
		LPCSTR		path;
	};

	void Load(const MODEL_NAME &modelBegin, const MODEL_NAME &modelEnd);

	std::vector<CModel*>	m_vectorModel;
	static MODEL_INFO		m_apModelInfo[MODEL_MAX];
};

#endif