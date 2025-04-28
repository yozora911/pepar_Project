#pragma once
#include "System/Sprite.h"
#include "CameraController.h"
#include "stage_model.h"
#include "Scene.h"
#include <Windows.h>
#include <vector>

//セレクトシーン
class SceneSelect : public Scene
{
public:
	SceneSelect() {};
	~SceneSelect() {};

	//初期化
	void Initialize() override;

	//終了化
	void Finalize() override;

	//更新処理
	void Update(float elapsedTime) override;

	//描画処理
	void Render() override;

	//GUI描画
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
