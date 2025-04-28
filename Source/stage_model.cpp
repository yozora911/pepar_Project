#include "stage_model.h"

//�R���X�g���N�^
Stage_model::Stage_model()
{
	model = new Model("Data/Model/floor/floor.mdl");
	scale.x = scale.y = scale.z = 3.0f;
}

//�f�X�g���N�^
Stage_model::~Stage_model()
{
	delete model;
}

//�X�V����
void Stage_model::Update(float elapsedTime)
{
	//�I�u�W�F�N�g�s����X�V
	UpdateTransform();

	//���f���s��X�V
	model->UpdateTransform();
}

//�`�揈��
void Stage_model::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	renderer->Render(rc, transform, model, ShaderId::Lambert);
}
