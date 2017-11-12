//--------------------------------------------------------------------------------
//	�R���W��������N���X
//�@KF_Collision.cpp
//	Author : Xu Wenjie
//	Date   : 2017-06-05
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "collisionSystem.h"
#include "collisionDetector.h"
#include "physicsSystem.h"
#include "gameObject.h"
#include "sphereCollider.h"
#include "fieldCollider.h"
#include "AABBCollider.h"
#include "OBBCollider.h"
#include "rigidbody3D.h"
#include "behavior.h"
#include "transform.h"

#if defined(_DEBUG) || defined(EDITOR)
//#include "rendererDX.h"
//#include "textureManager.h"
#include "debugObserver.h"
#endif

//--------------------------------------------------------------------------------
//  �ÓI�����o�ϐ�
//--------------------------------------------------------------------------------
CollisionSystem* CollisionSystem::instance = nullptr;

//--------------------------------------------------------------------------------
//
//  public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void CollisionSystem::Update(void)
{
	//CM_Dynamic�w�̃R���W�������Ƃɂ܂�FStatic�w�Ɠ����蔻�肵��
	//CM_Dynamic�w�̑��̃R���W�����Ɠ����蔻�肵��
	//�Ō��Field�Ɠ����蔻�肷��
	//Sphere
	for (auto iterator = collidersArrays[CM_Dynamic][CT_Sphere].begin(); iterator != collidersArrays[CM_Dynamic][CT_Sphere].end(); ++iterator)
	{
		if (!(*iterator)->GetGameObject()->IsActive()) continue;
		auto& sphere = *dynamic_cast<SphereCollider*>(*iterator);

#pragma omp parallel sections
		{
#pragma omp section 
			checkWithDynamicSphere(iterator, sphere);
#pragma omp section 
			checkWithDynamicAABB(sphere);
#pragma omp section 
			checkWithDynamicOBB(sphere);
#pragma omp section 
			checkWithStaticSphere(sphere);
#pragma omp section 
			checkWithStaticAABB(sphere);
#pragma omp section
			checkWithStaticOBB(sphere);
#pragma omp section
			checkWithField(sphere);
		}
	}

	//aabb
	for (auto iterator = collidersArrays[CM_Dynamic][CT_AABB].begin(); iterator != collidersArrays[CM_Dynamic][CT_AABB].end(); ++iterator)
	{
		if (!(*iterator)->GetGameObject()->IsActive()) continue;
		auto& AABB = *dynamic_cast<AABBCollider*>(*iterator);

#pragma omp parallel sections
		{
#pragma omp section 
			checkWithDynamicAABB(iterator, AABB);
#pragma omp section 
			checkWithDynamicOBB(AABB);
#pragma omp section 
			checkWithStaticSphere(AABB);
#pragma omp section 
			checkWithStaticAABB(AABB);
#pragma omp section 
			checkWithStaticOBB(AABB);
#pragma omp section
			checkWithField(AABB);
		}
	}

	//obb
	for (auto iterator = collidersArrays[CM_Dynamic][CT_OBB].begin(); iterator != collidersArrays[CM_Dynamic][CT_OBB].end(); ++iterator)
	{
		if (!(*iterator)->GetGameObject()->IsActive()) continue;
		auto& OBB = *dynamic_cast<OBBCollider*>(*iterator);

#pragma omp parallel sections
		{
#pragma omp section 
			checkWithDynamicOBB(iterator, OBB);
#pragma omp section 
			checkWithStaticSphere(OBB);
#pragma omp section 
			checkWithStaticAABB(OBB);
#pragma omp section 
			checkWithStaticOBB(OBB);
#pragma omp section 
			checkWithField(OBB);
		}
	}
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void CollisionSystem::LateUpdate(void)
{
#if defined(_DEBUG) || defined(EDITOR)
	auto debugObserver = DebugObserver::Instance();
	debugObserver->DisplayAlways("Collider��� : \n");
	for (int countMode = 0; countMode < CM_Max; ++countMode)
	{
		debugObserver->DisplayAlways(modeToString((ColliderMode)countMode) + " : ");
		for (int countType = 0; countType < CT_Max; ++countType)
		{
			int nNumCol = (int)collidersArrays[countMode][countType].size();
			debugObserver->DisplayAlways(
				typeToString((ColliderType)countType) +
				"-" +
				to_string(nNumCol) + "    ");
		}
		debugObserver->DisplayAlways('\n');
	}
#endif
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void CollisionSystem::Clear(void)
{
	for (auto colliders : collidersArray)
	{
		colliders.clear();
	}
	fields.clear();
}

//--------------------------------------------------------------------------------
//	�֐����FRegister
//  �֐������F�R���C�_�[��o�^����
//	�����F	collider
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void CollisionSystem::Register(Collider* collider)
{
	if (collider->GetType() < CT_Max)
	{
		collidersArrays[collider->GetMode()][collider->GetType()].push_back(collider);
		return;
	}
	switch (collider->GetType())
	{
	case CT_Field:
		fields.push_back(collider);
		break;
	default:
		assert("error type!!!");
		break;
	}
}

//--------------------------------------------------------------------------------
//	�֐����FDeregister
//  �֐������F�R���C�_�[���폜����
//	�����F	collider
//	�߂�l�F�Ȃ�
//--------------------------------------------------------------------------------
void CollisionSystem::Deregister(Collider* collider)
{
	if (collider->GetType() < CT_Max)
	{
		collidersArrays[collider->GetMode()][collider->GetType()].remove(collider);
		return;
	}
	switch (collider->GetType())
	{
	case CT_Field:
	{
		fields.remove(collider);
		break;
	}
	default:
		assert("error type!!!");
		break;
	}
}

//--------------------------------------------------------------------------------
//	�֐����FRayCast
//  �֐������F���C�L���X�g�֐��A���C�ƏՓ˂����̍ŋߓ_���Z�o����
//	�����F	ray�F���C
//			distance�F���C�̒���
//			rayOwner�F�����̃Q�[���I�u�W�F�N�g
//	�߂�l�FRayHitInfo*
//--------------------------------------------------------------------------------
RayHitInfo* CollisionSystem::RayCast(const Ray& ray, const float& distance, const GameObject* const rayOwner)
{
	RayHitInfo* realResult = nullptr;
#pragma omp parallel sections
	{
#pragma omp section
		{
			auto result = raycastDynamicSphere(ray, distance, rayOwner);
			realResult = getRealResult(realResult, result);
		}

#pragma omp section
		{
			auto result = raycastDynamicAABB(ray, distance, rayOwner);
			realResult = getRealResult(realResult, result);
		}

#pragma omp section
		{
			auto result = raycastDynamicOBB(ray, distance, rayOwner);
			realResult = getRealResult(realResult, result);
		}

#pragma omp section
		{
			auto result = raycastStaticSphere(ray, distance);
			realResult = getRealResult(realResult, result);
		}

#pragma omp section
		{
			auto result = raycastStaticAABB(ray, distance);
			realResult = getRealResult(realResult, result);
		}

#pragma omp section
		{
			auto result = raycastStaticOBB(ray, distance);
			realResult = getRealResult(realResult, result);
		}

#pragma omp section
		{
			auto result = raycastField(ray, distance);
			realResult = getRealResult(realResult, result);
		}
	}
	return realResult;
}

#if defined(_DEBUG) || defined(EDITOR)
//--------------------------------------------------------------------------------
//  DrawCollider
//--------------------------------------------------------------------------------
void CollisionSystem::DrawCollider(void)
{
	//if (!m_bDrawCollider) { return; }

	//auto pDevice = Main::GetManager()->GetRenderer()->GetDevice();
	//LPDIRECT3DTEXTURE9 texturePointer = Main::GetManager()->GetTextureManager()->GetTexture("polygon.png");
	//pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	//pDevice->SetTexture(0, texturePointer);

	////sphere
	//for (auto pCol : colliders[CM_Dynamic][Sphere])
	//{
	//	if (!pCol->GetGameObject()->IsActive()) continue;
	//	D3DXVECTOR3 Position = pCol->GetNextWorldPosition();
	//	float fRadius = ((SphereCollider*)pCol)->GetRadius();
	//	D3DXMATRIX mtx,mtxPos,mtxScale;
	//	D3DXMatrixIdentity(&mtx);
	//	D3DXMatrixScaling(&mtxScale, fRadius, fRadius, fRadius);
	//	mtx *= mtxScale;
	//	D3DXMatrixTranslation(&mtxPos, Position.x, Position.y, Position.z);
	//	mtx *= mtxPos;
	//	pDevice->SetTransform(D3DTS_WORLD, &mtx);
	//	m_meshSphere->DrawSubset(0);
	//}

	//for (auto pCol : colliders[Static][Sphere])
	//{
	//	if (!pCol->GetGameObject()->IsActive()) continue;
	//	auto Position = pCol->GetNextWorldPosition();
	//	float fRadius = ((SphereCollider*)pCol)->GetRadius();
	//	D3DXMATRIX mtx, mtxPos, mtxScale;
	//	D3DXMatrixIdentity(&mtx);
	//	D3DXMatrixScaling(&mtxScale, fRadius, fRadius, fRadius);
	//	mtx *= mtxScale;
	//	D3DXMatrixTranslation(&mtxPos, Position.X, Position.Y, Position.Z);
	//	mtx *= mtxPos;
	//	pDevice->SetTransform(D3DTS_WORLD, &mtx);
	//	m_meshSphere->DrawSubset(0);
	//}

	////AABB
	//for (auto pCol : colliders[CM_Dynamic][AABB])
	//{
	//	if (!pCol->GetGameObject()->IsActive()) continue;
	//	auto Position = pCol->GetNextWorldPosition();
	//	auto vHalfSize = ((AABBCollider*)pCol)->GetHalfSize();
	//	D3DXMATRIX mtx, mtxPos, mtxScale;
	//	D3DXMatrixIdentity(&mtx);
	//	D3DXMatrixScaling(&mtxScale, vHalfSize.X * 2.0f, vHalfSize.Y * 2.0f, vHalfSize.Z * 2.0f);
	//	mtx *= mtxScale;
	//	D3DXMatrixTranslation(&mtxPos, Position.X, Position.Y, Position.Z);
	//	mtx *= mtxPos;
	//	pDevice->SetTransform(D3DTS_WORLD, &mtx);
	//	m_meshCube->DrawSubset(0);
	//}

	//for (auto pCol : colliders[Static][AABB])
	//{
	//	if (!pCol->GetGameObject()->IsActive()) continue;
	//	auto Position = pCol->GetNextWorldPosition();
	//	auto vHalfSize = ((AABBCollider*)pCol)->GetHalfSize();
	//	D3DXMATRIX mtx, mtxPos, mtxScale;
	//	D3DXMatrixIdentity(&mtx);
	//	D3DXMatrixScaling(&mtxScale, vHalfSize.X * 2.0f, vHalfSize.Y * 2.0f, vHalfSize.Z * 2.0f);
	//	mtx *= mtxScale;
	//	D3DXMatrixTranslation(&mtxPos, Position.X, Position.Y, Position.Z);
	//	mtx *= mtxPos;
	//	pDevice->SetTransform(D3DTS_WORLD, &mtx);
	//	m_meshCube->DrawSubset(0);
	//}

	////OBB
	//for (auto pCol : colliders[CM_Dynamic][OBB])
	//{
	//	if (!pCol->GetGameObject()->IsActive()) continue;
	//	auto& vHalfSize = ((OBBCollider*)pCol)->GetHalfSize();
	//	D3DXMATRIX mtx;
	//	D3DXMatrixScaling(&mtx, vHalfSize.X * 2.0f, vHalfSize.Y * 2.0f, vHalfSize.Z * 2.0f);
	//	D3DXMATRIX mtxOff = pCol->GetOffset();
	//	mtx *= mtxOff;
	//	D3DXMATRIX mtxObj = pCol->GetGameObject()->GetTransform()->GetMatrix();
	//	mtx *= mtxObj;
	//	pDevice->SetTransform(D3DTS_WORLD, &mtx);
	//	m_meshCube->DrawSubset(0);
	//}

	//for (auto pCol : colliders[Static][OBB])
	//{
	//	if (!pCol->GetGameObject()->IsActive()) continue;
	//	auto& vHalfSize = ((OBBCollider*)pCol)->GetHalfSize();
	//	D3DXMATRIX mtx;
	//	D3DXMatrixScaling(&mtx, vHalfSize.X * 2.0f, vHalfSize.Y * 2.0f, vHalfSize.Z * 2.0f);
	//	D3DXMATRIX mtxOff = pCol->GetOffset();
	//	mtx *= mtxOff;
	//	D3DXMATRIX mtxObj = pCol->GetGameObject()->GetTransform()->GetMatrix();
	//	mtx *= mtxObj;
	//	pDevice->SetTransform(D3DTS_WORLD, &mtx);
	//	m_meshCube->DrawSubset(0);
	//}


	//pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}
#endif

//--------------------------------------------------------------------------------
//
//  private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
CollisionSystem::CollisionSystem()
#if defined(_DEBUG) || defined(EDITOR)
//: m_meshSphere(nullptr)
//, m_meshCube(nullptr)
//, m_bDrawCollider(false)
#endif
{
	Clear();
}

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void CollisionSystem::init(void)
{
#if defined(_DEBUG) || defined(EDITOR)
	//auto pDevice = Main::GetManager()->GetRenderer()->GetDevice();
	//Main::GetManager()->GetTextureManager()->UseTexture("polygon.png");
	//D3DXCreateSphere(pDevice, 1.0f, 10, 10, &m_meshSphere, nullptr);
	//D3DXCreateBox(pDevice, 1.0f, 1.0f, 1.0f, &m_meshCube, nullptr);
#endif
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void CollisionSystem::uninit(void)
{
	Clear();

#if defined(_DEBUG) || defined(EDITOR)
	//SAFE_RELEASE(m_meshSphere);
	//SAFE_RELEASE(m_meshCube);
	//Main::GetManager()->GetTextureManager()->DisuseTexture("polygon.png");
#endif
}

//--------------------------------------------------------------------------------
//
//  �Փ˔��菈��
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �X�t�B�A�ƃX�t�B�A�̓����蔻��
//--------------------------------------------------------------------------------
void CollisionSystem::checkWithDynamicSphere(const Iterator& begin, SphereCollider& sphere)
{
	for (auto iterator = begin; iterator != collidersArrays[CM_Dynamic][CT_Sphere].end(); ++iterator)
	{
		if (!(*iterator)->GetGameObject()->IsActive()) continue;
		if ((*iterator)->GetGameObject() == sphere.GetGameObject()) continue;
		CollisionDetector::Detect(sphere, *dynamic_cast<SphereCollider*>(*iterator));
	}
}

//--------------------------------------------------------------------------------
//  �X�t�B�A��AABB�̓����蔻��
//--------------------------------------------------------------------------------
void CollisionSystem::checkWithDynamicAABB(SphereCollider& sphere)
{
	for (auto collider : collidersArrays[CM_Dynamic][CT_AABB])
	{
		if (!collider->GetGameObject()->IsActive()) continue;
		if (collider->GetGameObject() == sphere.GetGameObject()) continue;
		CollisionDetector::Detect(sphere, *dynamic_cast<AABBCollider*>(collider));
	}
}

//--------------------------------------------------------------------------------
//  AABB��AABB�̓����蔻��
//--------------------------------------------------------------------------------
void CollisionSystem::checkWithDynamicAABB(const Iterator& begin, AABBCollider& aabb)
{
	for (auto iterator = begin; iterator != collidersArrays[CM_Dynamic][CT_AABB].end(); ++iterator)
	{
		if (!(*iterator)->GetGameObject()->IsActive()) continue;
		if ((*iterator)->GetGameObject() == aabb.GetGameObject()) continue;
		CollisionDetector::Detect(aabb, *dynamic_cast<AABBCollider*>(*iterator));
	}
}

//--------------------------------------------------------------------------------
//  �X�t�B�A��OBB�̓����蔻��
//--------------------------------------------------------------------------------
void CollisionSystem::checkWithDynamicOBB(SphereCollider& sphere)
{
	for (auto collider : collidersArrays[CM_Dynamic][CT_OBB])
	{
		if (!collider->GetGameObject()->IsActive()) continue;
		if (collider->GetGameObject() == sphere.GetGameObject()) continue;
		CollisionDetector::Detect(sphere, *dynamic_cast<OBBCollider*>(collider));
	}
}

//--------------------------------------------------------------------------------
//  AABB��OBB�̓����蔻��
//--------------------------------------------------------------------------------
void CollisionSystem::checkWithDynamicOBB(AABBCollider& aabb)
{
	for (auto collider : collidersArrays[CM_Dynamic][CT_OBB])
	{
		if (!collider->GetGameObject()->IsActive()) continue;
		if (collider->GetGameObject() == aabb.GetGameObject()) continue;
		CollisionDetector::Detect(aabb, *dynamic_cast<OBBCollider*>(collider));
	}
}

//--------------------------------------------------------------------------------
//  OBB��OBB�̓����蔻��
//--------------------------------------------------------------------------------
void CollisionSystem::checkWithDynamicOBB(const Iterator& begin, OBBCollider& obb)
{
	for (auto iterator = begin; iterator != collidersArrays[CM_Dynamic][CT_OBB].end(); ++iterator)
	{
		if (!(*iterator)->GetGameObject()->IsActive()) continue;
		if ((*iterator)->GetGameObject() == obb.GetGameObject()) continue;
		CollisionDetector::Detect(obb, *dynamic_cast<OBBCollider*>(*iterator));
	}
}

//--------------------------------------------------------------------------------
//  �X�t�B�A�ƃX�t�B�A�̓����蔻��
//--------------------------------------------------------------------------------
void CollisionSystem::checkWithStaticSphere(SphereCollider& sphere)
{
	for (auto collider : collidersArrays[CM_Static][CT_Sphere])
	{
		if (!collider->GetGameObject()->IsActive()) continue;
		CollisionDetector::Detect(sphere, *dynamic_cast<SphereCollider*>(collider));
	}
}

//--------------------------------------------------------------------------------
//  �X�t�B�A��AAB�̓����蔻��
//--------------------------------------------------------------------------------
void CollisionSystem::checkWithStaticSphere(AABBCollider& aabb)
{
	for (auto collider : collidersArrays[CM_Static][CT_Sphere])
	{
		if (!collider->GetGameObject()->IsActive()) continue;
		CollisionDetector::Detect(*dynamic_cast<SphereCollider*>(collider), aabb);
	}
}

//--------------------------------------------------------------------------------
//  �X�t�B�A��OBB�̓����蔻��
//--------------------------------------------------------------------------------
void CollisionSystem::checkWithStaticSphere(OBBCollider& obb)
{
	for (auto collider : collidersArrays[CM_Static][CT_Sphere])
	{
		if (!collider->GetGameObject()->IsActive()) continue;
		CollisionDetector::Detect(*dynamic_cast<SphereCollider*>(collider), obb);
	}
}

//--------------------------------------------------------------------------------
//  �X�t�B�A��OBB�̓����蔻��
//--------------------------------------------------------------------------------
void CollisionSystem::checkWithStaticAABB(SphereCollider& sphere)
{
	for (auto collider : collidersArrays[CM_Static][CT_AABB])
	{
		if (!collider->GetGameObject()->IsActive()) continue;
		CollisionDetector::Detect(sphere, *dynamic_cast<AABBCollider*>(collider));
	}
}

//--------------------------------------------------------------------------------
//  AABB��OBB�̓����蔻��
//--------------------------------------------------------------------------------
void CollisionSystem::checkWithStaticAABB(AABBCollider& aabb)
{
	for (auto collider : collidersArrays[CM_Static][CT_AABB])
	{
		if (!collider->GetGameObject()->IsActive()) continue;
		CollisionDetector::Detect(aabb, *dynamic_cast<AABBCollider*>(collider));
	}
}

//--------------------------------------------------------------------------------
//  OBB��AABB�̓����蔻��
//--------------------------------------------------------------------------------
void CollisionSystem::checkWithStaticAABB(OBBCollider& obb)
{
	for (auto collider : collidersArrays[CM_Static][CT_AABB])
	{
		if (!collider->GetGameObject()->IsActive()) continue;
		CollisionDetector::Detect(obb, *dynamic_cast<AABBCollider*>(collider));
	}
}

//--------------------------------------------------------------------------------
//  �X�t�B�A��OBB�̓����蔻��
//--------------------------------------------------------------------------------
void CollisionSystem::checkWithStaticOBB(SphereCollider& sphere)
{
	for (auto collider : collidersArrays[CM_Static][CT_OBB])
	{
		if (!collider->GetGameObject()->IsActive()) continue;
		CollisionDetector::Detect(sphere, *dynamic_cast<OBBCollider*>(collider));
	}
}

//--------------------------------------------------------------------------------
//  box��OBB�̓����蔻��
//--------------------------------------------------------------------------------
void CollisionSystem::checkWithStaticOBB(BoxCollider& box)
{
	for (auto collider : collidersArrays[CM_Static][CT_OBB])
	{
		if (!collider->GetGameObject()->IsActive()) continue;
		CollisionDetector::Detect(box, *dynamic_cast<OBBCollider*>(collider));
	}
}

//--------------------------------------------------------------------------------
//  �X�t�B�A�ƃt�B�[���h�̓����蔻��
//--------------------------------------------------------------------------------
void CollisionSystem::checkWithField(SphereCollider& sphere)
{
	for (auto collider : fields)
	{
		if (!collider->GetGameObject()->IsActive()) continue;
		CollisionDetector::Detect(sphere, *dynamic_cast<FieldCollider*>(collider));
	}
}

//--------------------------------------------------------------------------------
//  OBB�ƃt�B�[���h�̓����蔻��
//--------------------------------------------------------------------------------
void CollisionSystem::checkWithField(BoxCollider& box)
{
	for (auto collider : fields)
	{
		if (!collider->GetGameObject()->IsActive()) continue;
		CollisionDetector::Detect(box, *dynamic_cast<FieldCollider*>(collider));
	}
}

#if defined(_DEBUG) || defined(EDITOR)
//--------------------------------------------------------------------------------
//	�֐����FmodeToString
//  �֐������F���[�h��enum��string�ɕϊ�
//	�����F	mode
//	�߂�l�Fstring
//--------------------------------------------------------------------------------
string CollisionSystem::modeToString(const ColliderMode& mode)
{
	switch (mode)
	{
	case CM_Static:
		return "Static";
	case CM_Dynamic:
		return "CM_Dynamic";
	default:
		return "ErrorMode";
	}
}

//--------------------------------------------------------------------------------
//	�֐����FtoString
//  �֐������F�^�C�v��enum��string�ɕϊ�
//	�����F	type
//	�߂�l�Fstring
//--------------------------------------------------------------------------------
string CollisionSystem::typeToString(const ColliderType& type)
{
	switch (type)
	{
	case CT_Sphere:
		return "Sphere";
	case CT_AABB:
		return "AABB";
	case CT_OBB:
		return "OBB";
	case CT_Plane:
		return "Plane";
	case CT_Cylinder:
		return "Cylinder";
	default:
		return "ErrorType";
	}
}
#endif

//--------------------------------------------------------------------------------
//
//  Raycast����
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//	�֐����FraycastDynamicSphere
//  �֐������FCM_DynamicSphere�Ƃ�raycast
//	�����F	ray�F���C
//			distance�F���C�̋���
//			rayOwner�F���C�̏��L��
//	�߂�l�FRayHitInfo*
//--------------------------------------------------------------------------------
RayHitInfo* CollisionSystem::raycastDynamicSphere(const Ray& ray, const float& distance, const GameObject* const rayOwner)
{
	RayHitInfo* realResult = nullptr;
	for (auto collider : collidersArrays[CM_Dynamic][CT_Sphere])
	{
		if (!collider->GetGameObject()->IsActive()) continue;
		if (collider->GetGameObject() == rayOwner) continue;
		auto result = CollisionDetector::Detect(ray, distance, *dynamic_cast<SphereCollider*>(collider));
		realResult = getRealResult(realResult, result);
	}
	return realResult;
}

//--------------------------------------------------------------------------------
//	�֐����FraycastDynamicAABB
//  �֐������FCM_DynamicAABB�Ƃ�raycast
//	�����F	ray�F���C
//			distance�F���C�̋���
//			rayOwner�F���C�̏��L��
//	�߂�l�FRayHitInfo*
//--------------------------------------------------------------------------------
RayHitInfo* CollisionSystem::raycastDynamicAABB(const Ray& ray, const float& distance, const GameObject* const rayOwner)
{
	RayHitInfo* realResult = nullptr;
	for (auto collider : collidersArrays[CM_Dynamic][CT_AABB])
	{
		if (!collider->GetGameObject()->IsActive()) continue;
		if (collider->GetGameObject() == rayOwner) continue;
		auto result = CollisionDetector::Detect(ray, distance, *dynamic_cast<AABBCollider*>(collider));
		realResult = getRealResult(realResult, result);
	}
	return realResult;
}

//--------------------------------------------------------------------------------
//	�֐����FraycastDynamicOBB
//  �֐������FCM_DynamicOBB�Ƃ�raycast
//	�����F	ray�F���C
//			distance�F���C�̋���
//			rayOwner�F���C�̏��L��
//	�߂�l�FRayHitInfo*
//--------------------------------------------------------------------------------
RayHitInfo* CollisionSystem::raycastDynamicOBB(const Ray& ray, const float& distance, const GameObject* const rayOwner)
{
	RayHitInfo* realResult = nullptr;
	for (auto collider : collidersArrays[CM_Dynamic][CT_OBB])
	{
		if (!collider->GetGameObject()->IsActive()) continue;
		if (collider->GetGameObject() == rayOwner) continue;
		auto result = CollisionDetector::Detect(ray, distance, *dynamic_cast<OBBCollider*>(collider));
		realResult = getRealResult(realResult, result);
	}
	return realResult;
}

//--------------------------------------------------------------------------------
//	�֐����FraycastStaticSphere
//  �֐������FStaticSphere�Ƃ�raycast
//	�����F	ray�F���C
//			distance�F���C�̋���
//	�߂�l�FRayHitInfo*
//--------------------------------------------------------------------------------
RayHitInfo* CollisionSystem::raycastStaticSphere(const Ray& ray, const float& distance)
{
	RayHitInfo* realResult = nullptr;
	for (auto collider : collidersArrays[CM_Static][CT_Sphere])
	{
		if (!collider->GetGameObject()->IsActive()) continue;
		auto result = CollisionDetector::Detect(ray, distance, *dynamic_cast<SphereCollider*>(collider));
		realResult = getRealResult(realResult, result);
	}
	return realResult;
}

//--------------------------------------------------------------------------------
//	�֐����FraycastStaticAABB
//  �֐������FStaticAABB�Ƃ�raycast
//	�����F	ray�F���C
//			distance�F���C�̋���
//	�߂�l�FRayHitInfo*
//--------------------------------------------------------------------------------
RayHitInfo* CollisionSystem::raycastStaticAABB(const Ray& ray, const float& distance)
{
	RayHitInfo* realResult = nullptr;
	for (auto collider : collidersArrays[CM_Static][CT_AABB])
	{
		if (!collider->GetGameObject()->IsActive()) continue;
		auto result = CollisionDetector::Detect(ray, distance, *dynamic_cast<AABBCollider*>(collider));
		realResult = getRealResult(realResult, result);
	}
	return realResult;
}

//--------------------------------------------------------------------------------
//	�֐����FraycastStaticOBB
//  �֐������FStaticOBB�Ƃ�raycast
//	�����F	ray�F���C
//			distance�F���C�̋���
//	�߂�l�FRayHitInfo*
//--------------------------------------------------------------------------------
RayHitInfo* CollisionSystem::raycastStaticOBB(const Ray& ray, const float& distance)
{
	RayHitInfo* realResult = nullptr;
	for (auto collider : collidersArrays[CM_Static][CT_OBB])
	{
		if (!collider->GetGameObject()->IsActive()) continue;
		auto result = CollisionDetector::Detect(ray, distance, *dynamic_cast<OBBCollider*>(collider));
		realResult = getRealResult(realResult, result);
	}
	return realResult;
}

//--------------------------------------------------------------------------------
//	�֐����FraycastField
//  �֐������FStaticOBB�Ƃ�raycast
//	�����F	ray�F���C
//			distance�F���C�̋���
//	�߂�l�FRayHitInfo*
//--------------------------------------------------------------------------------
RayHitInfo* CollisionSystem::raycastField(const Ray& ray, const float& distance)
{
	RayHitInfo* realResult = nullptr;
	for (auto collider : fields)
	{
		if (!collider->GetGameObject()->IsActive()) continue;
		auto result = CollisionDetector::Detect(ray, distance, *dynamic_cast<FieldCollider*>(collider));
		realResult = getRealResult(realResult, result);
	}
	return realResult;
}

//--------------------------------------------------------------------------------
//	�֐����FgetRealResult
//  �֐������F���C�Ƃ̋������߂��̏���Ԃ��A�c��͔̂j������
//	�����F	current�F���̈�ԋ߂����
//			next�F�V�������
//	�߂�l�FRayHitInfo*
//--------------------------------------------------------------------------------
RayHitInfo* CollisionSystem::getRealResult(RayHitInfo* current, RayHitInfo* next)
{
	if (!next) return current;
	if (!current) return next;
	if (next->Distance < current->Distance)
	{
		delete current;
		return next;
	}
	delete next;
	return current;
}