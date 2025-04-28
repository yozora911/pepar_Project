#include"MapManager.h"
#include "System/Graphics.h"
#include<cmath>

void MapManager::init()
{
	//2�������X�g(���̐���)
	AddTile();
}

//tiles�ɕۑ�����Ă���^�C��Update
void MapManager::Update(float elapsedTime)
{
	for (auto& row : tiles)
	{
		// �e�^�C�������
		for (Tile* tile : row)
		{
			tile->Update(elapsedTime);
		}
	}
	Mouse_move();

	UpdateTransform();

	model->UpdateTransform();
}

//�}�E�X�J�[�\���̈ʒu���ǂ��̃O���b�h�������Ă���̂��𔻒肷�鏈��
void MapManager::Mouse_move()
{
	// �X�N���[���T�C�Y�擾
	float screenWidth = Graphics::Instance().GetScreenWidth();
	float screenHeight = Graphics::Instance().GetScreenHeight();


	// �}�E�X�J�[�\���ʒu�̎擾
	::GetCursorPos(&mouse_position);
	::ScreenToClient(Graphics::Instance().GetWindowHandle(), &mouse_position);
	DirectX::XMVECTOR ScreenPosition, WorldPosition;
	DirectX::XMFLOAT3 screenPosition;

	screenPosition.x = static_cast<float>(mouse_position.x);
	screenPosition.y = static_cast<float>(mouse_position.y);

	//�X�N���[�����W�����[���h���W�ɕϊ���,��̎��_�����߂�
	//�r���[�|�[�g
	float viewportX = 0.0f;
	float viewportY = 0.0f;
	float viewportW = screenWidth;
	float viewportH = screenHeight;
	float viewportMinZ = 0.0f;
	float viewportMaxZ = 1.0f;

	//�r���[�s��
	DirectX::XMMATRIX V = DirectX::XMLoadFloat4x4(&Camera::Instance().GetView());

	//�v���W�F�N�V�����s��
	DirectX::XMMATRIX P = DirectX::XMLoadFloat4x4(&Camera::Instance().GetProjection());

	//���[���h�s��
	DirectX::XMMATRIX W = DirectX::XMMatrixIdentity();

	//NDC���W���烏�[���h���W�֕ϊ�
	DirectX::XMMATRIX WVP = W * V * P;
	DirectX::XMMATRIX IWVP = DirectX::XMMatrixInverse(nullptr, WVP);

	switch (mouse_state)
	{
	case 0:
		//�n�_�����߂�
		WorldPosition = DirectX::XMLoadFloat3(&Camera::Instance().GetEye());

		DirectX::XMFLOAT3 rayStart;
		DirectX::XMStoreFloat3(&rayStart, WorldPosition);

		//�X�N���[�����W�����[���h���W�ɕϊ���,���C�̏I�_�����߂�
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

		//���_
		DirectX::XMFLOAT3 A = { 0,0,0 };
		DirectX::XMFLOAT3 B = { 0,0,25 };
		DirectX::XMFLOAT3 C = { 25,0,0 };
		DirectX::XMFLOAT3 D = { 25,0,25 };


		//�}�E�X�Ə��Ƃ̌�_
		if (Collision::SquareRayCast(rayStart, rayEnd, A, B, C, D, hitPosition, normal))
		{

			//�O���b�h�̐��̍��W�����߂�
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
		//�܂�ڂ̈ʒu���w��
		if (angle.y == 0)
		{
			//�܂�ڂɖʂ��Ă���ʂ������Ȃ���
			for (int x = 0; x < WIDTH; x++)
			{
				DirectX::XMFLOAT3 angle = { DirectX::XMConvertToRadians(90),0,0};
				DirectX::XMFLOAT3 position = tiles[int(glid_position.z / 2.5f)][x]->GetPosition();
				DirectX::XMFLOAT3 scale = tiles[int(glid_position.z / 2.5f)][x]->GetScale();
				tiles[int(glid_position.z / 2.5f)][x]->Set_local(scale,angle,position);
			}

			//�܂�ڂɖʂ��Ă��郏�[���h�s������Ƃɂ��̑��̃^�C�����ς���
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
			//�܂�ڂɖʂ��Ă���ʂ������Ȃ���
			for (int z = 0; z < DEPTH; z++)
			{
				DirectX::XMFLOAT3 angle = { 0,0,-DirectX::XMConvertToRadians(90) };
				DirectX::XMFLOAT3 position = tiles[z][glid_position.x / 2.5f]->GetPosition();
				DirectX::XMFLOAT3 scale = tiles[z][glid_position.x / 2.5f]->GetScale();
				tiles[z][glid_position.x / 2.5f]->Set_local(scale, angle, position);
			}


			//�܂�ڂɖʂ��Ă��郏�[���h�s������Ƃɂ��̑��̃^�C�����ς���
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

//tiles�ɕۑ�����Ă���^�C����Render
void MapManager::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	for (auto& row : tiles)
	{
		// �e�^�C�������
		for (Tile* tile : row)
		{
			tile->Render(rc, renderer);
		}
	}
	renderer->Render(rc, transform, model, ShaderId::Lambert);
}

//tile�Ƀ}�b�v�`�b�v�̃f�[�^�ɍ��킹�ăZ�b�g
void MapManager::AddTile()
{
	for (int z = 0; z < DEPTH; z++)
	{
		// z �� tiles �̃T�C�Y���傫���ꍇ�A�s��ǉ�
		if (tiles.size() <= z)
		{
			tiles.push_back(std::vector<Tile*>()); // �V�����s��ǉ�
		}

		for (int x = 0; x < WIDTH; x++)
		{
			tiles[z].emplace_back(new Normal_Tile(2.5f * x, 2.5f * z));
		}
	}
}

void MapManager::Clear()
{
	// �e�s�istd::vector<Tile*>�j�ɑ΂���
	for (auto& row : tiles)
	{
		// �e�^�C�������
		for (Tile* tile : row)
		{
			delete tile;
		}
		row.clear();  // �e�s�̃^�C���̃|�C���^�����������A�s�̃x�N�^�[���N���A
	}
	tiles.clear();  // tiles��2�����x�N�^�[�����S�ɃN���A
}
