////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3dx10math.h>
#include <xnamath.h>

////////////////////////////////////////////////////////////////////////////////
// Class name: CameraClass
////////////////////////////////////////////////////////////////////////////////
class CameraClass
{
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	void SetPosition(float, float, float);
	void SetPosition(D3DXVECTOR3);
	void SetRotation(float, float, float);

	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();
	
	void GetViewMatrix(D3DXMATRIX&);
	D3DXMATRIX& GetViewMatrix();

	void SetMoveLeftRight(float LR) { m_moveLeftRight += LR; }
	void SetMoveBackForward(float BF) { m_moveBackForward += BF; }
	void SetCamYaw(float yaw) { m_camYaw += yaw; }
	void SetCamPitch(float pitch) { m_camPitch += pitch; }

	void Update();
	void Render();
	void Reset();

private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	D3DXMATRIX m_viewMatrix;

	///////////////////////////////////////////////////////////////
	D3DXVECTOR3 m_camPosition;
	D3DXVECTOR3 m_camTarget;
	D3DXVECTOR3 m_camUp;
	D3DXMATRIX m_camView;

	D3DXVECTOR3 m_DefaultForward;
	D3DXVECTOR3 m_DefaultRight;
	D3DXVECTOR3 m_camForward;
	D3DXVECTOR3 m_camRight;

	float m_moveLeftRight;
	float m_moveBackForward;

	float m_camYaw;
	float m_camPitch;
};

#endif