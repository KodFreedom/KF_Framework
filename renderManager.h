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
#include "renderState.h"

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------
class MeshRenderer;

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class RenderManager
{
public:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	static auto	Create(void)
	{
		if (instance) return instance;
		instance = new RenderManager;
		return instance;
	}
	static void Release(void) { SAFE_UNINIT(instance); }
	static auto Instance(void) { return instance; }

	void Update(void);
	void Render(void);
	void Clear(void);
	void Register(MeshRenderer* renderer);

private:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	RenderManager();
	~RenderManager() {}
	void uninit(void) { Clear(); }

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	union
	{
		list<MeshRenderer*> renderersArray[Alpha::Max * Fog::Max * FillMode::Max * Synthesis::Max * CullMode::Max * Lighting::Max];
		list<MeshRenderer*> renderersArrays[Alpha::Max][Fog::Max][FillMode::Max][Synthesis::Max][CullMode::Max][Lighting::Max];
	};
	static RenderManager*	instance;
};