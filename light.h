//--------------------------------------------------------------------------------
//　light.h
//	ライトクラス
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include "main.h"

//--------------------------------------------------------------------------------
//  列挙型定義
//--------------------------------------------------------------------------------
enum LightType
{
	kShadowMapLight = 0,
};

//--------------------------------------------------------------------------------
//  ライト基底クラス
//--------------------------------------------------------------------------------
class Light
{
public:
	const LightType type_; // Type of light source
	Color diffuse_; // Diffuse color of light
	Color specular_; // Specular color of light
	Color ambient_; // Ambient color of light

	virtual void Set(void) = 0;

protected:
	Light(const LightType& type, const Color& diffuse, const Color& ambient, const Color& specular)
		: type_(type), diffuse_(diffuse), specular_(specular), ambient_(ambient) {}
	Light(const Light& value) : type_(kShadowMapLight) {}
	Light& operator=(const Light& value) {}
	~Light() {}
};

class ShadowMapLight : public Light
{
#ifdef _DEBUG
	friend class DebugObserver;
#endif // _DEBUG
public:
	ShadowMapLight()
		: Light(kShadowMapLight, Color::kWhite, Color::kGray, Color::kWhite)
		, position_(Vector3(35.0f, 50.0f, -35.0f)), look_at_(Vector3::kZero)
		, near_(5.0f), far_(100.0f) {}

	void Set(void);
	const Vector3& GetDirection(void) const { return direction_; }
	const Matrix44& GetView(void) const { return view_; }
	const Matrix44& GetProjection(void) const { return projection_; }

private:
	Vector3	position_;
	Vector3 look_at_;
	Vector3 direction_;
	float near_;
	float far_;
	Matrix44 view_;
	Matrix44 projection_;
};