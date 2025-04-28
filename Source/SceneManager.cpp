#include "SceneManager.h"

//更新処理
void SceneManger::Update(float elapsedTime)
{
	if (nextScene != nullptr)
	{
		//古いシーンを終了
		Clear();

		//新しいシーンを設定
		currentScene = nextScene;
		nextScene = nullptr;

		//シーンを初期化
		currentScene->Initialize();
	}
	if (currentScene != nullptr)
	{
		currentScene->Update(elapsedTime);
	}
}

//描画処理
void SceneManger::Render()
{
	if (currentScene != nullptr)
	{
		currentScene->Render();
	}
}

void SceneManger::DrawGUI()
{
	if (currentScene != nullptr)
	{
		currentScene->DrawGUI();
	}
}

void SceneManger::Clear()
{
	if (currentScene != nullptr)
	{
		currentScene->Finalize();
		delete currentScene;
		currentScene = nullptr;
	}
}

void SceneManger::ChangeScene(Scene* scene)
{
	//新しいシーンを設定
	nextScene = scene;
}
