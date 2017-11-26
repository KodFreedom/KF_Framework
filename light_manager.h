//--------------------------------------------------------------------------------
//
//　LightManager.h
//	Author : Xu Wenjie
//	Date   : 2017-11-02
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  列挙型定義
//--------------------------------------------------------------------------------
enum LightType
{
	LT_Point = 1,
	LT_Spot = 2,
	LT_Directional = 3,
};

//--------------------------------------------------------------------------------
//  クラス
//--------------------------------------------------------------------------------
class LightManager
{
public:
	//--------------------------------------------------------------------------------
	//  関数宣言
	//--------------------------------------------------------------------------------
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	static void	Create(const LPDIRECT3DDEVICE9 device)
	{
		if (instance) return;
		instance = new LightManager;
		instance->device = device;
	}
#endif
	static void Release(void) { SAFE_UNINIT(instance); }
	static auto Instance(void) { return instance; }

	int	 CreateLight(const LightType& type = LT_Directional,
		const Color& diffuse = Color::White, const Color& specular = Color::Black, const Color& ambient = Color::Gray,
		const Vector3& position = Vector3::Zero, const Vector3& direction = Vector3::Down,
		const float& cutoffRange = 0.0f, const float& falloff = 0.0f, const float& constantAttenuation = 0.0f,
		const float& linearAttenuation = 0.0f, const float& quadraticAttenuation = 0.0f,
		const float& theta = 0.0f, const float& phi = 0.0f);
	void ReleaseLight(const int lightID);
	void ReleaseAll(void);

private:
	//--------------------------------------------------------------------------------
	//  構造体
	//--------------------------------------------------------------------------------
	struct Light
	{
		Light(const LightType& type,
			const Color& diffuse, const Color& specular, const Color& ambient,
			const Vector3& position, const Vector3& direction,
			const float& cutoffRange, const float& falloff, const float& constantAttenuation,
			const float& linearAttenuation, const float& quadraticAttenuation,
			const float& theta, const float& phi)
			: Type(type), Diffuse(diffuse), Specular(specular), Ambient(ambient)
			, Position(position), Direction(direction), CutoffRange(cutoffRange)
			, Falloff(falloff), ConstantAttenuation(constantAttenuation)
			, LinearAttenuation(linearAttenuation), QuadraticAttenuation(quadraticAttenuation)
			, Theta(theta), Phi(phi){}
		// キャスト
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
		operator D3DLIGHT9() const;
#endif

		// 変数
		LightType	Type;
		Color		Diffuse;
		Color		Specular;
		Color		Ambient;
		Vector3		Position;
		Vector3		Direction;
		float       CutoffRange;
		float       Falloff;
		float       ConstantAttenuation;
		float       LinearAttenuation;
		float       QuadraticAttenuation;
		float       Theta;
		float       Phi;
	};

	//--------------------------------------------------------------------------------
	//  関数宣言
	//--------------------------------------------------------------------------------
	LightManager() { lights.clear(); }
	~LightManager() {}
	void uninit(void);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	static LightManager*		instance;
	unordered_map<int, Light>	lights;
#if defined(USING_DIRECTX) && (DIRECTX_VERSION == 9)
	LPDIRECT3DDEVICE9			device;
#endif
};