//--------------------------------------------------------------------------------
//	fieldColliderコンポネント
//　fieldCollider.h
//	Author : Xu Wenjie
//	Date   : 2017-06-05
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "collider.h"

//--------------------------------------------------------------------------------
//  構造体定義
//--------------------------------------------------------------------------------
struct PolygonInfo
{
	Vector3 LeftUp;
	Vector3 RightDown;
	Vector3 Side;
	Vector3 SurfaceNormal;
};

struct VertexesInRange
{
	int				VertexesNumberX;
	int				VertexesNumberZ;
	vector<Vector3> vertexes;
};

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class FieldCollider : public Collider
{
public:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	FieldCollider(GameObject* const owner, const string& fieldName)
		: Collider(owner, Field, Static)
		, blockXNumber(0)
		, blockZNumber(0)
		, blockSize(Vector2::Zero)
	{
		vertexes.clear();
		load(fieldName);
	}
	~FieldCollider() {}

	bool				Init(void) override { return true; }
	void				Uninit(void) override
	{
		vertexes.clear();
		Collider::Uninit();
	}
	PolygonInfo*		GetPolygonAt(const Vector3& point) const;
	VertexesInRange*	GetVertexesInRange(const Vector3& begin, const Vector3& end);

private:
	//--------------------------------------------------------------------------------
	//  関数定義
	//--------------------------------------------------------------------------------
	void			load(const string& fieldName);
	
	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	int				blockXNumber;
	int				blockZNumber;
	Vector2			blockSize;
	vector<Vector3>	vertexes;
};