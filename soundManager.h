//--------------------------------------------------------------------------------
//
//　soundManager.h
//	Author : Xu Wenjie
//	Date   : 2017-1-23
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"

//--------------------------------------------------------------------------------
//  定数定義
//--------------------------------------------------------------------------------
#define CSM CSoundManager	//マネージャの略称

//--------------------------------------------------------------------------------
//  クラス宣言
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
	//パラメータ構造体定義
	struct SOUNDPARAM
	{
		string strFileName;	// ファイル名
		int countLoop;		// ループカウント
	};

	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2*				m_pXAudio2;							// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice*	m_pMasteringVoice;					// マスターボイス
	IXAudio2SourceVoice*	m_apSourceVoice[SMax];	// ソースボイス
	BYTE*					m_apDataAudio[SMax];		// オーディオデータ
	DWORD					m_aSizeAudio[SMax];		// オーディオデータサイズ

	static SOUNDPARAM 		m_aParam[SMax];
};