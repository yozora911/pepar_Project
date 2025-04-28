#pragma once

#include <DirectXMath.h>
#include "System/ModelRenderer.h"

// �R���W����
class Collision
{
public:
	// ���C�L���X�g
	static bool RayCast(
		const DirectX::XMFLOAT3& start,
		const DirectX::XMFLOAT3& end,
		const DirectX::XMFLOAT4X4& worldTransform,
		const Model* model,
		DirectX::XMFLOAT3& hitPosition,
		DirectX::XMFLOAT3& hitNormal);

	// ���Ƃ̃��C�L���X�g
	static bool SquareRayCast(
		const DirectX::XMFLOAT3& start,
		const DirectX::XMFLOAT3& end,
		const DirectX::XMFLOAT3& vertexA,
		const DirectX::XMFLOAT3& vertexB,
		const DirectX::XMFLOAT3& vertexC,
		const DirectX::XMFLOAT3 & vertexD,
		DirectX::XMFLOAT3& hitPosition,
		DirectX::XMFLOAT3& hitNormal);
};
