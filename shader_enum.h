//--------------------------------------------------------------------------------
//　shader_enum.h
//	シェーダー用列挙型
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once

// ライトモード
enum LightMode
{
	kLightOff = 0,
	kLightOn,
	kLightMax
};

// カリングモード
enum CullMode
{
	KCullNone = 0,
	kCw,
	kCcw,
	kCullMax
};

// 合成方式
enum Synthesis
{
	kMultiplication = 0,
	kAdditive,
	kSubtractive,
	kSynthesisMax
};

// フィルモード
enum FillMode
{
	kPoint = 0,
	kWireFrame,
	kSolid,
	kFillMax
};

// フォグモード
enum FogMode
{
	kFogOff = 0,
	kFogOn,
	kFogMax
};

// αモード
enum AlphaMode
{
	kAlphaNone = 0,
	kAlphaTest,
	kDepthSort,
	kAlphaMax
};

// シェーダータイプ
enum ShaderType
{
	kDefaultShader = 0, // LightOn_CCW_Multi_Solid_FogOn
	kNoLightNoFog, // LightOff_CCW_Multi_Solid_FogOff
	kCullNone, // LightOn_None_Multi_Solid_FogOn
	kDefaultSkinShader, // SkinShader
	kJuggernautMeshShader,
	kJuggernautSkinShader,
    kDefault2dShader,
	kShaderMax
};

enum ShadowMapShaderType
{
	kBasicShadowMapShader = 0,
	kBasicSkinShadowMapShader,
	kShadowMapShaderMax
};