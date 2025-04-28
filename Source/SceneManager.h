#pragma once

#include "Scene.h"

//シーンマネージャー
class SceneManger
{
	SceneManger() {};
	~SceneManger() {};

public:
	//唯一のインスタンス取得
	static SceneManger& Instance()
	{
		static SceneManger instance;
		return instance;
	}

	//更新処理
	void Update(float elapsedTime);

	//描画処理
	void Render();

	//GUI描画
	void DrawGUI();

	//シーンクリア
	void Clear();

	//シーン切り替え
	void ChangeScene(Scene* scene);

private:
	Scene* currentScene = nullptr;
	Scene* nextScene = nullptr;
};
