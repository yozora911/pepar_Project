#include "SceneSelect.h"
#include "System/Graphics.h"
#include "System/Input.h"
#include "SceneManager.h"
#include "SceneGame.h"

//������
void SceneSelect::Initialize()
{
	//�X�v���C�g������
	select = new Sprite("Data/Sprite/Select_2.jpg");
	right = new Sprite(("Data/Sprite/rigth.jpg"));
	stage1 = new Sprite(("Data/Sprite/stage1.jpg"));
	stage2 = new Sprite(("Data/Sprite/stage2.jpg"));

	// �X�e�[�W�ʒu�̏�����
	stage1PosX = 384.0f;
	stage2PosX = 1280.0f;  // ��ʊO�E���ɔz�u
	nowStageIndex = 0;
	isMoving = false;
	moveDirection = 0;
	transitionSpeed = 800.0f;
}

//�I����
void SceneSelect::Finalize()
{
	//�X�v���C�g�I����
	if (select != nullptr)
	{
		delete select;
		select = nullptr;
	}
	if (right != nullptr)
	{
		delete right;
		right = nullptr;
	}
	if (stage1 != nullptr)
	{
		delete stage1;
		stage1 = nullptr;
	}
	if (stage2 != nullptr)
	{
		delete stage2;
		stage2 = nullptr;
	}
}

//�X�V����
void SceneSelect::Update(float elapsedTime)
{
	GamePad& gamePad = Input::Instance().GetGamePad();
	Input& input = Input::Instance();
	int mouseX = Input::Instance().GetMouse().GetPositionX();
	int mouseY = Input::Instance().GetMouse().GetPositionY();
	MouseButton btn = input.GetMouse().GetButtonDown();

	// �A�j���[�V�������łȂ���΃N���b�N�������s��
	if (!isMoving)
	{
		if (btn & Mouse::BTN_LEFT)
		{
			// �E���N���b�N
			if (mouseX >= 1216 && mouseX <= 1280 && mouseY >= 600 && mouseY <= 664)
			{
				isMoving = true;
				moveDirection = 1; // �E����

				// ���̃X�e�[�W��ݒ� (���݂̃X�e�[�W�͉E�ɁA���̃X�e�[�W�͍�����)
				int nextStageIndex = (nowStageIndex + 1) % stageCount;
				if (nextStageIndex == 0) {
					stage2PosX = 384.0f; // ���ݕ\����
					stage1PosX = -512.0f; // ������������Ă��鏀��
				}
				else {
					stage1PosX = 384.0f; // ���ݕ\����
					stage2PosX = -512.0f; // ������������Ă��鏀��
				}
			}
			// �����N���b�N
			else if (mouseX >= 0 && mouseX <= 64 && mouseY >= 600 && mouseY <= 664)
			{
				isMoving = true;
				moveDirection = -1; // ������

				// ���̃X�e�[�W��ݒ� (���݂̃X�e�[�W�͍��ɁA���̃X�e�[�W�͉E����)
				int nextStageIndex = (nowStageIndex + 1) % stageCount;
				if (nextStageIndex == 0) {
					stage2PosX = 384.0f; // ���ݕ\����
					stage1PosX = 1280.0f; // �E����������Ă��鏀��
				}
				else {
					stage1PosX = 384.0f; // ���ݕ\����
					stage2PosX = 1280.0f; // �E����������Ă��鏀��
				}
			}
			// �X�e�[�W�摜�N���b�N
			else if (mouseX >= 384 && mouseX <= 896 && mouseY >= 10 && mouseY <= 522)
			{
				SceneManger::Instance().ChangeScene(new SceneGame);
			}
		}
	}

	// �X�e�[�W�ړ��A�j���[�V��������
	if (isMoving)
	{
		float moveAmount = transitionSpeed * elapsedTime;

		if (moveDirection > 0) // �E�����ւ̈ړ��i���݂̃X�e�[�W���E�ɏo�āA���̂����������j
		{
			if (nowStageIndex == 0) // stage1���\����
			{
				// stage1���E�ɁAstage2���E�Ɉړ�
				stage1PosX += moveAmount;
				stage2PosX += moveAmount;

				// �ړ��I������
				if (stage2PosX >= 384.0f)
				{
					stage2PosX = 384.0f; // �ʒu���m��
					isMoving = false;
					nowStageIndex = 1; // stage2�ɐ؂�ւ�
				}
			}
			else // stage2���\����
			{
				// stage2���E�ɁAstage1���E�Ɉړ�
				stage2PosX += moveAmount;
				stage1PosX += moveAmount;

				// �ړ��I������
				if (stage1PosX >= 384.0f)
				{
					stage1PosX = 384.0f; // �ʒu���m��
					isMoving = false;
					nowStageIndex = 0; // stage1�ɐ؂�ւ�
				}
			}
		}
		else if (moveDirection < 0) // �������ւ̈ړ��i���݂̃X�e�[�W�����ɏo�āA���̂��E�������j
		{
			if (nowStageIndex == 0) // stage1���\����
			{
				// stage1�����ɁAstage2�����Ɉړ�
				stage1PosX -= moveAmount;
				stage2PosX -= moveAmount;

				// �ړ��I������
				if (stage2PosX <= 384.0f)
				{
					stage2PosX = 384.0f; // �ʒu���m��
					isMoving = false;
					nowStageIndex = 1; // stage2�ɐ؂�ւ�
				}
			}
			else // stage2���\����
			{
				// stage2�����ɁAstage1�����Ɉړ�
				stage2PosX -= moveAmount;
				stage1PosX -= moveAmount;

				// �ړ��I������
				if (stage1PosX <= 384.0f)
				{
					stage1PosX = 384.0f; // �ʒu���m��
					isMoving = false;
					nowStageIndex = 0; // stage1�ɐ؂�ւ�
				}
			}
		}
	}

	// �Ȃɂ��{�^������������Q�[���V�[���֐؂�ւ��i�f�o�b�O�p�j
	const GamePadButton anyButton =
		GamePad::BTN_A
		| GamePad::BTN_B
		| GamePad::BTN_X
		| GamePad::BTN_Y
		;
	if (gamePad.GetButtonDown() & anyButton)
	{
		SceneManger::Instance().ChangeScene(new SceneGame);
	}
}

//�`�揈��
void SceneSelect::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ShapeRenderer* shapeRenderer = graphics.GetShapeRenderer();
	ModelRenderer* modelRenderer = graphics.GetModelRenderer();

	// �`�揀��
	RenderContext rc;
	rc.deviceContext = dc;
	rc.lightDirection = { 0.0f, -1.0f, 0.0f };	// ���C�g�����i�������j
	rc.renderState = graphics.GetRenderState();

	//2D�X�v���C�g�`��
	{
		//�w�i�`��
		float screenWidth = static_cast<float>(graphics.GetScreenWidth());
		float screenHeight = static_cast<float>(graphics.GetScreenHeight());
		select->Render(rc,
			0, 0, 0, screenWidth, screenHeight,
			0,
			1, 1, 1, 1);

		//�E���`��
		right->Render(rc,
			1216, 600, 0,
			64, 64,
			0,
			1.0f, 1.0f, 1.0f, 1.0f);

		//�����`��
		right->Render(rc,
			0, 600, 0,
			64, 64,
			180,
			1.0f, 1.0f, 1.0f, 1.0f);

		stage1->Render(rc,
			static_cast<int>(stage1PosX), 10, 0,
			512, 512,
			0,
			1.0f, 1.0f, 1.0f, 1.0f);

		stage2->Render(rc,
			static_cast<int>(stage2PosX), 10, 0,
			512, 512,
			0,
			1.0f, 1.0f, 1.0f, 1.0f);
	}
}

//GUI�`��
void SceneSelect::DrawGUI()
{
}