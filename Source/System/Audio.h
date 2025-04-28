#pragma once

#include <xaudio2.h>
#include "System/AudioSource.h"

// �I�[�f�B�I
class Audio
{
private:
	Audio() = default;
	~Audio() = default;

public:
	// �C���X�^���X�擾
	static Audio& Instance()
	{
		static Audio instance;
		return instance;
	}

	// ������
	void Initialize();

	// �I����
	void Finalize();

	// �I�[�f�B�I�\�[�X�ǂݍ���
	AudioSource* LoadAudioSource(const char* filename);

private:
	IXAudio2*				xaudio = nullptr;
	IXAudio2MasteringVoice* masteringVoice = nullptr;
};
