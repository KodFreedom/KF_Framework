//--------------------------------------------------------------------------------
//�@shader_enum.h
//	�V�F�[�_�[�p�񋓌^
//	Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once

// ���C�g���[�h
enum LightMode
{
	kLightOff = 0,
	kLightOn,
	kLightMax
};

// �J�����O���[�h
enum CullMode
{
	KCullNone = 0,
	kCw,
	kCcw,
	kCullMax
};

// ��������
enum Synthesis
{
	kMultiplication = 0,
	kAdditive,
	kSubtractive,
	kSynthesisMax
};

// �t�B�����[�h
enum FillMode
{
	kPoint = 0,
	kWireFrame,
	kSolid,
	kFillMax
};

// �t�H�O���[�h
enum FogMode
{
	kFogOff = 0,
	kFogOn,
	kFogMax
};

// �����[�h
enum AlphaMode
{
	kAlphaNone = 0,
	kAlphaTest,
	kDepthSort,
	kAlphaMax
};

// �V�F�[�_�[�^�C�v
enum ShaderType
{
	kDefaultShader = 0,
	kShaderMax
};