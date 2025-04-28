#pragma once

#include <vector>
#include <wrl.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include "RenderContext.h"

class ShapeRenderer
{
public:
	ShapeRenderer(ID3D11Device* device);
	~ShapeRenderer() {}

	// ���`��
	void RenderBox(
		const RenderContext& rc,
		const DirectX::XMFLOAT3& position,
		const DirectX::XMFLOAT3& angle,
		const DirectX::XMFLOAT3& size,
		const DirectX::XMFLOAT4& color) const;

	// ���`��
	void RenderSphere(
		const RenderContext& rc,
		const DirectX::XMFLOAT3& position,
		float radius,
		const DirectX::XMFLOAT4& color) const;

	// �~���`��
	void RenderCylinder(
		const RenderContext& rc,
		const DirectX::XMFLOAT3& position,
		float radius,
		float height,
		const DirectX::XMFLOAT4& color) const;

	// �J�v�Z���`��
	void RenderCapsule(
		const RenderContext& rc,
		const DirectX::XMFLOAT4X4& transform,
		float radius,
		float height,
		const DirectX::XMFLOAT4& color) const;

private:
	struct Mesh
	{
		Microsoft::WRL::ComPtr<ID3D11Buffer>	vertexBuffer;
		UINT									vertexCount;
	};

	struct CbMesh
	{
		DirectX::XMFLOAT4X4		worldViewProjection;
		DirectX::XMFLOAT4		color;
	};
	// �`����s
	void Render(const RenderContext& rc, const Mesh& mesh, const DirectX::XMFLOAT4X4& transform, const DirectX::XMFLOAT4& color) const;

	// ���b�V������
	void CreateMesh(ID3D11Device* device, const std::vector<DirectX::XMFLOAT3>& vertices, Mesh& mesh);

	// �����b�V���쐬
	void CreateBoxMesh(ID3D11Device* device, float width, float height, float depth);

	// �����b�V���쐬
	void CreateSphereMesh(ID3D11Device* device, float radius, int subdivisions);

	// �������b�V���쐬
	void CreateHalfSphereMesh(ID3D11Device* device, float radius, int subdivisions);

	// �~��
	void CreateCylinderMesh(ID3D11Device* device, float radius1, float radius2, float start, float height, int subdivisions);

private:
	Mesh										boxMesh;
	Mesh										sphereMesh;
	Mesh										halfSphereMesh;
	Mesh										cylinderMesh;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>	vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>	pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>	inputLayout;
	Microsoft::WRL::ComPtr<ID3D11Buffer>		constantBuffer;
};
