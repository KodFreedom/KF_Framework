//--------------------------------------------------------------------------------
//	レンダーステート
//　renderState.h
//	Author : Xu Wenjie
//	Date   : 2017-05-22
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  列挙型定義
//--------------------------------------------------------------------------------
enum Lighting
{
	Lighting_Off = 0,
	Lighting_On,
	Lighting_Max
};

enum CullMode
{
	Cull_None = 0,
	Cull_CW,
	Cull_CCW,
	Cull_Max
};

enum Synthesis
{
	S_Multiplication = 0,
	S_Additive,
	S_Subtractive,
	S_Max
};

enum FillMode
{
	Fill_Point = 0,
	Fill_WireFrame,
	Fill_Solid,
	Fill_Max
};

enum Fog
{
	Fog_Off = 0,
	Fog_On,
	Fog_Max
};

enum Alpha
{
	A_None = 0,
	A_AlphaTest,
	A_ZSort,
	A_Max
};

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
class RenderState
{
public:
	RenderState() {}
	~RenderState() {}
	virtual void Set(void) = 0;
	virtual void Reset(void) = 0;
};

class DefaultRenderState : public RenderState
{
public:
	DefaultRenderState() : RenderState() {}
	~DefaultRenderState() {}
	void Set(void) override;
	void Reset(void) override;
};

class NoLightNoFog : public RenderState
{
public:
	NoLightNoFog() : RenderState() {}
	~NoLightNoFog() {}
	void Set(void) override;
	void Reset(void) override;
};

class NoCullMode : public RenderState
{
public:
	NoCullMode() : RenderState() {}
	~NoCullMode() {}
	void Set(void) override;
	void Reset(void) override;
};

class NoLightNoFogNoCullMode : public RenderState
{
public:
	NoLightNoFogNoCullMode() : RenderState() {}
	~NoLightNoFogNoCullMode() {}
	void Set(void) override;
	void Reset(void) override;
};