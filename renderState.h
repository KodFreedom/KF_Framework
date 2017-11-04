//--------------------------------------------------------------------------------
//	�����_�[�X�e�[�g
//�@renderState.h
//	Author : Xu Wenjie
//	Date   : 2017-05-22
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �񋓌^��`
//--------------------------------------------------------------------------------
enum RenderState
{
	RS_Lighting = 0,
	RS_CullMode,
	RS_Synthesis,
	RS_FillMode,
	RS_Fog,
	RS_Alpha,
	RS_Max
};

enum Lighting
{
	Off = 0,
	On,
	Max
};

enum CullMode
{
	None = 0,
	CW,
	CCW,
	Max
};

enum Synthesis
{
	Multiplication = 0,
	Additive,
	Subtractive,
	Max
};

enum FillMode
{
	Point = 0,
	WireFrame,
	Solid,
	Max
};

enum Fog
{
	Off = 0,
	On,
	Max
};

enum Alpha
{
	None = 0,
	AlphaTest,
	ZSort,
	Max
};
