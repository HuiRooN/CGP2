////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "cameraclass.h"


CameraClass::CameraClass()
{
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;

	m_camPosition = D3DXVECTOR3(0.0f, 0.0f, -8.0f);
	m_camTarget = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_camUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	D3DXMatrixLookAtLH(&m_camView, &m_camPosition, &m_camTarget, &m_camUp);
	m_DefaultForward = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_DefaultRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_camForward = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_camRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

	m_moveLeftRight = 0.0f;
	m_moveBackForward = 0.0f;

	m_camYaw = 0.0f;
	m_camPitch = 0.0f;
}


CameraClass::CameraClass(const CameraClass& other)
{
}


CameraClass::~CameraClass()
{
}


void CameraClass::SetPosition(float x, float y, float z)
{
	m_camPosition.x = x;
	m_camPosition.y = y;
	m_camPosition.z = z;
	return;
}

void CameraClass::SetPosition(D3DXVECTOR3 camPos)
{
	m_camPosition = camPos;
}

void CameraClass::SetRotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
	return;
}

D3DXVECTOR3 CameraClass::GetPosition()
{
	return D3DXVECTOR3(m_camPosition.x, m_camPosition.y, m_camPosition.z);
}


D3DXVECTOR3 CameraClass::GetRotation()
{
	return D3DXVECTOR3(m_rotationX, m_rotationY, m_rotationZ);
}

void CameraClass::GetViewMatrix(D3DXMATRIX& viewMatrix)
{
	viewMatrix = m_camView;
	return;
}

D3DXMATRIX & CameraClass::GetViewMatrix()
{
	return m_camView;
}

void CameraClass::Update()
{
	D3DXMATRIX camRotationMatrix;
	
	D3DXMatrixRotationYawPitchRoll(&camRotationMatrix, m_camYaw, m_camPitch, 0);

	D3DXVec3TransformCoord(&m_camTarget, &m_DefaultForward, &camRotationMatrix);
	D3DXVec3Normalize(&m_camTarget, &m_camTarget);

	D3DXMATRIX RotateYTempMatrix;
	D3DXMatrixRotationY(&RotateYTempMatrix, m_camYaw);

	D3DXVec3TransformCoord(&m_camRight, &m_DefaultRight, &RotateYTempMatrix);
	D3DXVec3TransformCoord(&m_camUp, &m_camUp, &RotateYTempMatrix);
	D3DXVec3TransformCoord(&m_camForward, &m_DefaultForward, &RotateYTempMatrix);

	m_camPosition += m_moveLeftRight * m_camRight;
	m_camPosition += m_moveBackForward * m_camForward;

	m_moveLeftRight = 0.0f;
	m_moveBackForward = 0.0f;
	
	m_camTarget = m_camPosition + m_camTarget;

	D3DXMatrixLookAtLH(&m_camView, &m_camPosition, &m_camTarget, &m_camUp);
}

void CameraClass::Render()
{
	D3DXVECTOR3 up, position, lookAt;
	float yaw, pitch, roll;
	D3DXMATRIX rotationMatrix;


	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Setup the position of the camera in the world.
	position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;

	// Setup where the camera is looking by default.
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = m_rotationX * 0.0174532925f;
	yaw   = m_rotationY * 0.0174532925f;
	roll  = m_rotationZ * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAt = position + lookAt;

	// Finally create the view matrix from the three updated vectors.
	D3DXMatrixLookAtLH(&m_viewMatrix, &position, &lookAt, &up);

	return;
}

void CameraClass::Reset()
{
	m_camPosition = D3DXVECTOR3(0.0f, 0.0f, -8.0f);
	m_camTarget = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_camUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_camForward = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_camRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

	m_camYaw = 0.0f;
	m_camPitch = 0.0f;
}
