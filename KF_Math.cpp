//--------------------------------------------------------------------------------
//　kf_math.cpp
//  classes and methods for math
//	演算用のクラスとメソッド
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#include "kf_math.h"
using namespace kodfreedom;

//--------------------------------------------------------------------------------
//  constant variables / 定数
//--------------------------------------------------------------------------------
const Vector2 Vector2::kZero = Vector2(0.0f);
const Vector2 Vector2::kOne = Vector2(1.0f);

const Vector3 Vector3::kZero = Vector3(0.0f);				
const Vector3 Vector3::kOne = Vector3(1.0f);				
const Vector3 Vector3::kUp = Vector3(0.0f, 1.0f, 0.0f);	
const Vector3 Vector3::kDown = Vector3(0.0f, -1.0f, 0.0f);	
const Vector3 Vector3::kLeft = Vector3(-1.0f, 0.0f, 0.0f);	
const Vector3 Vector3::kRight = Vector3(1.0f, 0.0f, 0.0f);	
const Vector3 Vector3::kForward = Vector3(0.0f, 0.0f, 1.0f);
const Vector3 Vector3::kBack = Vector3(0.0f, 0.0f, -1.0f);	
const Vector3 Vector3::kAxisX = Vector3::kRight;
const Vector3 Vector3::kAxisY = Vector3::kUp;
const Vector3 Vector3::kAxisZ = Vector3::kForward;

const Vector4 Vector4::kZero = Vector4(0.0f);
const Vector4 Vector4::kOne = Vector4(1.0f);

const Matrix44 Matrix44::kZero = Matrix44(0.0f);
const Matrix44 Matrix44::kIdentity = Matrix44();

const Quaternion Quaternion::kIdentity = Quaternion(0.0f, 0.0f, 0.0f, 1.0f);

const Color Color::kWhite = Color(1.0f, 1.0f, 1.0f, 1.0f);
const Color Color::kBlack = Color(0.0f, 0.0f, 0.0f, 1.0f);
const Color Color::kGray = Color(0.2f, 0.2f, 0.2f, 1.0f);
const Color Color::kRed = Color(1.0f, 0.0f, 0.0f, 1.0f);
const Color Color::kBlue = Color(0.0f, 0.0f, 1.0f, 1.0f);
const Color Color::kGreen = Color(0.0f, 1.0f, 0.0f, 1.0f);