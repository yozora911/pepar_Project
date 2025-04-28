#include "SceneSelect.h"
#include "System/Graphics.h"
#include "System/Input.h"
#include "SceneManager.h"
#include "SceneGame.h"

//初期化
void SceneSelect::Initialize()
{
	//スプライト初期化
	select = new Sprite("Data/Sprite/Select_2.jpg");
	right = new Sprite(("Data/Sprite/rigth.jpg"));
	stage1 = new Sprite(("Data/Sprite/stage1.jpg"));
	stage2 = new Sprite(("Data/Sprite/stage2.jpg"));

	// ステージ位置の初期化
	stage1PosX = 384.0f;
	stage2PosX = 1280.0f;  // 画面外右側に配置
	nowStageIndex = 0;
	isMoving = false;
	moveDirection = 0;
	transitionSpeed = 800.0f;
}

//終了化
void SceneSelect::Finalize()
{
	//スプライト終了化
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

//更新処理
void SceneSelect::Update(float elapsedTime)
{
	GamePad& gamePad = Input::Instance().GetGamePad();
	Input& input = Input::Instance();
	int mouseX = Input::Instance().GetMouse().GetPositionX();
	int mouseY = Input::Instance().GetMouse().GetPositionY();
	MouseButton btn = input.GetMouse().GetButtonDown();

	// アニメーション中でなければクリック処理を行う
	if (!isMoving)
	{
		if (btn & Mouse::BTN_LEFT)
		{
			// 右矢印クリック
			if (mouseX >= 1216 && mouseX <= 1280 && mouseY >= 600 && mouseY <= 664)
			{
				isMoving = true;
				moveDirection = 1; // 右方向

				// 次のステージを設定 (現在のステージは右に、次のステージは左から)
				int nextStageIndex = (nowStageIndex + 1) % stageCount;
				if (nextStageIndex == 0) {
					stage2PosX = 384.0f; // 現在表示中
					stage1PosX = -512.0f; // 左側から入ってくる準備
				}
				else {
					stage1PosX = 384.0f; // 現在表示中
					stage2PosX = -512.0f; // 左側から入ってくる準備
				}
			}
			// 左矢印クリック
			else if (mouseX >= 0 && mouseX <= 64 && mouseY >= 600 && mouseY <= 664)
			{
				isMoving = true;
				moveDirection = -1; // 左方向

				// 次のステージを設定 (現在のステージは左に、次のステージは右から)
				int nextStageIndex = (nowStageIndex + 1) % stageCount;
				if (nextStageIndex == 0) {
					stage2PosX = 384.0f; // 現在表示中
					stage1PosX = 1280.0f; // 右側から入ってくる準備
				}
				else {
					stage1PosX = 384.0f; // 現在表示中
					stage2PosX = 1280.0f; // 右側から入ってくる準備
				}
			}
			// ステージ画像クリック
			else if (mouseX >= 384 && mouseX <= 896 && mouseY >= 10 && mouseY <= 522)
			{
				SceneManger::Instance().ChangeScene(new SceneGame);
			}
		}
	}

	// ステージ移動アニメーション処理
	if (isMoving)
	{
		float moveAmount = transitionSpeed * elapsedTime;

		if (moveDirection > 0) // 右方向への移動（現在のステージが右に出て、次のが左から入る）
		{
			if (nowStageIndex == 0) // stage1が表示中
			{
				// stage1を右に、stage2を右に移動
				stage1PosX += moveAmount;
				stage2PosX += moveAmount;

				// 移動終了判定
				if (stage2PosX >= 384.0f)
				{
					stage2PosX = 384.0f; // 位置を確定
					isMoving = false;
					nowStageIndex = 1; // stage2に切り替え
				}
			}
			else // stage2が表示中
			{
				// stage2を右に、stage1を右に移動
				stage2PosX += moveAmount;
				stage1PosX += moveAmount;

				// 移動終了判定
				if (stage1PosX >= 384.0f)
				{
					stage1PosX = 384.0f; // 位置を確定
					isMoving = false;
					nowStageIndex = 0; // stage1に切り替え
				}
			}
		}
		else if (moveDirection < 0) // 左方向への移動（現在のステージが左に出て、次のが右から入る）
		{
			if (nowStageIndex == 0) // stage1が表示中
			{
				// stage1を左に、stage2を左に移動
				stage1PosX -= moveAmount;
				stage2PosX -= moveAmount;

				// 移動終了判定
				if (stage2PosX <= 384.0f)
				{
					stage2PosX = 384.0f; // 位置を確定
					isMoving = false;
					nowStageIndex = 1; // stage2に切り替え
				}
			}
			else // stage2が表示中
			{
				// stage2を左に、stage1を左に移動
				stage2PosX -= moveAmount;
				stage1PosX -= moveAmount;

				// 移動終了判定
				if (stage1PosX <= 384.0f)
				{
					stage1PosX = 384.0f; // 位置を確定
					isMoving = false;
					nowStageIndex = 0; // stage1に切り替え
				}
			}
		}
	}

	// なにかボタンを押したらゲームシーンへ切り替え（デバッグ用）
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

//描画処理
void SceneSelect::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ShapeRenderer* shapeRenderer = graphics.GetShapeRenderer();
	ModelRenderer* modelRenderer = graphics.GetModelRenderer();

	// 描画準備
	RenderContext rc;
	rc.deviceContext = dc;
	rc.lightDirection = { 0.0f, -1.0f, 0.0f };	// ライト方向（下方向）
	rc.renderState = graphics.GetRenderState();

	//2Dスプライト描画
	{
		//背景描画
		float screenWidth = static_cast<float>(graphics.GetScreenWidth());
		float screenHeight = static_cast<float>(graphics.GetScreenHeight());
		select->Render(rc,
			0, 0, 0, screenWidth, screenHeight,
			0,
			1, 1, 1, 1);

		//右矢印描画
		right->Render(rc,
			1216, 600, 0,
			64, 64,
			0,
			1.0f, 1.0f, 1.0f, 1.0f);

		//左矢印描画
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

//GUI描画
void SceneSelect::DrawGUI()
{
}