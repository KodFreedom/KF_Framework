//--------------------------------------------------------------------------------
//	�`��R���|�l���g
//�@drawComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-05-18	
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "component.h"
#include "renderState.h"

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �`��R���|�l���g�N���X
//--------------------------------------------------------------------------------
class CDrawComponent : public CComponent
{
public:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	CDrawComponent(CGameObject* const pGameObj) : CComponent(pGameObj)
		,m_pRenderState(&s_nullRenderState), m_usMatID(0)
	{
		m_strTexName.clear();
	}

	~CDrawComponent() {}

	virtual bool	Init(void) override { return true; }
	virtual void	Uninit(void) override;
	virtual void	Draw(void) = 0;

	//Set�֐�
	void			SetRenderState(CRenderState* const pRenderState) 
	{
		if (!pRenderState)
		{
			m_pRenderState = &s_nullRenderState;
			return;
		}

		m_pRenderState = pRenderState; 
	}
	void			SetTexName(const string& strTexName);
	void			SetMatID(const unsigned short& usID) { m_usMatID = usID; }

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	static CLightOffRenderState	s_lightOffRenderState;
	static CNullRenderState		s_nullRenderState;

protected:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	CDrawComponent() : CComponent() {}

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	string			m_strTexName;	//�e�N�X�`��
	unsigned short	m_usMatID;		//�}�e���A��
	CRenderState*	m_pRenderState;	//�����_�[�X�e�[�g
};

//--------------------------------------------------------------------------------
//  �k���`��R���|�l���g�N���X
//--------------------------------------------------------------------------------
class CNullDrawComponent : public CDrawComponent
{
public:
	CNullDrawComponent() : CDrawComponent() {}
	~CNullDrawComponent() {}

	bool	Init(void) override { return true; }
	void	Uninit(void) override {}
	void	Release(void) override {}
	void	Draw(void) override {}
};