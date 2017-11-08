//--------------------------------------------------------------------------------
//	レンダーステート
//　renderState.h
//	Author : Xu Wenjie
//	Date   : 2017-05-22
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  列挙型定義
//--------------------------------------------------------------------------------
/*enum RenderState
{
	RS_Lighting = 0,
	RS_CullMode,
	RS_Synthesis,
	RS_FillMode,
	RS_Fog,
	RS_Alpha,
	RS_Max
};*/

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
