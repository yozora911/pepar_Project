#include <algorithm>
#include "Misc.h"
#include "GpuResourceUtils.h"
#include "ModelRenderer.h"
#include "BasicShader.h"
#include "LambertShader.h"

// �R���X�g���N�^
ModelRenderer::ModelRenderer(ID3D11Device* device)
{
	// �V�[���p�萔�o�b�t�@
	GpuResourceUtils::CreateConstantBuffer(
		device,
		sizeof(CbScene),
		sceneConstantBuffer.GetAddressOf());

	// �X�P���g���p�萔�o�b�t�@
	GpuResourceUtils::CreateConstantBuffer(
		device,
		sizeof(CbSkeleton),
		skeletonConstantBuffer.GetAddressOf());

	// �V�F�[�_�[����
	shaders[static_cast<int>(ShaderId::Basic)] = std::make_unique<BasicShader>(device);
	shaders[static_cast<int>(ShaderId::Lambert)] = std::make_unique<LambertShader>(device);
}

// �`����s
void ModelRenderer::Render(const RenderContext& rc, const DirectX::XMFLOAT4X4& worldTransform, const Model* model, ShaderId shaderId)
{
	ID3D11DeviceContext* dc = rc.deviceContext;

	// �V�[���p�萔�o�b�t�@�X�V
	{
		CbScene cbScene{};
		DirectX::XMMATRIX V = DirectX::XMLoadFloat4x4(&rc.view);
		DirectX::XMMATRIX P = DirectX::XMLoadFloat4x4(&rc.projection);
		DirectX::XMStoreFloat4x4(&cbScene.viewProjection, V * P);
		cbScene.lightDirection.x = rc.lightDirection.x;
		cbScene.lightDirection.y = rc.lightDirection.y;
		cbScene.lightDirection.z = rc.lightDirection.z;
		dc->UpdateSubresource(sceneConstantBuffer.Get(), 0, 0, &cbScene, 0, 0);
	}

	// �萔�o�b�t�@�ݒ�
	ID3D11Buffer* vsConstantBuffers[] =
	{
		sceneConstantBuffer.Get(),
		skeletonConstantBuffer.Get(),
	};
	dc->VSSetConstantBuffers(0, _countof(vsConstantBuffers), vsConstantBuffers);

	ID3D11Buffer* psConstantBuffers[] =
	{
		sceneConstantBuffer.Get(),
	};
	dc->PSSetConstantBuffers(0, _countof(psConstantBuffers), psConstantBuffers);

	// �T���v���X�e�[�g�ݒ�
	ID3D11SamplerState* samplerStates[] =
	{
		rc.renderState->GetSamplerState(SamplerState::LinearWrap)
	};
	dc->PSSetSamplers(0, _countof(samplerStates), samplerStates);

	// �����_�[�X�e�[�g�ݒ�
	dc->OMSetDepthStencilState(rc.renderState->GetDepthStencilState(DepthState::TestAndWrite), 0);
	dc->RSSetState(rc.renderState->GetRasterizerState(RasterizerState::SolidCullBack));

	// �u�����h�X�e�[�g�ݒ�
	dc->OMSetBlendState(rc.renderState->GetBlendState(BlendState::Transparency), nullptr, 0xFFFFFFFF);

	// �`�揈��
	Shader* shader = shaders[static_cast<int>(shaderId)].get();
	shader->Begin(rc);

	DirectX::XMMATRIX WorldTransform = DirectX::XMLoadFloat4x4(&worldTransform);

	const ModelResource* resource = model->GetResource();
	for (const ModelResource::Mesh& mesh : resource->GetMeshes())
	{
		// ���_�o�b�t�@�ݒ�
		UINT stride = sizeof(ModelResource::Vertex);
		UINT offset = 0;
		dc->IASetVertexBuffers(0, 1, mesh.vertexBuffer.GetAddressOf(), &stride, &offset);
		dc->IASetIndexBuffer(mesh.indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// �X�P���g���p�萔�o�b�t�@�X�V
		CbSkeleton cbSkeleton{};
		if (mesh.nodeIndices.size() > 0)
		{
			for (size_t i = 0; i < mesh.nodeIndices.size(); ++i)
			{
				int nodeIndex = mesh.nodeIndices.at(i);
				const Model::Node& node = model->GetNodes().at(nodeIndex);
				DirectX::XMMATRIX GlobalTransform = DirectX::XMLoadFloat4x4(&node.globalTransform);
				DirectX::XMMATRIX OffsetTransform = DirectX::XMLoadFloat4x4(&mesh.offsetTransforms.at(i));
				DirectX::XMMATRIX BoneTransform = OffsetTransform * GlobalTransform * WorldTransform;
				DirectX::XMStoreFloat4x4(&cbSkeleton.boneTransforms[i], BoneTransform);
			}
		}
		else
		{
			const Model::Node& node = model->GetNodes().at(mesh.nodeIndex);
			DirectX::XMMATRIX GlobalTransform = DirectX::XMLoadFloat4x4(&node.globalTransform);
			DirectX::XMMATRIX BoneTransform = GlobalTransform * WorldTransform;
			DirectX::XMStoreFloat4x4(&cbSkeleton.boneTransforms[0], BoneTransform);
		}
		dc->UpdateSubresource(skeletonConstantBuffer.Get(), 0, 0, &cbSkeleton, 0, 0);

		// �`��
		for (const ModelResource::Subset& subset : mesh.subsets)
		{
			shader->Update(rc, *subset.material);

			dc->DrawIndexed(subset.indexCount, subset.startIndex, 0);
		}
	}

	shader->End(rc);

	// �萔�o�b�t�@�ݒ����
	for (ID3D11Buffer*& vsConstantBuffer : vsConstantBuffers) { vsConstantBuffer = nullptr; }
	for (ID3D11Buffer*& psConstantBuffer : psConstantBuffers) { psConstantBuffer = nullptr; }
	dc->VSSetConstantBuffers(0, _countof(vsConstantBuffers), vsConstantBuffers);
	dc->PSSetConstantBuffers(0, _countof(psConstantBuffers), psConstantBuffers);

	// �T���v���X�e�[�g�ݒ����
	for (ID3D11SamplerState*& samplerState : samplerStates) { samplerState = nullptr; }
	dc->PSSetSamplers(0, _countof(samplerStates), samplerStates);
}
