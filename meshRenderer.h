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
#include "rendererManager.h"

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class MeshRenderer : public Component
{
public:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	MeshRenderer(GameObject* const owner)
		: Component(owner)
		, currentPriority(RP_Default)
		, currentState(RS_Default)
	{
		meshName.clear();
		textureName.clear();
		materialName.clear();
	}

	~MeshRenderer() {}

	virtual bool	Init(void) override;
	virtual void	Uninit(void) override;
	virtual void	Update(void);

	//Set�֐�
	void SetMeshName(const string& name);
	void SetTextureName(const string& name);
	void SetMaterialName(const string& name);
	void SetRenderPriority(const RenderPriority& value) { currentPriority = value; }
	void SetRenderState(const RenderStateType& value) { currentState = value; }

	//Get�֐�
	const auto& GetMeshName(void) const { return meshName; }
	const auto& GetTextureName(void) const { return textureName; }
	const auto& GetMaterialName(void) const { return materialName; }
	const auto& GetRenderPriority(void) const { return currentPriority; }
	const auto& GetRenderState(void) const { return currentState; }

protected:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	MeshRenderer() : Component() {}

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	string			meshName;
	string			textureName;
	string			materialName;
	RenderPriority	currentPriority;
	RenderStateType	currentState;
};