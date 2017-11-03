//--------------------------------------------------------------------------------
//
//�@soundManager.h
//	Author : Xu Wenjie
//	Date   : 2017-1-23
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "main.h"

//--------------------------------------------------------------------------------
//  �񋓌^��`
//--------------------------------------------------------------------------------
enum SoundLabel
{
	GameBGM,
	Max,
};

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class SoundManager
{
public:
	static auto	Create(void)
	{
		if (instance) return instance;
		instance = new SoundManager;
		instance->LoadAll();
		return instance;
	}
	static void Release(void) { SAFE_UNINIT(instance); }
	static auto Instance(void) { return instance; }

	HRESULT	LoadAll(void);
	void	UnloadAll(void);
	void	Play(const SoundLabel label);
	void	Stop(const SoundLabel label);
	bool	IsOver(const SoundLabel label);
	bool	IsPlaying(const SoundLabel label);
	void	StopAll(void);

private:
	//--------------------------------------------------------------------------------
	//�@�\���̒�`
	//--------------------------------------------------------------------------------
	struct Paramater
	{
		string	FileName;
		int		CountLoop;
	};

	//--------------------------------------------------------------------------------
	//  �֐��錾
	//--------------------------------------------------------------------------------
	SoundManager();
	~SoundManager() {}
	void	uninit(void) { UnloadAll(); }
	HRESULT checkChunk(HANDLE file, DWORD format, DWORD *chunkSize, DWORD *chunkDataPosition);
	HRESULT readChunkData(HANDLE file, void *buffer, DWORD buffersize, DWORD bufferoffset);

	//--------------------------------------------------------------------------------
	//  �ϐ���`
	//--------------------------------------------------------------------------------
	IXAudio2*				instanceXAudio2;
	IXAudio2MasteringVoice*	masteringVoice;
	IXAudio2SourceVoice*	sourceVoices[SoundLabel::Max];
	BYTE*					audioDatas[SoundLabel::Max];
	DWORD					audioSizes[SoundLabel::Max];
	static Paramater 		paramaters[SoundLabel::Max];
	static SoundManager*	instance;
};