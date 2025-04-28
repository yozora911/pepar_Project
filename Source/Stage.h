#pragma once

#include "System/ModelRenderer.h"

//�X�e�[�W
class Stage
{
public:
	Stage();
	~Stage();

	//�X�V����
	void Update(float elapsedTime);

	//�`�揈��
	void Render(const RenderContext& rc, ModelRenderer* renderer);

private:
	Model* model = nullptr;
};
