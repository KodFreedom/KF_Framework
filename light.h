//--------------------------------------------------------------------------------
//�@light.h
//  ���C�g�N���X
//  Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include "kf_math.h"
using namespace kodfreedom;

//--------------------------------------------------------------------------------
//  �񋓌^��`
//--------------------------------------------------------------------------------
enum LightType
{
    kDirectionalLight = 0,
};

//--------------------------------------------------------------------------------
//  ���C�g���N���X
//--------------------------------------------------------------------------------
class Light
{
public:
    const LightType type_; // Type of light source
    Color diffuse_; // Diffuse color of light
    Color specular_; // Specular color of light
    Color ambient_; // Ambient color of light
    Vector3 direction_;

protected:
    Light(const LightType& type
        , const Color& diffuse, const Color& ambient, const Color& specular
        , const Vector3& direction)
        : type_(type), diffuse_(diffuse), specular_(specular), ambient_(ambient), direction_(direction) {}
    Light(const Light& value) : type_(kDirectionalLight) {}
    Light& operator=(const Light& value) {}
    ~Light() {}
};

//--------------------------------------------------------------------------------
//  �f�B���N�V���i�����C�g
//--------------------------------------------------------------------------------
class DirectionalLight : public Light
{
public:
    DirectionalLight(const Vector3& direction);
};