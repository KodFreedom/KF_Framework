//--------------------------------------------------------------------------------
//  カメラマネージャ
//　cameraManager.h
//	Author : Xu Wenjie
//	Date   : 2017-11-05
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class Camera;

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class CameraManager
{
public:
	//--------------------------------------------------------------------------------
	//  関数定義
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
	//  関数定義
	//--------------------------------------------------------------------------------
	CameraManager() {}
	~CameraManager() {}
	void uninit(void) { ReleaseAll(); }

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	list<Camera*>			cameras;
	static CameraManager*	instance;
};