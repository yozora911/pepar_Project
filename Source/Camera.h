#pragma once

#include<DirectXMath.h>

//�J����
class Camera
{
public:
	static Camera& Instance()
	{
		static Camera instance;
		return instance;
	}

	//�w�����������
	void SetLookAt(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& focus,
		const DirectX::XMFLOAT3& up);

	//�p�[�X�y�N�e�B�u�ݒ�
	void SetPerspectiveFov(float fovY, float aspect, float nearZ, float farZ);

	//�r���[�s��擾
	const DirectX::XMFLOAT4X4& GetView()const { return view; }

	//�v���W�F�N�V�����s��擾
	const DirectX::XMFLOAT4X4& GetProjection()const { return projection; }

	//���_���擾
	const DirectX::XMFLOAT3& GetEye() const { return EYE; }
private:
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 projection;

	Camera(){}
	~Camera(){}
	DirectX::XMFLOAT3 EYE;

};