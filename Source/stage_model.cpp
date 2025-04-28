#include "stage_model.h"

//コンストラクタ
Stage_model::Stage_model()
{
	model = new Model("Data/Model/floor/floor.mdl");
	scale.x = scale.y = scale.z = 3.0f;
}

//デストラクタ
Stage_model::~Stage_model()
{
	delete model;
}

//更新処理
void Stage_model::Update(float elapsedTime)
{
	//オブジェクト行列を更新
	UpdateTransform();

	//モデル行列更新
	model->UpdateTransform();
}

//描画処理
void Stage_model::Render(const RenderContext& rc, ModelRenderer* renderer)
{
	renderer->Render(rc, transform, model, ShaderId::Lambert);
}
