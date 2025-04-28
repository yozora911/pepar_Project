#pragma once

#include<DirectXMath.h>
#include <windows.h>


//�J�����R���g���[���[
class CameraController
{
public:
	//�X�V����
	void Update(float elapsedTime);

	//�^�[�Q�b�g�ʒu�ݒ�
	void SetTarget(const DirectX::XMFLOAT3& target) { this->target = target; }

private:
	DirectX::XMFLOAT3 target = { 0,0,0 };//�����_
	DirectX::XMFLOAT3 angle = { 0,0,0 };//��]�p�x
	float rollSpeed = DirectX::XMConvertToRadians(90);//��]���x
	float range = 30.0f;//����

	POINT mouse_position = { 0,0 };
	POINT start_mouse_position;
	int mouse_state = 0;
};