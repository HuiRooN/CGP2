#pragma once

#include "GameObject.h"

class InputClass;
class CameraClass;
class TextureShaderClass;

class SceneUIClass :
	public GameObject
{
public:
	enum TEXID {TEX_TITLE, TEX_GOAL, TEX_CONTROL, TEX_CLEAR, TEX_FAIL, TEX_END};

public:
	SceneUIClass();
	SceneUIClass(const SceneUIClass&);
	~SceneUIClass();

public:
	void SetSceneID(int scene) { m_iSceneID = scene; }
	void SetTexID(TEXID tex) { m_eTexID = tex; }
	void SetInput(InputClass* input) { m_Input = input; }
	void SetCamera(CameraClass* camera) { m_Camera = camera; }

	TEXID GetSceneID() { return m_eTexID; }
	bool GetIsFirst() { return m_bIsFirst; }

public:
	void InitializeTex(ID3D11Device*);
	virtual bool Frame(float);
	void Render(ID3D11DeviceContext*, TextureShaderClass*, D3DXMATRIX, D3DXMATRIX);

private:
	void Initialize();
	void ShutDown();

private:
	int m_iSceneID;
	TEXID m_eTexID;

	bool m_bIsFirst;

	InputClass* m_Input;
	CameraClass* m_Camera;

	ModelClass* m_GoalTex;
	ModelClass* m_CntlTex;
	ModelClass* m_ClearTex;
	ModelClass* m_FailTex;
};

