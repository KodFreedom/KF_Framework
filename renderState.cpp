//--------------------------------------------------------------------------------
//	レンダーステート
//　renderState.cpp
//	Author : Xu Wenjie
//	Date   : 2017-05-22
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "renderState.h"
#include "renderSystem.h"

//--------------------------------------------------------------------------------
//
//  DefaultRenderState
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  Set
//--------------------------------------------------------------------------------
void DefaultRenderState::Set(void)
{
}

//--------------------------------------------------------------------------------
//  Reset
//--------------------------------------------------------------------------------
void DefaultRenderState::Reset(void)
{
}

//--------------------------------------------------------------------------------
//
//  NoLightNoFog
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  Set
//--------------------------------------------------------------------------------
void NoLightNoFog::Set(void)
{
	RenderSystem::Instance()->SetRenderState(Lighting_Off);
	RenderSystem::Instance()->SetRenderState(Fog_Off);
}

//--------------------------------------------------------------------------------
//  Reset
//--------------------------------------------------------------------------------
void NoLightNoFog::Reset(void)
{
	RenderSystem::Instance()->SetRenderState(Lighting_On);
	RenderSystem::Instance()->SetRenderState(Fog_On);
}

//--------------------------------------------------------------------------------
//
//  NoCullMode
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  Set
//--------------------------------------------------------------------------------
void NoCullMode::Set(void)
{
	RenderSystem::Instance()->SetRenderState(Cull_None);
}

//--------------------------------------------------------------------------------
//  Reset
//--------------------------------------------------------------------------------
void NoCullMode::Reset(void)
{
	RenderSystem::Instance()->SetRenderState(Cull_CCW);
}

//--------------------------------------------------------------------------------
//
//  NoLightNoFogNoCullMode
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  Set
//--------------------------------------------------------------------------------
void NoLightNoFogNoCullMode::Set(void)
{
	RenderSystem::Instance()->SetRenderState(Cull_None);
	RenderSystem::Instance()->SetRenderState(Lighting_Off);
	RenderSystem::Instance()->SetRenderState(Fog_Off);
}

//--------------------------------------------------------------------------------
//  Reset
//--------------------------------------------------------------------------------
void NoLightNoFogNoCullMode::Reset(void)
{
	RenderSystem::Instance()->SetRenderState(Cull_CCW);
	RenderSystem::Instance()->SetRenderState(Lighting_On);
	RenderSystem::Instance()->SetRenderState(Fog_On);
}