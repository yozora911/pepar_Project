#pragma once

#include "System/ModelRenderer.h"
#include "Character.h"

//�X�e�[�W���f��
class Stage_model : public Character
{
public:
	Stage_model();
	~Stage_model() override;

	//�X�V����
	void Update(float elapsedTime);

	//�`�揈��
	void Render(const RenderContext& rc, ModelRenderer* renderer);

	Model* GetModel() { return model; }

private:
	Model* model = nullptr;
};
