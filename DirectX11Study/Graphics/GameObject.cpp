#include "GameObject.h"

#include "Model.h"

bool GameObject::Initailize(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture, ConstantBuffer<CB_VS_vertexshader>& cbVsVertexShader)
{
	if (!mModel.Initailize(filePath, device, deviceContext, texture, cbVsVertexShader)) {
		return false;
	}

	SetPosition(0.0f, 0.0f, 0.0f);
	SetRotation(0.0f, 0.0f, 0.0f);
	UpdateWorldMatrix();
	return true;
}

void GameObject::Draw(const XMMATRIX& viewProjectionMatrix)
{
	mModel.Draw(mWorldMatrix, viewProjectionMatrix);
}

const XMVECTOR& GameObject::GetPositionVector() const {
	return mPosVector;
}

const XMFLOAT3& GameObject::GetPositionFloat3() const {
	return mPos;
}

const XMVECTOR& GameObject::GetRotationVector() const {
	return mRotVector;
}

const XMFLOAT3& GameObject::GetRotationFloat3() const {
	return mRot;
}

const XMVECTOR& GameObject::GetForwardVector()
{
	return mVecForward;
}

const XMVECTOR& GameObject::GetRightVector()
{
	return mVecRight;
}

const XMVECTOR& GameObject::GetBackwardVector()
{
	return mVecBackward;
}

const XMVECTOR& GameObject::GetLeftVector()
{
	return mVecLeft;
}

void GameObject::SetPosition(const XMVECTOR& pos) {
	XMStoreFloat3(&mPos, pos);
	mPosVector = pos;
	UpdateWorldMatrix();
}

void GameObject::SetPosition(float x, float y, float z) {
	mPos = XMFLOAT3(x, y, z);
	mPosVector = XMLoadFloat3(&mPos);
	UpdateWorldMatrix();
}

void GameObject::SetPosition(const XMFLOAT3& pos)
{
	mPos = pos;
	mPosVector = XMLoadFloat3(&mPos);
	UpdateWorldMatrix();
}

void GameObject::AdjustPosition(const XMVECTOR& pos) {
	mPosVector += pos;
	XMStoreFloat3(&mPos, mPosVector);
	UpdateWorldMatrix();
}

void GameObject::AdjustPosition(float x, float y, float z) {
	mPos.x += x;
	mPos.y += y;
	mPos.z += z;
	mPosVector = XMLoadFloat3(&mPos);
	UpdateWorldMatrix();
}

void GameObject::AdjustPosition(const XMFLOAT3& pos)
{
	mPos = pos;
	mPosVector = XMLoadFloat3(&mPos);
	UpdateWorldMatrix();
}

void GameObject::SetRotation(const XMVECTOR& rot) {
	mRotVector = rot;
	XMStoreFloat3(&mRot, rot);
	UpdateWorldMatrix();
}

void GameObject::SetRotation(float x, float y, float z) {
	mRot = XMFLOAT3(x, y, z);
	mRotVector = XMLoadFloat3(&mRot);
	UpdateWorldMatrix();
}

void GameObject::SetRotation(const XMFLOAT3& rot)
{
	mRot = rot;
	mRotVector = XMLoadFloat3(&mRot);
	UpdateWorldMatrix();
}

void GameObject::AdjustRotation(const XMVECTOR& rot) {
	mRotVector += rot;
	XMStoreFloat3(&mRot, mRotVector);
	UpdateWorldMatrix();
}

void GameObject::AdjustRotation(float x, float y, float z) {
	mRot.x += x;
	mRot.y += y;
	mRot.z += z;
	mRotVector = XMLoadFloat3(&mRot);
	UpdateWorldMatrix();
}

void GameObject::AdjustRotation(const XMFLOAT3& rot)
{
	mRot = rot;
	mRotVector = XMLoadFloat3(&mRot);
	UpdateWorldMatrix();
}

void GameObject::SetLookAtPos(XMFLOAT3 lookAtPos) {
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

void GameObject::UpdateWorldMatrix() {
	mWorldMatrix = XMMatrixRotationRollPitchYaw(mRot.x, mRot.y, mRot.z) * XMMatrixTranslation(mPos.x, mPos.y, mPos.z);
	XMMATRIX vecRotationMatrix = XMMatrixRotationRollPitchYaw(0.0f, mRot.y, 0.0f);
	mVecForward = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
	mVecBackward = XMVector3TransformCoord(DEFAULT_BACKWARD_VECTOR, vecRotationMatrix);
	mVecLeft = XMVector3TransformCoord(DEFAULT_LEFT_VECTOR, vecRotationMatrix);
	mVecRight = XMVector3TransformCoord(DEFAULT_RIGHT_VECTOR, vecRotationMatrix);
}