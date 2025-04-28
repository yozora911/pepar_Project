#include "System/Input.h"

// ������
void Input::Initialize(HWND hWnd)
{
	gamePad = std::make_unique<GamePad>();
	mouse = std::make_unique<Mouse>(hWnd);
}

// �X�V����
void Input::Update()
{
	gamePad->Update();
	mouse->Update();
}
