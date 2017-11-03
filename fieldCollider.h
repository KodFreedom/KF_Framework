//--------------------------------------------------------------------------------
//	fieldCollider�R���|�l���g
//�@fieldCollider.h
//	Author : Xu Wenjie
//	Date   : 2017-06-05
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "collider.h"

//--------------------------------------------------------------------------------
//  �\���̒�`
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
//  �N���X�錾
//--------------------------------------------------------------------------------
class FieldCollider : public Collider
{
public:
	//--------------------------------------------------------------------------------
	//  �֐���`
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
	//  �֐���`
	//--------------------------------------------------------------------------------
	void			load(const string& fieldName);
	
	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	int				blockXNumber;
	int				blockZNumber;
	Vector2			blockSize;
	vector<Vector3>	vertexes;
};