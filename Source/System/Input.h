#pragma once

#include <memory>
#include "System/GamePad.h"
#include "System/Mouse.h"

// �C���v�b�g
class Input
{
private:
	Input() = default;
	~Input() = default;

public:
	// �C���X�^���X�擾
	static Input& Instance()
	{
		static Input instance;
		return instance;
	}

	// ������
	void Initialize(HWND hWnd);

	// �X�V����
	void Update();

	// �Q�[���p�b�h�擾
	GamePad& GetGamePad() { return *gamePad; }

	// �}�E�X�擾
	Mouse& GetMouse() { return *mouse; }

private:
	std::unique_ptr<GamePad>	gamePad;
	std::unique_ptr<Mouse>		mouse;
};
