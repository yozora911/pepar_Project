#include"MapManager.h"
#include "System/Graphics.h"
#include<cmath>

void MapManager::init()
{
	//2次元リスト(床の生成)
	AddTile();
}

//tilesに保存されているタイルUpdate
void MapManager::Update(float elapsedTime)
{
	for (auto& row : tiles)
	{
		// 各タイルを解放
		for (Tile* tile : row)
		{
			tile->Update(elapsedTime);
		}
	}
	Mouse_move();

	UpdateTransform();

	model->UpdateTransform();
}

//マウスカーソルの位置がどこのグリッドをさしているのかを判定する処理
void MapManager::Mouse_move()
{
	// スクリーンサイズ取得
	float screenWidth = Graphics::Instance().GetScreenWidth();
	float screenHeight = Graphics::Instance().GetScreenHeight();


	// マウスカーソル位置の取得
	::GetCursorPos(&mouse_position);
	::ScreenToClient(Graphics::Instance().GetWindowHandle(), &mouse_position);
	DirectX::XMVECTOR ScreenPosition, WorldPosition;
	DirectX::XMFLOAT3 screenPosition;

	screenPosition.x = static_cast<float>(mouse_position.x);
	screenPosition.y = static_cast<float>(mouse_position.y);

	//スクリーン座標をワールド座標に変換し,例の視点を求める
	//ビューポート
	float viewportX = 0.0f;
	float viewportY = 0.0f;
	float viewportW = screenWidth;
	float viewportH = screenHeight;
	float viewportMinZ = 0.0f;
	float viewportMaxZ = 1.0f;

	//ビュー行列
	DirectX::XMMATRIX V = DirectX::XMLoadFloat4x4(&Camera::Instance().GetView());

	//プロジェクション行列
	DirectX::XMMATRIX P = DirectX::XMLoadFloat4x4(&Camera::Instance().GetProjection());

	//ワールド行列
	DirectX::XMMATRIX W = DirectX::XMMatrixIdentity();

	//NDC座標からワールド座標へ変換
	DirectX::XMMATRIX WVP = W * V * P;
	DirectX::XMMATRIX IWVP = DirectX::XMMatrixInverse(nullptr, WVP);

	switch (mouse_state)
	{
	case 0:
		//始点を求める
		WorldPosition = DirectX::XMLoadFloat3(&Camera::Instance().GetEye());

		DirectX::XMFLOAT3 rayStart;
		DirectX::XMStoreFloat3(&rayStart, WorldPosition);

		//スクリーン座標をワールド座標に変換し,レイの終点を求める
		DirectX::XMVECTOR NDCPosition2 = DirectX::XMVectorSet(
			2.0f * (screenPosition.x - viewportX) / viewportW - 1.0f,
			1.0f - 2.0f * (screenPosition.y - viewportY) / viewportH,
			1.0f,
			1.0f);
		WorldPosition = DirectX::XMVector3TransformCoord(NDCPosition2, IWVP);
		DirectX::XMFLOAT3 rayEnd;
		DirectX::XMStoreFloat3(&rayEnd, WorldPosition);

		DirectX::XMFLOAT3 hitPosition = { 0,0,0 };
		DirectX::XMFLOAT3 normal = { 0,0,0 };

		//頂点
		DirectX::XMFLOAT3 A = { 0,0,0 };
		DirectX::XMFLOAT3 B = { 0,0,25 };
		DirectX::XMFLOAT3 C = { 25,0,0 };
		DirectX::XMFLOAT3 D = { 25,0,25 };


		//マウスと床との交点
		if (Collision::SquareRayCast(rayStart, rayEnd, A, B, C, D, hitPosition, normal))
		{

			//グリッドの線の座標を決める
			float float_x = (hitPosition.x / 2.5f);
			float float_y = (hitPosition.y / 2.5f);
			float float_z = (hitPosition.z / 2.5f);

			int  int_x = (hitPosition.x / 2.5f);
			int  int_y = (hitPosition.y / 2.5f);
			int  int_z = (hitPosition.z / 2.5f);

			float result_x = std::round(float_x - int_x);
			float result_y = std::round(float_y - int_y);
			float result_z = std::round(float_z - int_z);

			if ((int_x + result_x) * 2.5f - hitPosition.x < (int_z + result_z) * 2.5f - hitPosition.z)
			{
				glid_position.x = (int_x + result_x) * 2.5f;
				glid_position.z = 0;
				angle.y = DirectX::XMConvertToRadians(90);
			}
			else
			{
				glid_position.z = (int_z + result_z) * 2.5f;
				glid_position.x = 0;
				angle.y = 0;
			}
			glid_position.y = (int_y + result_y) * 2.5f;
			position = glid_position;
			if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0)
			{
				mouse_state++;
			}
		}
		break;
	case 1:
		//折り目の位置を指定
		if (angle.y == 0)
		{
			//折り目に面している面だけを曲げる
			for (int x = 0; x < WIDTH; x++)
			{
				DirectX::XMFLOAT3 angle = { DirectX::XMConvertToRadians(90),0,0};
				DirectX::XMFLOAT3 position = tiles[int(glid_position.z / 2.5f)][x]->GetPosition();
				DirectX::XMFLOAT3 scale = tiles[int(glid_position.z / 2.5f)][x]->GetScale();
				tiles[int(glid_position.z / 2.5f)][x]->Set_local(scale,angle,position);
			}

			//折り目に面しているワールド行列をもとにその他のタイルも変える
			for (int z = glid_position.z / 2.5f+1; z < DEPTH ; z++)
			{
				for (int x = 0; x < WIDTH; x++)
				{
					DirectX::XMFLOAT3 position = { 0,0,1 };
					DirectX::XMFLOAT3 angle = { 0,0,0 };
					DirectX::XMFLOAT3 scale = { 1,1,1 };
					tiles[z][x]->Set_local(scale, angle, position);
					tiles[z][x]->Global_Transform(tiles[z - 1][x]->GetTransform());
				}
			}
		}
		if (angle.y == DirectX::XMConvertToRadians(90))
		{
			//折り目に面している面だけを曲げる
			for (int z = 0; z < DEPTH; z++)
			{
				DirectX::XMFLOAT3 angle = { 0,0,-DirectX::XMConvertToRadians(90) };
				DirectX::XMFLOAT3 position = tiles[z][glid_position.x / 2.5f]->GetPosition();
				DirectX::XMFLOAT3 scale = tiles[z][glid_position.x / 2.5f]->GetScale();
				tiles[z][glid_position.x / 2.5f]->Set_local(scale, angle, position);
			}


			//折り目に面しているワールド行列をもとにその他のタイルも変える
			//for (int x = glid_position.x / 2.5f - 1; 0 <= x; x--)
			//{
			//	for (int z = 0;z < DEPTH;z++)
			//	{
			//		DirectX::XMFLOAT3 position = {-1,0,0};
			//		DirectX::XMFLOAT3 angle = { 0,0,0 };
			//		DirectX::XMFLOAT3 scale = { 1,1,1 };
			//		tiles[z][x]->Set_local(scale, angle, position);
			//		tiles[z][x]->Global_Transform(tiles[z][x+1]->GetTransform());
			//	}
			//}
		}

		break;
	}
}

//tilesに保存されているタイルのRender
void MapManager::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	for (auto& row : tiles)
	{
		// 各タイルを解放
		for (Tile* tile : row)
		{
			tile->Render(rc, renderer);
		}
	}
	renderer->Render(rc, transform, model, ShaderId::Lambert);
}

//tileにマップチップのデータに合わせてセット
void MapManager::AddTile()
{
	for (int z = 0; z < DEPTH; z++)
	{
		// z が tiles のサイズより大きい場合、行を追加
		if (tiles.size() <= z)
		{
			tiles.push_back(std::vector<Tile*>()); // 新しい行を追加
		}

		for (int x = 0; x < WIDTH; x++)
		{
			tiles[z].emplace_back(new Normal_Tile(2.5f * x, 2.5f * z));
		}
	}
}

void MapManager::Clear()
{
	// 各行（std::vector<Tile*>）に対して
	for (auto& row : tiles)
	{
		// 各タイルを解放
		for (Tile* tile : row)
		{
			delete tile;
		}
		row.clear();  // 各行のタイルのポインタを解放した後、行のベクターもクリア
	}
	tiles.clear();  // tilesの2次元ベクターを完全にクリア
}
