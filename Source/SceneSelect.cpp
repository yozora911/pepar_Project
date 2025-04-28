#include "SceneSelect.h"
#include "System/Graphics.h"
#include "System/Input.h"
#include "Camera.h"
#include "SceneManager.h"
#include "SceneGame.h"
#include "Collision.h"

//初期化
void SceneSelect::Initialize()
{
	//スプライト初期化
	select = new Sprite("Data/Sprite/Select_2.jpg");
	right = new Sprite(("Data/Sprite/rigth.jpg"));

	//ステージ初期化
	stage = new Stage_model();
	stage->SetPosition(DirectX::XMFLOAT3(0.0f, 0.0f, -0.0f));

	//カメラ初期設定
	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::Instance();
	camera.SetLookAt(
		DirectX::XMFLOAT3(0, 10, -10), //視点
		DirectX::XMFLOAT3(0, 0, 0), //注視点
		DirectX::XMFLOAT3(0, 1, 0) //上方向
	);
	camera.SetPerspectiveFov(
		DirectX::XMConvertToRadians(45), //視野角
		graphics.GetScreenWidth() / graphics.GetScreenHeight(), //画面アスペクト比
		0.1, //クリップ距離（近）
		1000.0f //クリップ距離（遠）
	);

	//カメラコントローラー初期化
	cameraController = new CameraController();
}

//終了化
void SceneSelect::Finalize()
{
	//カメラコントローラー終了化
	if (cameraController != nullptr)
	{
		delete cameraController;
		cameraController = nullptr;
	}

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

	//ステージ終了化
	if (stage != nullptr)
	{
		delete stage;
		stage = nullptr;
	}
}

//更新処理
void SceneSelect::Update(float elapsedTime)
{
	// カメラコントローラー更新処理
	DirectX::XMFLOAT3 target = stage->GetPosition();
	target.y += 0.5f;
	cameraController->SetTarget(target);
	cameraController->Update(elapsedTime);


	// マウス左クリックした
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		// スクリーンサイズ取得
		float screenWidth = Graphics::Instance().GetScreenWidth();
		float screenHeight = Graphics::Instance().GetScreenHeight();

		//マウスカーソル位置の取得
		POINT cursor;
		::GetCursorPos(&cursor);
		::ScreenToClient(Graphics::Instance().GetWindowHandle(), &cursor);

		Camera& camera = Camera::Instance();
		//各行列を取得
		DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&camera.GetView());
		DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&camera.GetProjection());
		DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

		//スクリーン座標の設定
		DirectX::XMVECTOR ScreenPosition, WorldPosition;
		DirectX::XMFLOAT3 screenPosition = { 0.0f, 0.0f, 0.0f };
		screenPosition.x = static_cast<float>(cursor.x);
		screenPosition.y = static_cast<float>(cursor.y);

		//スクリーン座標をワールド座標に変換し、レイの始点を求める
		//ビューポート
		float viewportX = 0.0f;
		float viewportY = 0.0f;
		float viewportW = screenWidth;
		float viewportH = screenHeight;
		float viewportMinZ = 0.0f;
		float viewportMaxZ = 1.0f;

		//スクリーン座標からNDC座標へ変換
		screenPosition.z = 0.0f;
		DirectX::XMVECTOR NDCPosition = DirectX::XMVectorSet(
			2.0f * (screenPosition.x - viewportX) / viewportW - 1.0f,
			1.0f - 2.0f * (screenPosition.y - viewportY) / viewportH,
			(screenPosition.z - viewportMinZ) / (viewportMaxZ - viewportMinZ),
			1.0f
		);


		//NDC座標からワールド座標へ変換
		DirectX::XMMATRIX WVP = World * View * Projection;//👉 これは「ワールド → NDC」への変換行列を作ってる！
		DirectX::XMMATRIX IWVP = DirectX::XMMatrixInverse(nullptr, WVP);//👉 上で作ったWVP行列を逆行列にして、		「NDC → ワールド」への逆変換行列を作ってる！
		WorldPosition = DirectX::XMVector3TransformCoord(NDCPosition, IWVP);//👉 そして、マウス位置から作った NDCPosition をこの逆行列で変換することで、📌「マウスが指してるワールド空間上の点」を求めてるわけ！

		DirectX::XMFLOAT3 rayStart;
		DirectX::XMStoreFloat3(&rayStart, WorldPosition);

		//スクリーン座標をワールド座標に変換し、レイの終点を求める
		screenPosition.z = 1.0f;

		NDCPosition = DirectX::XMVectorSet(
			2.0f * (screenPosition.x - viewportX) / viewportW - 1.0f,
			1.0f - 2.0f * (screenPosition.y - viewportY) / viewportH,
			(screenPosition.z - viewportMinZ) / (viewportMaxZ - viewportMinZ),
			1.0f);

		// 終点用にNDC → ワールド座標変換（Far側）
		WorldPosition = DirectX::XMVector3TransformCoord(NDCPosition, IWVP);

		DirectX::XMFLOAT3 rayEnd;
		DirectX::XMStoreFloat3(&rayEnd, WorldPosition);

		//モデルとレイキャストを行い、配置座標を求める
		DirectX::XMFLOAT3 hitPosition, hitNormal;
		if (Collision::RayCast(rayStart, rayEnd,
			stage->GetTransform(), stage->GetModel(), hitPosition, hitNormal))
		{
			SceneManger::Instance().ChangeScene(new SceneGame);
		}
	}

	GamePad& gamePad = Input::Instance().GetGamePad();

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

	float Rotate = RotateSpeed * elapsedTime;

	stage->SetAngle(DirectX::XMFLOAT3(-45.0f, stage->GetAngle().y + Rotate, 0.0f));

	stage->Update(elapsedTime);
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

	//カメラパラメーター設定
	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();

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
	}

	//3Dモデル描画
	{
		stage->Render(rc, modelRenderer);
	}
}

//GUI描画
void SceneSelect::DrawGUI()
{
}