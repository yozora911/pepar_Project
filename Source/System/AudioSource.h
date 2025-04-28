#pragma once

#include <memory>
#include <xaudio2.h>
#include "System/AudioResource.h"

// �I�[�f�B�I�\�[�X
class AudioSource
{
public:
	AudioSource(IXAudio2* xaudio, std::shared_ptr<AudioResource>& resource);
	~AudioSource();

	// �Đ�
	void Play(bool loop);

	// ��~
	void Stop();

	// ���ʐݒ�
	void SetVolume(float volume);

private:
	IXAudio2SourceVoice*			sourceVoice = nullptr;
	std::shared_ptr<AudioResource>	resource;
};
