#include "SceneManager.h"

//�X�V����
void SceneManger::Update(float elapsedTime)
{
	if (nextScene != nullptr)
	{
		//�Â��V�[�����I��
		Clear();

		//�V�����V�[����ݒ�
		currentScene = nextScene;
		nextScene = nullptr;

		//�V�[����������
		currentScene->Initialize();
	}
	if (currentScene != nullptr)
	{
		currentScene->Update(elapsedTime);
	}
}

//�`�揈��
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
	//�V�����V�[����ݒ�
	nextScene = scene;
}
