//--------------------------------------------------------------------------------
//	�`��p�}�l�[�W��
//�@renderManager.h
//	Author : Xu Wenjie
//	Date   : 2017-09-20
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------
class CRenderComponent;
class CRenderState;

//--------------------------------------------------------------------------------
//  �񋓌^��`
//--------------------------------------------------------------------------------
enum RenderPriority
{//�����_�[�D��x
	RP_3D = 0,
	RP_3D_ALPHATEST,
	RP_3D_ZSORT,
	RP_MAX
};

enum RenderState
{//�����_�[�X�e�[�g
	RS_LIGHTOFF_CULLFACEON_MUL = 0,	//���C�g�I�t�A���ʕ`��A��Z����
	RS_LIGHTOFF_CULLFACEOFF_MUL,	//���C�g�I�t�A���ʕ`��A��Z����
	RS_LIGHTON_CULLFACEON_MUL,		//���C�g�I���A�Жʕ`��A��Z����
	RS_LIGHTON_CULLFACEOFF_MUL,		//���C�g�I���A�Жʕ`��A��Z����
	RS_MAX
};

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class CRenderManager
{
public:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	CRenderManager();
	~CRenderManager() {}

	static auto Create(void)
	{
		auto pRenderManager = new CRenderManager();
		pRenderManager->init();
		return pRenderManager;
	}
	void		Update(void);
	void		Render(void);
	void		Release(void)
	{
		uninit();
		delete this;
	}
	void		Clear(void);
	void		Register(CRenderComponent* pRender, const RenderPriority& rp, const RenderState& rs)
	{
		m_apRenderComponents[rp][rs].push_back(pRender);
	}

private:
	//--------------------------------------------------------------------------------
	//  �\���̒�`
	//--------------------------------------------------------------------------------
	typedef list<CRenderComponent*> ListDraw;

	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	void		init(void);
	void		uninit(void);
	void		setRenderState(const RenderPriority& rp, const RenderState& rs);
	void		resetRenderState(const RenderPriority& rp, const RenderState& rs);

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	ListDraw		m_apRenderComponents[RP_MAX][RS_MAX];
	CRenderState*	m_apRenderState[RS_MAX];
};