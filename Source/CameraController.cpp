#include "CameraController.h"
#include"System/Input.h"
#include"Camera.h"


//更新処理
void CameraController::Update(float elapsedTime)
{
	//カメラの回転速度
	float speed = rollSpeed * elapsedTime;

	//右クリックをしているときだけカメラを回転
	GetCursorPos(&start_mouse_position);
	if ((GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0)
	{
		if (mouse_position.x != 0 && mouse_position.y != 0)
		{
			angle.y += (start_mouse_position.x - mouse_position.x)*rollSpeed*0.01f;
			angle.x += (start_mouse_position.y - mouse_position.y)*rollSpeed*0.01f;
		}
		mouse_position = start_mouse_position;
	}
	else
	{
		mouse_position = { 0,0 };
	}


	

	//カメラ回転値を回転行列に変換
	DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

	//回転行列から前方向ベクトルを取り出す
	DirectX::XMVECTOR Front = Transform.r[2];
	DirectX::XMFLOAT3 front;
	DirectX::XMStoreFloat3(&front, Front);

	//注視点から後ろベクトル方向に一定距離離れたカメラ視点を求める
	DirectX::XMFLOAT3 eye;
	eye.x = target.x + (-front.x * range);
	eye.y = target.y + (-front.y * range);
	eye.z = target.z + (-front.z * range);

	//カメラの視点と注視点を設定
	Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
}
