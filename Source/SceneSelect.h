#pragma once
#include "System/Sprite.h"
#include "CameraController.h"
#include "stage_model.h"
#include "Scene.h"
#include <Windows.h>
#include <vector>

//�Z���N�g�V�[��
class SceneSelect : public Scene
{
public:
	SceneSelect() {};
	~SceneSelect() {};

	//������
	void Initialize() override;

	//�I����
	void Finalize() override;

	//�X�V����
	void Update(float elapsedTime) override;

	//�`�揈��
	void Render() override;

	//GUI�`��
	void DrawGUI() override;

private:
	Sprite* select = nullptr;
	Sprite* right = nullptr;
	Stage_model* stage = nullptr;
	CameraController* cameraController = nullptr;

	float RotateSpeed = 1.0f;
	std::vector<Stage_model*> stages;

	int SelectIndex = 0;
};
