#pragma once

#include "Scene.h"

//�V�[���}�l�[�W���[
class SceneManger
{
	SceneManger() {};
	~SceneManger() {};

public:
	//�B��̃C���X�^���X�擾
	static SceneManger& Instance()
	{
		static SceneManger instance;
		return instance;
	}

	//�X�V����
	void Update(float elapsedTime);

	//�`�揈��
	void Render();

	//GUI�`��
	void DrawGUI();

	//�V�[���N���A
	void Clear();

	//�V�[���؂�ւ�
	void ChangeScene(Scene* scene);

private:
	Scene* currentScene = nullptr;
	Scene* nextScene = nullptr;
};
