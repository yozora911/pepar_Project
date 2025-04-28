#pragma once

#include<DirectXMath.h>
#include <windows.h>


//カメラコントローラー
class CameraController
{
public:
	//更新処理
	void Update(float elapsedTime);

	//ターゲット位置設定
	void SetTarget(const DirectX::XMFLOAT3& target) { this->target = target; }

private:
	DirectX::XMFLOAT3 target = { 0,0,0 };//注視点
	DirectX::XMFLOAT3 angle = { 0,0,0 };//回転角度
	float rollSpeed = DirectX::XMConvertToRadians(90);//回転速度
	float range = 30.0f;//距離

	POINT mouse_position = { 0,0 };
	POINT start_mouse_position;
	int mouse_state = 0;
};