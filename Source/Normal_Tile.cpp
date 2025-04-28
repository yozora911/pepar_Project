#include "Normal_Tile.h"

Normal_Tile::Normal_Tile(float x,float z)
{
	position.x = x;
	position.z = z;
	model = new Model("Data/Model/floor/floor.mdl");
	UpdateTransform();
}

Normal_Tile::~Normal_Tile()
{
	delete model;
}

void Normal_Tile::Update(float elapsedTime)
{
	/*UpdateTransform();*/

	//model->UpdateTransform();
}

void Normal_Tile::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	renderer->Render(rc, transform, model, ShaderId::Lambert);
}

void Normal_Tile::Global_Transform(DirectX::XMFLOAT4X4 global_transform)
{
	DirectX::XMMATRIX Transform = DirectX::XMLoadFloat4x4(&transform);
	DirectX::XMMATRIX global_Transform = DirectX::XMLoadFloat4x4(&global_transform);

	// ‘f’¼‚Églobal_transform‚ð‚©‚¯‚é‚¾‚¯
	Transform = Transform * global_Transform;

	DirectX::XMStoreFloat4x4(&transform, Transform);
}

void Normal_Tile::Set_local(DirectX::XMFLOAT3 scale, DirectX::XMFLOAT3 angle, DirectX::XMFLOAT3 position)
{
	this->scale = scale;
	this->angle = angle;
	this->position = position;
	UpdateTransform();
}
