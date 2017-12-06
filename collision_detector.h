//--------------------------------------------------------------------------------
//	�R���W�����T�m��
//�@collisionDetector.h
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#pragma once
#include "main.h"

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------
class Rigidbody3D;
class Collider;
class BoxCollider;
class SphereCollider;
class AabbCollider;
class ObbCollider;
class FieldCollider;

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���W����
//--------------------------------------------------------------------------------
class Collision
{
public:
	Collision()
		: rigidbody_one(nullptr)
		, rigidbody_two(nullptr)
		, normal(Vector3::kZero)
		, point(Vector3::kZero)
		, penetration(0.0f) {}
	~Collision() {}

	Rigidbody3D* rigidbody_one;
	Rigidbody3D* rigidbody_two;
	Vector3      normal; // �Փ˖@��
	Vector3	     point; // �Փ˓_
	float        penetration; // �Փː[�x
};

//--------------------------------------------------------------------------------
//  �R���W�������
//--------------------------------------------------------------------------------
class CollisionInfo
{
public:
	CollisionInfo()
		: self(nullptr)
		, other(nullptr)
	{}
	~CollisionInfo()
	{
		collisions.clear();
	}
	Collider* self;
	Collider* other;
	list<Collision*> collisions;
};

//--------------------------------------------------------------------------------
//  ���C�Փˏ��(�ŋߓ_)
//--------------------------------------------------------------------------------
class RayHitInfo
{
public:
	RayHitInfo()
		: other(nullptr)
		, distance(0.0f)
		, normal(Vector3::kZero)
		, position(Vector3::kZero)
	{}
	~RayHitInfo() {}

	Collider* other;
	float     distance;
	Vector3   normal;
	Vector3   position;
};

//--------------------------------------------------------------------------------
//  �R���W�����v�Z�N���X
//--------------------------------------------------------------------------------
class CollisionDetector
{
public:
	//--------------------------------------------------------------------------------
	//  �萔��`
	//--------------------------------------------------------------------------------
	static constexpr float kMaxFieldSlope = 45.0f * kPi / 180.0f; // �t�B�[���h�o���ő�p�x
	static const float kMaxFieldSlopeCos; // �t�B�[���h�o���ő�p�x��cos�l
	static constexpr float kMinWallSlope = 60.0f * kPi / 180.0f; // �t�B�[���h���ǂƔF�߂�ŏ��p�x
	static const float kMinWallSlopeCos; // �t�B�[���h���ǂƔF�߂�ŏ��p�x��cos�l
	static constexpr float kMaxObbSlope = 60.0f * kPi / 180.0f; // Obb�o���ő�p�x
	static const float kMaxObbSlopeCos; // Obb�o���ő�p�x��cos�l

	//--------------------------------------------------------------------------------
	//	�֐����FDetect
	//  �֐������F�X�t�B�A�ƃX�t�B�A�̓����蔻��֐�
	//			�@����������R���W�������𕨗����Z�}�l�[�W���ɓo�^����
	//	�����F	sphereL�F�X�t�B�A�R���C�_�[
	//			sphereR�F�X�t�B�A�R���C�_�[
	//	�߂�l�F�Ȃ�
	//--------------------------------------------------------------------------------
	static void Detect(SphereCollider& sphere_left, SphereCollider& sphere_right);
	
	//--------------------------------------------------------------------------------
	//	�֐����FDetect
	//  �֐������F�X�t�B�A��AABB�̓����蔻��֐�
	//			�@����������R���W�������𕨗����Z�}�l�[�W���ɓo�^����
	//	�����F	sphere�F�X�t�B�A�R���C�_�[
	//			aabb�F��]�Ȃ��{�b�N�X�R���C�_�[
	//	�߂�l�F�Ȃ�
	//--------------------------------------------------------------------------------
	static void Detect(SphereCollider& sphere, AabbCollider& aabb);
	
	//--------------------------------------------------------------------------------
	//	�֐����FDetect
	//  �֐������F�X�t�B�A��OBB�̓����蔻��֐�
	//			�@����������R���W�������𕨗����Z�}�l�[�W���ɓo�^����
	//	�����F	sphere�F�X�t�B�A�R���C�_�[
	//			obb�F��]����{�b�N�X�R���C�_�[
	//	�߂�l�F�Ȃ�
	//--------------------------------------------------------------------------------
	static void Detect(SphereCollider& sphere, ObbCollider& obb);
	
	//--------------------------------------------------------------------------------
	//	�֐����FDetect
	//  �֐������FAABB��AABB�̓����蔻��֐�
	//			�@����������R���W�������𕨗����Z�}�l�[�W���ɓo�^����
	//	�����F	aabbL�F��]�Ȃ��{�b�N�X�R���C�_�[
	//			aabbR�F��]�Ȃ��{�b�N�X�R���C�_�[
	//	�߂�l�F�Ȃ�
	//--------------------------------------------------------------------------------
	static void Detect(AabbCollider& aabb_left, AabbCollider& aabb_right);
	
	//--------------------------------------------------------------------------------
	//	�֐����FDetect
	//  �֐������Fbox��box�̓����蔻��֐�(�����ꂩ���K��OBB)
	//			�@����������R���W�������𕨗����Z�}�l�[�W���ɓo�^����
	//	�����F	boxL�F�{�b�N�X�R���C�_�[
	//			boxR�F�{�b�N�X�R���C�_�[
	//	�߂�l�F�Ȃ�
	//--------------------------------------------------------------------------------
	static void Detect(BoxCollider& box_left, BoxCollider& box_right);
	
	//--------------------------------------------------------------------------------
	//	�֐����FDetect
	//  �֐������F�X�t�B�A�ƃt�B�[���h�̓����蔻��֐�
	//			�@����������R���W�������𕨗����Z�}�l�[�W���ɓo�^����
	//	�����F	sphere�F�X�t�B�A�R���C�_�[
	//			field�F�t�B�[���h�R���C�_�[
	//	�߂�l�F�Ȃ�
	//--------------------------------------------------------------------------------
	static void Detect(BoxCollider& box, FieldCollider& field);
	
	//--------------------------------------------------------------------------------
	//	�֐����FDetect
	//  �֐������FBox�ƃt�B�[���h�̓����蔻��֐�
	//			�@����������R���W�������𕨗����Z�}�l�[�W���ɓo�^����
	//	�����F	box�F�{�b�N�X�R���C�_�[
	//			field�F�t�B�[���h�R���C�_�[
	//	�߂�l�F�Ȃ�
	//--------------------------------------------------------------------------------
	static void Detect(SphereCollider& sphere, FieldCollider& field);
	
	//--------------------------------------------------------------------------------
	//	�֐����FDetect
	//  �֐������F���C�ƃ{�b�N�X�̓����蔻��
	//	�����F	ray�F���C
	//			distancetance�F���C�̒���
	//			box�F�{�b�N�X�R���C�_�[
	//	�߂�l�FRayHitInfo*
	//--------------------------------------------------------------------------------
	static RayHitInfo* Detect(const Ray& ray, const float& distance, BoxCollider& box);

	//--------------------------------------------------------------------------------
	//	�֐����FDetect
	//  �֐������F���C�ƃX�t�B�A�̓����蔻��
	//	�����F	ray�F���C
	//			distancetance�F���C�̒���
	//			sphere�F�X�t�B�A�R���C�_�[
	//	�߂�l�FRayHitInfo*
	//--------------------------------------------------------------------------------
	static RayHitInfo* Detect(const Ray& ray, const float& distance, SphereCollider& sphere);
	
	//--------------------------------------------------------------------------------
	//	�֐����FDetect
	//  �֐������F���C�ƃt�B�[���h�̓����蔻��
	//	�����F	ray�F���C
	//			distancetance�F���C�̒���
	//			field�F�t�B�[���h�R���C�_�[
	//	�߂�l�FRayHitInfo*
	//--------------------------------------------------------------------------------
	static RayHitInfo* Detect(const Ray& ray, const float& distance, FieldCollider& field);

private:
	//--------------------------------------------------------------------------------
	//  constructors and destructors
	//--------------------------------------------------------------------------------
	CollisionDetector() {}
	CollisionDetector(const CollisionDetector& value) {}
	CollisionDetector& operator=(const CollisionDetector& value) {}
	~CollisionDetector() {}

	//--------------------------------------------------------------------------------
	//	�֐����Fdetect
	//  �֐������F�_��AABB�̓����蔻��
	//	�����F	point�F�_�̈ʒu
	//			aabb�FAABB�R���C�_�[
	//	�߂�l�FCollision*
	//--------------------------------------------------------------------------------
	static Collision* Detect(const Vector3& point, const AabbCollider& aabb);
	
	//--------------------------------------------------------------------------------
	//	�֐����Fdetect
	//  �֐������F�_��Box�̓����蔻��
	//	�����F	collisionOut�F�Փˏ��(�o�͗p)
	//			vPoint�F�_�̈ʒu
	//			box�Fbox�R���C�_�[
	//	�߂�l�FCollision*
	//--------------------------------------------------------------------------------
	static Collision* Detect(const Vector3& point, const BoxCollider& box);

	//--------------------------------------------------------------------------------
	//	�֐����Fdetect
	//  �֐������F�_��field�̓����蔻��
	//	�����F	point�F�_�̈ʒu
	//			field�Ffield�R���C�_�[
	//	�߂�l�FCollision*
	//--------------------------------------------------------------------------------
	static Collision* Detect(const Vector3& point, const FieldCollider& field);
	
	//--------------------------------------------------------------------------------
	//	�֐����Fdetect
	//  �֐������F�����ƒ����̌�������(�񎟌�)
	//	�����F	beginL�FLineLeft�̎n�_
	//			endL�FLineLeft�̏I�_
	//			beginR�FLineRight�̎n�_
	//			endR�FLineRight�̏I�_
	//	�߂�l�FVector2*
	//--------------------------------------------------------------------------------
	static Vector2* Detect(const Vector2& begin_left, const Vector2& end_left, const Vector2& begin_right, const Vector2& end_right);
	
	//--------------------------------------------------------------------------------
	//	�֐����Fdetect
	//  �֐������F�����ƒ����̌�������(�O����)
	//	�����F	begin_left�Fline_lefteft�̎n�_
	//			end_left�Fline_lefteft�̏I�_
	//			begin_right�Fline_rightight�̎n�_
	//			end_right�Fline_rightight�̏I�_
	//	�߂�l�FVector3*
	//--------------------------------------------------------------------------------
	static Vector3* Detect(const Vector3& begin_left, const Vector3& end_left, const Vector3& begin_right, const Vector3& end_right);

	//--------------------------------------------------------------------------------
	//	�֐����FprojectBoxToAxis
	//  �֐������Fbox��^����ꂽ���ɓ��e���Ē������Z�o����֐�
	//	�����F	box�F�{�b�N�X�R���C�_�[
	//			axis�F�����
	//	�߂�l�Ffloat
	//--------------------------------------------------------------------------------
	static float ProjectBoxToAxis(const BoxCollider& box, const Vector3& axis);

	//--------------------------------------------------------------------------------
	//	�֐����FIsOverlapOnAxis
	//  �֐������F�{�b�N�X���m���^����ꂽ���ɏd�˂Ă邩�ǂ������`�F�b�N����
	//	�����F	box_left�F�{�b�N�X�R���C�_�[
	//			box_right�F�{�b�N�X�R���C�_�[
	//			axis�F�����
	//	�߂�l�Fbool
	//--------------------------------------------------------------------------------
	static bool	IsOverlapOnAxis(const BoxCollider& box_left, const BoxCollider& box_right, const Vector3& axis);

	//--------------------------------------------------------------------------------
	//	�֐����FcheckOverlapOnAxis
	//  �֐������FAABB���m��XYZ���ɏd�˂Ă邩�ǂ������`�F�b�N����(�O����)
	//	�����F	aabb_left�FAABB�R���C�_�[
	//			aabb_right�FAABB�R���C�_�[
	//	�߂�l�Fbool
	//--------------------------------------------------------------------------------
	static bool IsOverlap(const AabbCollider& aabb_left, const AabbCollider& aabb_right);

	//--------------------------------------------------------------------------------
	//	�֐����FisOverlap
	//  �֐������FAABB���m���^����ꂽ���ɏd�˂Ă邩�ǂ������`�F�b�N����(�񎟌�)
	//	�����F	min_left�Faabb_left��ԍ���̓_���
	//			max_left�Faabb_left��ԉE���̓_���
	//			min_right�Faabb_right��ԍ���̓_���
	//			max_right�Faabb_right��ԉE���̓_���
	//	�߂�l�Fbool
	//--------------------------------------------------------------------------------
	static bool IsOverlap(const Vector2& min_left, const Vector2& max_left, const Vector2& min_right, const Vector2& max_right);
	
	//--------------------------------------------------------------------------------
	//	�֐����FMaxPenetration
	//  �֐������F�Փː[�x����Ԑ[���̃R���W������Ԃ��A�c��͔̂j������
	//	�����F	current�F����Ԑ[���R���W����
	//			next�F�V�����R���W����
	//	�߂�l�FCollision*
	//--------------------------------------------------------------------------------
	static Collision*	MaxPenetration(Collision* current, Collision* next);
};