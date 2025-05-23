#pragma once

#include<DirectXMath.h>

//カメラ
class Camera
{
public:
	static Camera& Instance()
	{
		static Camera instance;
		return instance;
	}

	//指定方向を向く
	void SetLookAt(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& focus,
		const DirectX::XMFLOAT3& up);

	//パースペクティブ設定
	void SetPerspectiveFov(float fovY, float aspect, float nearZ, float farZ);

	//ビュー行列取得
	const DirectX::XMFLOAT4X4& GetView()const { return view; }

	//プロジェクション行列取得
	const DirectX::XMFLOAT4X4& GetProjection()const { return projection; }

	//視点を取得
	const DirectX::XMFLOAT3& GetEye() const { return EYE; }
private:
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 projection;

	Camera(){}
	~Camera(){}
	DirectX::XMFLOAT3 EYE;

};