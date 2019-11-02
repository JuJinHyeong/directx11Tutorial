#pragma once
#include <DirectXMath.h>
using namespace DirectX;

class Camera {
private:
	void UpdateViewMatrix();
	XMVECTOR mPosVector;
	XMVECTOR mRotVector;
	XMFLOAT3 mPos;
	XMFLOAT3 mRot;
	XMMATRIX mViewMatrix;
	XMMATRIX mProjectionMatrix;

	const XMVECTOR DEFAULT_FORWARD_VECTOR = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const XMVECTOR DEFAULT_UP_VECTOR = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_BACKWARD_VECTOR = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	const XMVECTOR DEFAULT_LEFT_VECTOR = XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_RIGHT_VECTOR = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	XMVECTOR mVecForward;
	XMVECTOR mVecLeft;
	XMVECTOR mVecRight;
	XMVECTOR mVecBackward;

public:
	Camera();
	void SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ);

	const XMMATRIX& GetViewMatrix() const;
	const XMMATRIX& GetProjectionMatrix() const;

	const XMVECTOR& GetPositionVector() const;
	const XMFLOAT3& GetPositionFloat3() const;
	const XMVECTOR& GetRotationVector() const;
	const XMFLOAT3& GetRotationFloat3() const;

	const XMVECTOR& GetForwardVector();
	const XMVECTOR& GetRightVector();
	const XMVECTOR& GetBackwardVector();
	const XMVECTOR& GetLeftVector();

	void SetPosition(const XMVECTOR& pos);
	void SetPosition(float x, float y, float z);
	void SetPosition(const XMFLOAT3& pos);
	void AdjustPosition(const XMVECTOR& pos);
	void AdjustPosition(float x, float y, float z);
	void AdjustPosition(const XMFLOAT3& pos);
	void SetRotation(const XMVECTOR& rot);
	void SetRotation(float x, float y, float z);
	void SetRotation(const XMFLOAT3& rot);
	void AdjustRotation(const XMVECTOR& rot);
	void AdjustRotation(float x, float y, float z);
	void AdjustRotation(const XMFLOAT3& rot);
	void SetLookAtPos(XMFLOAT3 lookAtPos);

};