//--------------------------------------------------------------------------------
//
//�@motionManager.h
//	Author : Xu Wenjie
//	Date   : 2017-11-06
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------
class MotionInfo;

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class MotionManager
{
public:
	//--------------------------------------------------------------------------------
	//  �֐��錾
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
	auto		GetMotionInfoBy(const string& motionName) { return motions.at(motionName).Pointer; }
	void		CreateMotionFileBy(const string& txtFilePath);

private:
	//--------------------------------------------------------------------------------
	//  �\���̒�`
	//--------------------------------------------------------------------------------
	struct MotionStruct
	{
		int			UserNumber;
		MotionInfo*	Pointer;
	};

	//--------------------------------------------------------------------------------
	//  �֐��錾
	//--------------------------------------------------------------------------------
	MotionManager() { motions.clear(); }
	~MotionManager() {}
	void init(void) {};
	void uninit(void);

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	unordered_map<string, MotionStruct>	motions;
	static MotionManager*				instance;
};