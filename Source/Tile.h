#pragma once
#include"Character.h"
class Tile:public Character
{
public:
	Tile(){}
	Tile(float x,float z) {}
	virtual ~Tile() {}

	virtual void Update(float elapsedTime) {};

	virtual void Render(const RenderContext& rc, ModelRenderer* renderer) {};

	virtual DirectX::XMFLOAT4X4 GetTransform() { return transform; }

	virtual void Global_Transform(DirectX::XMFLOAT4X4 global_transform){}

	virtual void Set_local(DirectX::XMFLOAT3 scale, DirectX::XMFLOAT3 angle, DirectX::XMFLOAT3 position){}
};