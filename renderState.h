//--------------------------------------------------------------------------------
//	�����_�[�X�e�[�g�N���X
//�@renderState.h
//	Author : Xu Wenjie
//	Date   : 2017-05-22
//--------------------------------------------------------------------------------
#ifndef _RENDERSTATE_H_
#define _RENDERSTATE_H_

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �X�[�p�[�N���X
//--------------------------------------------------------------------------------
class CRenderState
{
public:
	CRenderState() {}
	~CRenderState() {}

	virtual void	SetRenderState(void) = 0;
	virtual void	ResetRenderState(void) = 0;
};

//--------------------------------------------------------------------------------
//  LightOff�N���X
//--------------------------------------------------------------------------------
class CLightOffRenderState : public CRenderState
{
public:
	CLightOffRenderState() : CRenderState() {}
	~CLightOffRenderState() {}

	void	SetRenderState(void) override;
	void	ResetRenderState(void) override;
};

//--------------------------------------------------------------------------------
//  �k���N���X
//--------------------------------------------------------------------------------
class CNullRenderState : public CRenderState
{
public:
	CNullRenderState() : CRenderState() {}
	~CNullRenderState() {}

	void	SetRenderState(void) override {}
	void	ResetRenderState(void) override {}
};
#endif