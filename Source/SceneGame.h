#pragma once

#include "Stage.h"
#include "Scene.h"
#include"CameraController.h"
// �Q�[���V�[��
class SceneGame : public Scene
{
public:
	SceneGame() {};
	~SceneGame() override {};

	// ������
	void Initialize()override;

	// �I����
	void Finalize()override;

	// �X�V����
	void Update(float elapsedTime)override;

	// �`�揈��
	void Render()override;

	// GUI�`��
	void DrawGUI()override;

private:
	Stage* stage = nullptr;
	CameraController* cameraController = nullptr;
};
