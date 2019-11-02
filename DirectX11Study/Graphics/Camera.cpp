#include "Camera.h"

Camera::Camera()
	:
	mPos(XMFLOAT3(0.0f, 0.0f, 0.0f)),
	mRot(XMFLOAT3(0.0f, 0.0f, 0.0f))
{
	mPosVector = XMLoadFloat3(&mPos);
	mRotVector = XMLoadFloat3(&mRot);
	UpdateViewMatrix();
}

void Camera::SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ) {
	float fovRadians = (fovDegrees / 360.0f) * XM_2PI;
	mProjectionMatrix = XMMatrixPerspectiveFovLH(fovRadians, aspectRatio, nearZ, farZ);
}

const XMMATRIX& Camera::GetViewMatrix() const {
	return mViewMatrix;
}

const XMMATRIX& Camera::GetProjectionMatrix() const {
	return mProjectionMatrix;
}

const XMVECTOR& Camera::GetPositionVector() const {
	return mPosVector;
}

const XMFLOAT3& Camera::GetPositionFloat3() const {
	return mPos;
}

const XMVECTOR& Camera::GetRotationVector() const {
	return mRotVector;
}

const XMFLOAT3& Camera::GetRotationFloat3() const {
	return mRot;
}

const XMVECTOR& Camera::GetForwardVector()
{
	return mVecForward;
}

const XMVECTOR& Camera::GetRightVector()
{
	return mVecRight;
}

const XMVECTOR& Camera::GetBackwardVector()
{
	return mVecBackward;
}

const XMVECTOR& Camera::GetLeftVector()
{
	return mVecLeft;
}

void Camera::SetPosition(const XMVECTOR& pos) {
	XMStoreFloat3(&mPos, pos);
	mPosVector = pos;
	UpdateViewMatrix();
}

void Camera::SetPosition(float x, float y, float z) {
	mPos = XMFLOAT3(x, y, z);
	mPosVector = XMLoadFloat3(&mPos);
	UpdateViewMatrix();
}

void Camera::SetPosition(const XMFLOAT3& pos)
{
	mPos = pos;
	mPosVector = XMLoadFloat3(&mPos);
	UpdateViewMatrix();
}

void Camera::AdjustPosition(const XMVECTOR& pos) {
	mPosVector += pos;
	XMStoreFloat3(&mPos, mPosVector);
	UpdateViewMatrix();
}

void Camera::AdjustPosition(float x, float y, float z) {
	mPos.x += x;
	mPos.y += y;
	mPos.z += z;
	mPosVector = XMLoadFloat3(&mPos);
	UpdateViewMatrix();
}

void Camera::AdjustPosition(const XMFLOAT3& pos)
{
	mPos = pos;
	mPosVector = XMLoadFloat3(&mPos);
	UpdateViewMatrix();
}

void Camera::SetRotation(const XMVECTOR& rot) {
	mRotVector = rot;
	XMStoreFloat3(&mRot, rot);
	UpdateViewMatrix();
}

void Camera::SetRotation(float x, float y, float z) {
	mRot = XMFLOAT3(x, y, z);
	mRotVector = XMLoadFloat3(&mRot);
	UpdateViewMatrix();
}

void Camera::SetRotation(const XMFLOAT3& rot)
{
	mRot = rot;
	mRotVector = XMLoadFloat3(&mRot);
	UpdateViewMatrix();
}

void Camera::AdjustRotation(const XMVECTOR& rot) {
	mRotVector += rot;
	XMStoreFloat3(&mRot, mRotVector);
	UpdateViewMatrix();
}

void Camera::AdjustRotation(float x, float y, float z) {
	mRot.x += x;
	mRot.y += y;
	mRot.z += z;
	mRotVector = XMLoadFloat3(&mRot);
	UpdateViewMatrix();
}

void Camera::AdjustRotation(const XMFLOAT3& rot)
{
	mRot = rot;
	mRotVector = XMLoadFloat3(&mRot);
	UpdateViewMatrix();
}

void Camera::SetLookAtPos(XMFLOAT3 lookAtPos){
	if (lookAtPos.x == mPos.x && lookAtPos.y == mPos.y && lookAtPos.z == mPos.z) {
		return;
	}

	lookAtPos.x = mPos.x - lookAtPos.x;
	lookAtPos.y = mPos.y - lookAtPos.y;
	lookAtPos.z = mPos.z - lookAtPos.z;

	double x = static_cast<double>(lookAtPos.x);
	double y = static_cast<double>(lookAtPos.y);
	double z = static_cast<double>(lookAtPos.z);

	float pitch = 0.0f;
	if (lookAtPos.y != 0.0f) {
		const float distance = sqrt(x * x + z * z);
		pitch = atan(lookAtPos.y / distance);
	}

	float yaw = 0.0f;
	if (lookAtPos.x != 0.0f) {
		yaw = atan(x / z);
	}
	if (lookAtPos.z > 0) {
		yaw += XM_PI;
	}

	SetRotation(pitch, yaw, 0.0f);
}

void Camera::UpdateViewMatrix() {
	XMMATRIX camRotationMatrix = XMMatrixRotationRollPitchYaw(mRot.x, mRot.y, mRot.z);
	XMVECTOR camTarget = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, camRotationMatrix);

	camTarget += mPosVector;
	XMVECTOR upDir = XMVector3TransformCoord(DEFAULT_UP_VECTOR, camRotationMatrix);
	mViewMatrix = XMMatrixLookAtLH(mPosVector, camTarget, upDir);

	XMMATRIX vecRotationMatrix = XMMatrixRotationRollPitchYaw(0.0f, mRot.y, 0.0f);
	mVecForward = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
	mVecBackward = XMVector3TransformCoord(DEFAULT_BACKWARD_VECTOR, vecRotationMatrix);
	mVecLeft = XMVector3TransformCoord(DEFAULT_LEFT_VECTOR, vecRotationMatrix);
	mVecRight = XMVector3TransformCoord(DEFAULT_RIGHT_VECTOR, vecRotationMatrix);

}