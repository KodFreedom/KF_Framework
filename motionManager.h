//--------------------------------------------------------------------------------
//
//　motionManager.h
//	Author : Xu Wenjie
//	Date   : 2017-11-06
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class MotionInfo;

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class MotionManager
{
#ifdef _DEBUG
	friend class DebugObserver;
#endif // _DEBUG

public:
	//--------------------------------------------------------------------------------
	//  関数宣言
	//--------------------------------------------------------------------------------
	static auto	Create(void)
	{
		if (instance) return instance;
		instance = new MotionManager;
		instance->init();
		return instance;
	}
	static void Release(void) { SAFE_UNINIT(instance); }
	static auto Instance(void) { return instance; }

	void		Use(const string& motionName);
	void		Disuse(const string& motionName);
	auto		GetMotionInfoBy(const string& motionName) { return motions.at(motionName).Info; }

private:
	//--------------------------------------------------------------------------------
	//  構造体定義
	//--------------------------------------------------------------------------------
	struct MotionStruct
	{
		unsigned short		UserNumber;
		MotionInfo*			Info;
	};

	//--------------------------------------------------------------------------------
	//  関数宣言
	//--------------------------------------------------------------------------------
	MotionManager() { motions.clear(); }
	~MotionManager() {}
	void init(void) {};
	void uninit(void);

	//--------------------------------------------------------------------------------
	//  変数定義
	//--------------------------------------------------------------------------------
	unordered_map<string, MotionStruct>	motions;
	static MotionManager*				instance;
};