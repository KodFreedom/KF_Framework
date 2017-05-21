//--------------------------------------------------------------------------------
//	�`��R���|�l���g
//�@drawComponent.h
//	Author : Xu Wenjie
//	Date   : 2017-05-18	
//--------------------------------------------------------------------------------
#ifndef _DRAW_COMPONENT_H_
#define _DRAW_COMPONENT_H_

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "component.h"
#include "renderState.h"

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------
class CGameObject;
class CMeshComponent;

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
	CDrawComponent() 
		: m_pRenderState(&s_nullRenderState)
	{}

	~CDrawComponent() {}

	virtual KFRESULT	Init(void) override { return KF_SUCCEEDED; }
	virtual void		Uninit(void) override {};
	virtual void		Draw(const CGameObject &gameObj, const CMeshComponent &meshComponent) = 0;
	virtual void		ReceiveMsg(const MESSAGE &msg) override {}

	//Set�֐�
	void				SetRenderState(CRenderState* pRenderState) { m_pRenderState = pRenderState; }

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	static CLightOffRenderState	s_lightOffRenderState;
	static CNullRenderState		s_nullRenderState;

protected:
	CRenderState*		m_pRenderState;
};

//--------------------------------------------------------------------------------
//  �k���`��R���|�l���g�N���X
//--------------------------------------------------------------------------------
class CNullDrawComponent : public CDrawComponent
{
public:
	CNullDrawComponent() {}
	~CNullDrawComponent() {}

	KFRESULT	Init(void) override { return KF_SUCCEEDED; }
	void		Uninit(void) override {}
	void		Release(void) override {}
	void		Draw(const CGameObject &gameObj, const CMeshComponent &meshComponent) override {}
	void		ReceiveMsg(const MESSAGE &msg) override {}
};

#endif
