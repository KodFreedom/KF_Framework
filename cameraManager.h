//--------------------------------------------------------------------------------
//  �J�����}�l�[�W��
//�@cameraManager.h
//	Author : Xu Wenjie
//	Date   : 2017-11-05
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------
class Camera;

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class CameraManager
{
public:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	static auto	Create(void)
	{
		if (instance) return instance;
		instance = new CameraManager;
		return instance;
	}
	static void Release(void) { SAFE_UNINIT(instance); }
	static auto Instance(void) { return instance; }

	void Update(void);
	void LateUpdate(void);
	void ReleaseAll(void);
	void Register(Camera* camera) { cameras.push_back(camera); }
	void RegisterAsMain(Camera* camera) { cameras.push_front(camera); }
	auto GetMainCamera(void) const { return cameras.front(); }

private:
	//--------------------------------------------------------------------------------
	//  �֐���`
	//--------------------------------------------------------------------------------
	CameraManager() {}
	~CameraManager() {}
	void uninit(void) { ReleaseAll(); }

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	list<Camera*>			cameras;
	static CameraManager*	instance;
};