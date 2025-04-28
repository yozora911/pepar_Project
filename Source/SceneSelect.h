#pragma once

#include "System/Sprite.h"
#include "Scene.h"

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
	Sprite* stage1 = nullptr;
	Sprite* stage2 = nullptr;

	bool isMoving = false;       // 移動アニメーション中かどうか
	float stage1PosX = 384.0f;   // stage1のX座標
	float stage2PosX = 1280.0f;  // stage2のX座標（追加）
	int moveDirection = 0;       // 移動方向 (-1:左, 1:右)（追加）
	float transitionSpeed = 800.0f; // 移動速度（追加）
	int nowStageIndex = 0;       // 現在表示中のステージ
	const int stageCount = 2;    // ステージの総数
};
