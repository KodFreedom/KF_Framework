//--------------------------------------------------------------------------------
//�@sound_manager.cpp
//  manage the se,bgm's save, load
//	�T�E���h�Ǘ���
//	Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#include "sound_manager.h"
#include "kf_utility.h"

//--------------------------------------------------------------------------------
//  �ÓI�����o�ϐ�
//--------------------------------------------------------------------------------
SoundManager::Paramater SoundManager::paramaters_[eSoundMax] =
{
	{ L"data/bgm/game.wav", -1 },	// BGM
};

//--------------------------------------------------------------------------------
//  �w�肵���T�E���h��炷
//--------------------------------------------------------------------------------
void SoundManager::Play(const SoundLabel label)
{
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	// �o�b�t�@�̒l�ݒ�
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = audio_sizes_[label];
	buffer.pAudioData = audio_datas_[label];
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = paramaters_[label].count_loop;

	// ��Ԏ擾
	source_voices_[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// �Đ���
	 // �ꎞ��~
		source_voices_[label]->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		source_voices_[label]->FlushSourceBuffers();
	}

	// �I�[�f�B�I�o�b�t�@�̓o�^
	source_voices_[label]->SubmitSourceBuffer(&buffer);

	// �Đ�
	source_voices_[label]->Start(0);
}

//--------------------------------------------------------------------------------
//  �w�肵���T�E���h���~�߂�
//--------------------------------------------------------------------------------
void SoundManager::Stop(const SoundLabel label)
{
	XAUDIO2_VOICE_STATE xa2state;

	// ��Ԏ擾
	source_voices_[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// �Đ���
	 // �ꎞ��~
		source_voices_[label]->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		source_voices_[label]->FlushSourceBuffers();
	}
}

//--------------------------------------------------------------------------------
//  �w�肵���T�E���h���I����Ă邩���`�F�b�N
//--------------------------------------------------------------------------------
bool SoundManager::IsOver(const SoundLabel label)
{
	XAUDIO2_VOICE_STATE xa2state;

	// ��Ԏ擾
	source_voices_[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{
		return false;
	}

	return true;
}

//--------------------------------------------------------------------------------
//  �w�肵���T�E���h���Ȃ��Ă邩���`�F�b�N
//--------------------------------------------------------------------------------
bool SoundManager::IsPlaying(const SoundLabel label)
{
	XAUDIO2_VOICE_STATE xa2state;

	// ��Ԏ擾
	source_voices_[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{
		return true;
	}

	return false;
}

//--------------------------------------------------------------------------------
//  �S�ẴT�E���h���~�܂�
//--------------------------------------------------------------------------------
void SoundManager::StopAll(void)
{
	for (int count = 0; count < eSoundMax; count++)
	{
		if (source_voices_[count])
		{
			// �ꎞ��~
			source_voices_[count]->Stop(0);
		}
	}
}

//--------------------------------------------------------------------------------
//
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------
bool SoundManager::Init(void)
{
	// COM���C�u�����̏�����
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2�I�u�W�F�N�g�̍쐬
	HRESULT hr = XAudio2Create(&instance_xaudio2_, 0);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"XAudio2�I�u�W�F�N�g�̍쐬�Ɏ��s�I", L"�x���I", MB_ICONWARNING);

		// COM���C�u�����̏I������
		CoUninitialize();

		return false;
	}

	// �}�X�^�[�{�C�X�̐���
	hr = instance_xaudio2_->CreateMasteringVoice(&mastering_voice_);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"�}�X�^�[�{�C�X�̐����Ɏ��s�I", L"�x���I", MB_ICONWARNING);

		SAFE_RELEASE(instance_xaudio2_);

		// COM���C�u�����̏I������
		CoUninitialize();

		return false;
	}

	// �T�E���h�f�[�^�̏�����
	for (int count = 0; count < eSoundMax; ++count)
	{
		HANDLE file;
		DWORD chunk_size = 0;
		DWORD chunk_position = 0;
		DWORD file_type;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// �o�b�t�@�̃N���A
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// �T�E���h�f�[�^�t�@�C���̐���
		file = CreateFile(paramaters_[count].file_path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (file == INVALID_HANDLE_VALUE)
		{
			//MessageBox(NULL, L"�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(1)", L"�x���I", MB_ICONWARNING);
			return false;
		}
		if (SetFilePointer(file, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// �t�@�C���|�C���^��擪�Ɉړ�
			//MessageBox(NULL, L"�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(2)", L"�x���I", MB_ICONWARNING);
			return false;
		}

		// WAVE�t�@�C���̃`�F�b�N
		if (!CheckChunk(file, 'FFIR', chunk_size, chunk_position))
		{
			//MessageBox(NULL, L"WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(1)", L"�x���I", MB_ICONWARNING);
			return false;
		}

		if (!ReadChunkData(file, &file_type, sizeof(DWORD), chunk_position))
		{
			//MessageBox(NULL, L"WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(2)", L"�x���I", MB_ICONWARNING);
			return false;
		}
		if (file_type != 'EVAW')
		{
			//MessageBox(NULL, L"WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(3)", L"�x���I", MB_ICONWARNING);
			return false;
		}

		// �t�H�[�}�b�g�`�F�b�N
		if (!CheckChunk(file, ' tmf', chunk_size, chunk_position))
		{
			//MessageBox(NULL, L"�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(1)", L"�x���I", MB_ICONWARNING);
			return false;
		}

		if (!ReadChunkData(file, &wfx, chunk_size, chunk_position))
		{
			//MessageBox(NULL, L"�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(2)", L"�x���I", MB_ICONWARNING);
			return false;
		}

		// �I�[�f�B�I�f�[�^�ǂݍ���
		if (!CheckChunk(file, 'atad', audio_sizes_[count], chunk_position))
		{
			//MessageBox(NULL, L"�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(1)", L"�x���I", MB_ICONWARNING);
			return false;
		}

		audio_datas_[count] = (BYTE*)malloc(audio_sizes_[count]);
		if (!ReadChunkData(file, audio_datas_[count], audio_sizes_[count], chunk_position))
		{
			//MessageBox(NULL, L"�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(2)", L"�x���I", MB_ICONWARNING);
			return false;
		}

		// �\�[�X�{�C�X�̐���
		if (!instance_xaudio2_->CreateSourceVoice(&source_voices_[count], &(wfx.Format)))
		{
			//MessageBox(NULL, L"�\�[�X�{�C�X�̐����Ɏ��s�I", L"�x���I", MB_ICONWARNING);
			return false;
		}

		// �o�b�t�@�̒l�ݒ�
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = audio_sizes_[count];
		buffer.pAudioData = audio_datas_[count];
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount = paramaters_[count].count_loop;

		// �I�[�f�B�I�o�b�t�@�̓o�^
		source_voices_[count]->SubmitSourceBuffer(&buffer);
	}

	return true;
}

//--------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------
void SoundManager::Uninit(void)
{
	for (int count = 0; count < eSoundMax; ++count)
	{
		if (source_voices_[count])
		{
			// �ꎞ��~
			source_voices_[count]->Stop(0);

			// �\�[�X�{�C�X�̔j��
			source_voices_[count]->DestroyVoice();
			source_voices_[count] = nullptr;

			// �I�[�f�B�I�f�[�^�̊J��
			free(audio_datas_[count]);
			audio_datas_[count] = nullptr;
		}
	}

	// �}�X�^�[�{�C�X�̔j��
	mastering_voice_->DestroyVoice();
	mastering_voice_ = nullptr;

	// XAudio2�I�u�W�F�N�g�̉��
	SAFE_RELEASE(instance_xaudio2_);

	// COM���C�u�����̏I������
	CoUninitialize();
}

//--------------------------------------------------------------------------------
// �`�����N�̃`�F�b�N
//--------------------------------------------------------------------------------
bool SoundManager::CheckChunk(HANDLE file, DWORD format, DWORD& chunk_size, DWORD& chunk_data_position)
{
	bool result = true;
	DWORD read;
	DWORD chunk_type;
	DWORD chunk_data_size;
	DWORD ffir_data_size = 0;
	DWORD file_type;
	DWORD bytesRead = 0;
	DWORD offset = 0;

	if (SetFilePointer(file, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// �t�@�C���|�C���^��擪�Ɉړ�
		return false;
	}

	while (result)
	{
		if (ReadFile(file, &chunk_type, sizeof(DWORD), &read, NULL) == 0)
		{// �`�����N�̓ǂݍ���
			result = false;
		}

		if (ReadFile(file, &chunk_data_size, sizeof(DWORD), &read, NULL) == 0)
		{// �`�����N�f�[�^�̓ǂݍ���
			result = false;
		}

		switch (chunk_type)
		{
		case 'FFIR':
			ffir_data_size = chunk_data_size;
			chunk_data_size = 4;
			if (ReadFile(file, &file_type, sizeof(DWORD), &read, NULL) == 0)
			{// �t�@�C���^�C�v�̓ǂݍ���
				result = false;
			}
			break;

		default:
			if (SetFilePointer(file, chunk_data_size, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// �t�@�C���|�C���^���`�����N�f�[�^���ړ�
				return false;
			}
		}

		offset += sizeof(DWORD) * 2;
		if (chunk_type == format)
		{
			chunk_size = chunk_data_size;
			chunk_data_position = offset;
			return true;
		}

		offset += chunk_data_size;
		if (bytesRead >= ffir_data_size)
		{
			return false;
		}
	}

	return true;
}

//--------------------------------------------------------------------------------
// �`�����N�f�[�^�̓ǂݍ���
//--------------------------------------------------------------------------------
bool SoundManager::ReadChunkData(HANDLE file, void *buffer, DWORD buffer_size, DWORD buffer_offset)
{
	DWORD read;

	if (SetFilePointer(file, buffer_offset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// �t�@�C���|�C���^���w��ʒu�܂ňړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (ReadFile(file, buffer, buffer_size, &read, NULL) == 0)
	{// �f�[�^�̓ǂݍ���
		return HRESULT_FROM_WIN32(GetLastError());
	}

	return S_OK;
}