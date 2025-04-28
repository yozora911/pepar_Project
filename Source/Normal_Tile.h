#pragma once
#include"Tile.h"
#include "System/ModelRenderer.h"
#include<DirectXMath.h>
class Normal_Tile:public Tile
{
public:
	Normal_Tile(float x,float z);
	~Normal_Tile();

	void Update(float elapsedTime);

	void Render(const RenderContext& rc, ModelRenderer* renderer);

	DirectX::XMFLOAT4X4 GetTransform() { return transform; }

	void Global_Transform(DirectX::XMFLOAT4X4 global_transform);

	void Set_local(DirectX::XMFLOAT3 scale,DirectX::XMFLOAT3 angle,DirectX::XMFLOAT3 position);

	DirectX::XMFLOAT3 GetScale() { return scale; }

	DirectX::XMFLOAT3 GetPosition() { return position; }
private:
	Model* model;
};