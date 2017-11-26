//--------------------------------------------------------------------------------
//
//�@soundManager.cpp
//	Author : Xu Wenjie
//	Date   : 2017-1-23
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------
#include "soundManager.h"

//--------------------------------------------------------------------------------
//  �ÓI�����o�ϐ�
//--------------------------------------------------------------------------------
SoundManager* SoundManager::instance = nullptr;
SoundManager::Paramater SoundManager::paramaters[SL_Max] =
{
	{ "data/BGM/gameBGM.wav", -1 },	// BGM
};

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
SoundManager::SoundManager()
	: instanceXAudio2(nullptr)
	, masteringVoice(nullptr)
{
	for (int count = 0; count < (int)SL_Max; ++count)
	{
		sourceVoices[count] = nullptr;
		audioDatas[count] = nullptr;
		audioSizes[count] = 0;
	}
}

//--------------------------------------------------------------------------------
//  �ǂݍ��ݏ���
//--------------------------------------------------------------------------------
HRESULT	SoundManager::LoadAll(void)
{
	// COM���C�u�����̏�����
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2�I�u�W�F�N�g�̍쐬
	HRESULT hr = XAudio2Create(&instanceXAudio2, 0);
	if (FAILED(hr))
	{
		MessageBox(NULL, "XAudio2�I�u�W�F�N�g�̍쐬�Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// COM���C�u�����̏I������
		CoUninitialize();

		return hr;
	}

	// �}�X�^�[�{�C�X�̐���
	hr = instanceXAudio2->CreateMasteringVoice(&masteringVoice);
	if (FAILED(hr))
	{
		MessageBox(NULL, "�}�X�^�[�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);

		if (instanceXAudio2)
		{
			// XAudio2�I�u�W�F�N�g�̊J��
			instanceXAudio2->Release();
			instanceXAudio2 = NULL;
		}

		// COM���C�u�����̏I������
		CoUninitialize();

		return hr;
	}

	// �T�E���h�f�[�^�̏�����
	for (int count = 0; count < SL_Max; ++count)
	{
		HANDLE file;
		DWORD chunkSize = 0;
		DWORD chunkPosition = 0;
		DWORD filetype;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// �o�b�t�@�̃N���A
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// �T�E���h�f�[�^�t�@�C���̐���
		file = CreateFile(paramaters[count].FileName.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (file == INVALID_HANDLE_VALUE)
		{
			MessageBox(NULL, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if (SetFilePointer(file, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// �t�@�C���|�C���^��擪�Ɉړ�
			MessageBox(NULL, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}

		// WAVE�t�@�C���̃`�F�b�N
		hr = checkChunk(file, 'FFIR', &chunkSize, &chunkPosition);
		if (FAILED(hr))
		{
			MessageBox(NULL, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return hr;
		}
		hr = readChunkData(file, &filetype, sizeof(DWORD), chunkPosition);
		if (FAILED(hr))
		{
			MessageBox(NULL, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return hr;
		}
		if (filetype != 'EVAW')
		{
			MessageBox(NULL, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(3)", "�x���I", MB_ICONWARNING);
			return hr;
		}

		// �t�H�[�}�b�g�`�F�b�N
		hr = checkChunk(file, ' tmf', &chunkSize, &chunkPosition);
		if (FAILED(hr))
		{
			MessageBox(NULL, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return hr;
		}
		hr = readChunkData(file, &wfx, chunkSize, chunkPosition);
		if (FAILED(hr))
		{
			MessageBox(NULL, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return hr;
		}

		// �I�[�f�B�I�f�[�^�ǂݍ���
		hr = checkChunk(file, 'atad', &audioSizes[count], &chunkPosition);
		if (FAILED(hr))
		{
			MessageBox(NULL, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return hr;
		}
		audioDatas[count] = (BYTE*)malloc(audioSizes[count]);
		hr = readChunkData(file, audioDatas[count], audioSizes[count], chunkPosition);
		if (FAILED(hr))
		{
			MessageBox(NULL, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return hr;
		}

		// �\�[�X�{�C�X�̐���
		hr = instanceXAudio2->CreateSourceVoice(&sourceVoices[count], &(wfx.Format));
		if (FAILED(hr))
		{
			MessageBox(NULL, "�\�[�X�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);
			return hr;
		}

		// �o�b�t�@�̒l�ݒ�
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = audioSizes[count];
		buffer.pAudioData = audioDatas[count];
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount = paramaters[count].CountLoop;

		// �I�[�f�B�I�o�b�t�@�̓o�^
		sourceVoices[count]->SubmitSourceBuffer(&buffer);
	}

	return hr;
}

//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
void SoundManager::UnloadAll(void)
{
	for (int count = 0; count < SL_Max; ++count)
	{
		if (sourceVoices[count])
		{
			// �ꎞ��~
			sourceVoices[count]->Stop(0);

			// �\�[�X�{�C�X�̔j��
			sourceVoices[count]->DestroyVoice();
			sourceVoices[count] = nullptr;

			// �I�[�f�B�I�f�[�^�̊J��
			free(audioDatas[count]);
			audioDatas[count] = nullptr;
		}
	}

	// �}�X�^�[�{�C�X�̔j��
	masteringVoice->DestroyVoice();
	masteringVoice = nullptr;

	// XAudio2�I�u�W�F�N�g�̉��
	SAFE_RELEASE(instanceXAudio2);

	// COM���C�u�����̏I������
	CoUninitialize();
}

//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
void SoundManager::Play(const SoundLabel label)
{
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	// �o�b�t�@�̒l�ݒ�
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = audioSizes[label];
	buffer.pAudioData = audioDatas[label];
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = paramaters[label].CountLoop;

	// ��Ԏ擾
	sourceVoices[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// �Đ���
	 // �ꎞ��~
		sourceVoices[label]->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		sourceVoices[label]->FlushSourceBuffers();
	}

	// �I�[�f�B�I�o�b�t�@�̓o�^
	sourceVoices[label]->SubmitSourceBuffer(&buffer);

	// �Đ�
	sourceVoices[label]->Start(0);
}

//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
void SoundManager::Stop(const SoundLabel label)
{
	XAUDIO2_VOICE_STATE xa2state;

	// ��Ԏ擾
	sourceVoices[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// �Đ���
	 // �ꎞ��~
		sourceVoices[label]->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		sourceVoices[label]->FlushSourceBuffers();
	}
}

//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
bool SoundManager::IsOver(const SoundLabel label)
{
	XAUDIO2_VOICE_STATE xa2state;

	// ��Ԏ擾
	sourceVoices[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{
		return false;
	}

	return true;
}

//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
bool SoundManager::IsPlaying(const SoundLabel label)
{
	XAUDIO2_VOICE_STATE xa2state;

	// ��Ԏ擾
	sourceVoices[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{
		return true;
	}

	return false;
}

//--------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------
void SoundManager::StopAll(void)
{
	for (int count = 0; count < SL_Max; count++)
	{
		if (sourceVoices[count])
		{
			// �ꎞ��~
			sourceVoices[count]->Stop(0);
		}
	}
}

//--------------------------------------------------------------------------------
// �`�����N�̃`�F�b�N
//--------------------------------------------------------------------------------
HRESULT SoundManager::checkChunk(HANDLE file, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD read;
	DWORD chunkType;
	DWORD chunkDataSize;
	DWORD RIFFDataSize = 0;
	DWORD filetype;
	DWORD bytesRead = 0;
	DWORD offset = 0;

	if (SetFilePointer(file, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// �t�@�C���|�C���^��擪�Ɉړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}

	while (hr == S_OK)
	{
		if (ReadFile(file, &chunkType, sizeof(DWORD), &read, NULL) == 0)
		{// �`�����N�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if (ReadFile(file, &chunkDataSize, sizeof(DWORD), &read, NULL) == 0)
		{// �`�����N�f�[�^�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch (chunkType)
		{
		case 'FFIR':
			RIFFDataSize = chunkDataSize;
			chunkDataSize = 4;
			if (ReadFile(file, &filetype, sizeof(DWORD), &read, NULL) == 0)
			{// �t�@�C���^�C�v�̓ǂݍ���
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if (SetFilePointer(file, chunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// �t�@�C���|�C���^���`�����N�f�[�^���ړ�
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		offset += sizeof(DWORD) * 2;
		if (chunkType == format)
		{
			*pChunkSize = chunkDataSize;
			*pChunkDataPosition = offset;

			return S_OK;
		}

		offset += chunkDataSize;
		if (bytesRead >= RIFFDataSize)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//--------------------------------------------------------------------------------
// �`�����N�f�[�^�̓ǂݍ���
//--------------------------------------------------------------------------------
HRESULT SoundManager::readChunkData(HANDLE file, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD read;

	if (SetFilePointer(file, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// �t�@�C���|�C���^���w��ʒu�܂ňړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (ReadFile(file, pBuffer, dwBuffersize, &read, NULL) == 0)
	{// �f�[�^�̓ǂݍ���
		return HRESULT_FROM_WIN32(GetLastError());
	}

	return S_OK;
}