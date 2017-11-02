//--------------------------------------------------------------------------------
//	�`��R���|�l���g
//�@renderComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-05-18	
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "component.h"
#include "renderManager.h"

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �`��R���|�l���g�N���X
//--------------------------------------------------------------------------------
class CRenderComponent : public Component
{
public:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	CRenderComponent(GameObject* const pGameObj)
		: Component(pGameObj)
		, m_usMatID(0)
		, m_renderPriority(RP_3D)
		, m_renderState(RS_LIGHTON_CULLFACEON_MUL)
	{
		m_strTexName.clear();
	}

	~CRenderComponent() {}

	virtual bool	Init(void) override;
	virtual void	Uninit(void) override;
	virtual void	Update(void);
	virtual void	Render(void) = 0;

	//Set�֐�
	void			SetTexName(const string& strTexName);
	void			SetMatID(const unsigned short& usID) { m_usMatID = usID; }
	void			SetRenderPriority(const RenderPriority& rp) { m_renderPriority = rp; }
	void			SetRenderState(const RenderState& rs) { m_renderState = rs; }

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------

protected:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	CRenderComponent() : Component() {}

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	string			m_strTexName;		//�e�N�X�`��
	unsigned short	m_usMatID;			//�}�e���A��
	RenderPriority	m_renderPriority;
	RenderState	m_renderState;
};

//--------------------------------------------------------------------------------
//  �k���`��R���|�l���g�N���X
//--------------------------------------------------------------------------------
class CNullRenderComponent : public CRenderComponent
{
public:
	CNullRenderComponent() : CRenderComponent() {}
	~CNullRenderComponent() {}

	bool	Init(void) override { return true; }
	void	Uninit(void) override {}
	void	Release(void) override {}
	void	Update(void) override {}
	void	Render(void) override {}
};