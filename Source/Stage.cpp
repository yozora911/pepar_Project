#include "Stage.h"

//�R���X�g���N�^
Stage::Stage()
{
	//�X�e�[�W���f����ǂݍ���
	model = new Model("Data/Model/Stage/ExampleStage.mdl");

}

//�f�X�g���N�^
Stage::~Stage()
{
	//�X�e�[�W���f����j��
	delete model;
}

//�X�V����
void Stage::Update(float elapsedTime)
{
}

//�`�揈��
void Stage::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	DirectX::XMFLOAT4X4 transform;
	DirectX::XMStoreFloat4x4(&transform, DirectX::XMMatrixIdentity());

	//�����_���Ƀ��f����`�悵�Ă��炤
	renderer->Render(rc, transform, model, ShaderId::Lambert);
}
