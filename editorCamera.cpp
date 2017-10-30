//--------------------------------------------------------------------------------
//	�G�f�B�^�p�J�����N���X
//�@actionGameCamera.h
//	Author : Xu Wenjie
//	Date   : 2016-06-20
//--------------------------------------------------------------------------------
#ifdef _DEBUG
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "inputManager.h"
#include "editorCamera.h"
#include "inputDX.h"

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �ÓI�����o�ϐ�
//--------------------------------------------------------------------------------
const float	CEditorCamera::sc_fRotSpeed = 0.05f;
const float	CEditorCamera::sc_fStartRotMin = 0.2f;
const float	CEditorCamera::sc_fZoomSpeed = 0.5f;
const float	CEditorCamera::sc_fDistanceMin = 3.0f;
const float	CEditorCamera::sc_fDistanceMax = 500.0f;

//--------------------------------------------------------------------------------
//
//  public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
CEditorCamera::CEditorCamera() : CCamera()
{
}

//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
void CEditorCamera::Init(void)
{
	CCamera::Init();
	SetCamera(Vector3(0.0f), Vector3(0.0f, 20.0f, -20.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f));
}

//--------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------
void CEditorCamera::Update(void)
{
	auto pInput = CMain::GetManager()->GetInputManager();
	if (!pInput->GetMouse()->GetMousePress(CMouseDX::MOUSE_RIGHT)) { return; }
	
	Vector3 vRot;
	float fZoomSpeed = 0.0f;
	float fAxisX = pInput->GetMoveHorizontal();
	float fAxisY = pInput->GetMoveVertical();
	float fRAxisX = pInput->GetRotHorizontal();
	float fRAxisY = pInput->GetRotVertical();
	float fZoom = pInput->GetZoom();

	//���ړ_��]
	if (fabsf(fRAxisX) > sc_fStartRotMin)
	{//Y����]
		vRot.Y = sc_fRotSpeed * fRAxisX;
	}
	if (fabsf(fRAxisY) > sc_fStartRotMin)
	{//X����]
		vRot.Y = sc_fRotSpeed * fRAxisY;
	}

	//�g��k��
	if (fabsf(fZoom) > sc_fStartRotMin)
	{
		fZoomSpeed = sc_fZoomSpeed * fZoom;
	}

	//��]
	Yaw(vRot.Y);
	Pitch(vRot.Y);

	//�Y�[��
	m_fDistance += fZoomSpeed;
	m_fDistance = m_fDistance < sc_fDistanceMin ? sc_fDistanceMin : m_fDistance > sc_fDistanceMax ? sc_fDistanceMax : m_fDistance;
}
#endif // _DEBUG