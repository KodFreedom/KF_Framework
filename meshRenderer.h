//--------------------------------------------------------------------------------
//	���b�V���`��R���|�l���g
//�@MeshRenderer.h
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
//  �N���X�錾
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �`��R���|�l���g�N���X
//--------------------------------------------------------------------------------
class MeshRenderer : public Component
{
	friend class RenderManager;
public:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	MeshRenderer(GameObject* const owner)
		: Component(owner)
		, materialID(0)
		, lighting(Lighting_On)
		, cullMode(Cull_CCW)
		, synthesis(S_Multiplication)
		, fillMode(Fill_Solid)
		, alpha(A_None)
		, fog(Fog_On)
	{
		textureName.clear();
	}

	~MeshRenderer() {}

	virtual bool	Init(void) override;
	virtual void	Uninit(void) override;
	virtual void	Update(void);
	virtual void	Render(void) = 0;

	//Set�֐�
	void Set(const string& texture);
	void Set(const unsigned short& usID) { materialID = usID; }
	void Set(const Lighting& value) { lighting = value; }
	void Set(const CullMode& value) { cullMode = value; }
	void Set(const Synthesis& value) { synthesis = value; }
	void Set(const FillMode& value) { fillMode = value; }
	void Set(const Alpha& value) { alpha = value; }
	void Set(const Fog& value) { fog = value; }

protected:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	MeshRenderer() : Component() {}

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	string			textureName;
	unsigned short	materialID;
	Lighting		lighting;
	CullMode		cullMode;
	Synthesis		synthesis;
	FillMode		fillMode;
	Alpha			alpha;
	Fog				fog;
};