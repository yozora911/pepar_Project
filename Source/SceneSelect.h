#pragma once

#include "System/Sprite.h"
#include "Scene.h"

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
	Sprite* stage1 = nullptr;
	Sprite* stage2 = nullptr;

	bool isMoving = false;       // �ړ��A�j���[�V���������ǂ���
	float stage1PosX = 384.0f;   // stage1��X���W
	float stage2PosX = 1280.0f;  // stage2��X���W�i�ǉ��j
	int moveDirection = 0;       // �ړ����� (-1:��, 1:�E)�i�ǉ��j
	float transitionSpeed = 800.0f; // �ړ����x�i�ǉ��j
	int nowStageIndex = 0;       // ���ݕ\�����̃X�e�[�W
	const int stageCount = 2;    // �X�e�[�W�̑���
};
