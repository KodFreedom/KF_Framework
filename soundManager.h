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
//  �萔��`
//--------------------------------------------------------------------------------
#define CSM CSoundManager	//�}�l�[�W���̗���

//--------------------------------------------------------------------------------
//  �N���X�錾
//--------------------------------------------------------------------------------
class CSoundManager
{
public:
	enum SOUND_LABEL
	{
		BGM_GAME,
		SMax,
	};

	CSoundManager();
	~CSoundManager() {}

	void	Release(void)
	{
		UnloadAll();
		delete this;
	}
	HRESULT	LoadAll(void);
	void	UnloadAll(void);
	void	Play(SOUND_LABEL label);
	void	Stop(SOUND_LABEL label);
	bool	IsOver(SOUND_LABEL label);
	bool	IsPlaying(SOUND_LABEL label);
	void	StopAll(void);

private:
	//�p�����[�^�\���̒�`
	struct SOUNDPARAM
	{
		string strFileName;	// �t�@�C����
		int countLoop;		// ���[�v�J�E���g
	};

	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2*				m_pXAudio2;							// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	IXAudio2MasteringVoice*	m_pMasteringVoice;					// �}�X�^�[�{�C�X
	IXAudio2SourceVoice*	m_apSourceVoice[SMax];	// �\�[�X�{�C�X
	BYTE*					m_apDataAudio[SMax];		// �I�[�f�B�I�f�[�^
	DWORD					m_aSizeAudio[SMax];		// �I�[�f�B�I�f�[�^�T�C�Y

	static SOUNDPARAM 		m_aParam[SMax];
};