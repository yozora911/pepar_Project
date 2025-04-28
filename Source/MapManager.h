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
	//�V���O���g��
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

		// ���f�����傫���̂ŃX�P�[�����O
		scale.x = scale.y = scale.z = 0.01f;
		init(); 
	}
	~MapManager() { delete model; }

	//�}�b�v
	std::vector<Map> maps;

	//�}�b�v�ɍ��킹���^�C��
	std::vector<std::vector<Tile*>>tiles;

	//�}�E�X�̈ʒu
	POINT mouse_position;

	//�O���b�h�̃t���O
	bool Grid_flag;

	//���f��
	Model* model;

	//�}�E�X�X�e�[�g
	int mouse_state = 0;

	//�O���b�h�̈ʒu
	DirectX::XMFLOAT3 glid_position = { 0,0,0 };
};