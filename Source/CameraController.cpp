#include "CameraController.h"
#include"System/Input.h"
#include"Camera.h"


//�X�V����
void CameraController::Update(float elapsedTime)
{
	//�J�����̉�]���x
	float speed = rollSpeed * elapsedTime;

	//�E�N���b�N�����Ă���Ƃ������J��������]
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


	

	//�J������]�l����]�s��ɕϊ�
	DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

	//��]�s�񂩂�O�����x�N�g�������o��
	DirectX::XMVECTOR Front = Transform.r[2];
	DirectX::XMFLOAT3 front;
	DirectX::XMStoreFloat3(&front, Front);

	//�����_������x�N�g�������Ɉ�苗�����ꂽ�J�������_�����߂�
	DirectX::XMFLOAT3 eye;
	eye.x = target.x + (-front.x * range);
	eye.y = target.y + (-front.y * range);
	eye.z = target.z + (-front.z * range);

	//�J�����̎��_�ƒ����_��ݒ�
	Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
}
