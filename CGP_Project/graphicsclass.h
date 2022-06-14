////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "inputclass.h"

#include "cameraclass.h"
#include "modelclass.h"
#include "lightshaderclass.h"
#include "lightclass.h"

#include "textureshaderclass.h"
#include "bitmapclass.h"
#include "textclass.h"

#include "GameObject.h"

#include "EffectClass.h"

class GameObjectMgr;
class CollisionMgr;
class PlayerClass;
class AIClass;
class SceneUIClass;
class UIClass;
class SoundClass;

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
public:
	enum STAGE { STAGE_TITLE, STAGE_MAIN, STAGE_RESULT, STAGE_END };

public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

public:
	void SetInputClass(InputClass* Input) { m_Input = Input; }
	void SetStage(STAGE eStage) { m_eStage = eStage; }

	STAGE GetStage() { return m_eStage; }

public:
	bool Initialize(int, int, HWND);
	void Shutdown();

	bool Frame(int, int);
	bool Frame(int, int, float);

	bool Render();

private:
	void TextFrame(int, int);
	void CollisionFrame(float);
	void KeyUpdate(float frameTime);
	
	void ResetItems();
	void CreateEffect(EffectClass::EFFECTID);
	void SetEffectDead();

private:
	D3DClass* m_D3D;
	InputClass* m_Input;

	CameraClass* m_Camera;
	LightShaderClass* m_LightShader;
	LightClass* m_Light;

	TextureShaderClass* m_TextureShader;
	BitmapClass* m_Bitmap;
	TextClass* m_Text;

	GameObjectMgr* m_UIObjectMgr;
	GameObjectMgr* m_GameObjectMgr;
	CollisionMgr* m_CollisionMgr;

	ModelClass* m_Map;
	ModelClass* m_Boat;
	ModelClass* m_Car;
	ModelClass* m_Unicorn;

	PlayerClass* m_Player;
	AIClass* m_AI;
	SceneUIClass* m_SceneUIClass;
	UIClass* m_TimerUIClass;

	SoundClass* m_BGMSound;
	SoundClass* m_SuccessSound;
	SoundClass* m_FailSound;
	SoundClass* m_ItemSound;
	SoundClass* m_CrashSound;

private:
	STAGE m_eStage;

	bool m_bInit = false;
	bool m_bReInit[3] = { false, false, false };

	bool m_bIsCamMove;

	int m_PolyCnt;
	int m_ObjCnt;
	int m_screenWidth;
	int m_screenHeight;

	float m_TimeLimit;
	int m_iNumCnt;

	bool m_bColliOn;
	float m_ColliCheck;

	bool m_Fail;
	float m_ChangeStageTime;
};

#endif