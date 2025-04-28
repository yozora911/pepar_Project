#pragma once
#include"Map.h"
#include"Normal_Tile.h"
#include<vector>
#include"Collision.h"
#include<windows.h>
#include"Camera.h"
#include"Character.h"
class MapManager:Character
{
public:
	//シングルトン
	static MapManager& Instance()
	{
		static MapManager instance;
		return instance;
	}

	void init();

	void Update(float elapsedTime);

	void Mouse_move();

	void Render(const RenderContext& rc, ModelRenderer* renderer);

	void AddTile();

	void Clear();

private:
	MapManager() {
		model = new Model("Data/Model/grid/Grid.mdl");

		// モデルが大きいのでスケーリング
		scale.x = scale.y = scale.z = 0.01f;
		init(); 
	}
	~MapManager() { delete model; }

	//マップ
	std::vector<Map> maps;

	//マップに合わせたタイル
	std::vector<std::vector<Tile*>>tiles;

	//マウスの位置
	POINT mouse_position;

	//グリッドのフラグ
	bool Grid_flag;

	//モデル
	Model* model;

	//マウスステート
	int mouse_state = 0;

	//グリッドの位置
	DirectX::XMFLOAT3 glid_position = { 0,0,0 };
};