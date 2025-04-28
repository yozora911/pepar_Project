#pragma once

#include "System/ModelRenderer.h"
#include "Character.h"

//ステージモデル
class Stage_model : public Character
{
public:
	Stage_model();
	~Stage_model() override;

	//更新処理
	void Update(float elapsedTime);

	//描画処理
	void Render(const RenderContext& rc, ModelRenderer* renderer);

	Model* GetModel() { return model; }

private:
	Model* model = nullptr;
};
