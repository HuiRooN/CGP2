////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"

#include "GameObjectMgr.h"
#include "CollisionMgr.h"

#include "SkyboxClass.h"
#include "BoundaryClass.h"
#include "AILineClass.h"
#include "GoalClass.h"
#include "PlayerClass.h"
#include "AIClass.h"
#include "ItemClass.h"
#include "SceneUIClass.h"
#include "UIClass.h"

#include "soundclass.h"

#include <stdlib.h>
#include <time.h>

GraphicsClass::GraphicsClass()
{
	srand(unsigned int(time(NULL)));

	m_D3D = 0;
	m_Camera = 0;

	m_LightShader = 0;
	m_Light = 0;

	m_TextureShader = 0;
	m_Bitmap = 0;

	m_Text = 0;

	m_GameObjectMgr = 0;
	m_CollisionMgr = 0;

	m_Map = 0;
	m_Boat = 0;
	m_Car = 0;
	m_Unicorn = 0;
	
	m_Player = 0;
	m_AI = 0;
	m_SceneUIClass = 0;
	m_TimerUIClass = 0;
	
	m_BGMSound = 0;
	m_SuccessSound = 0;
	m_FailSound = 0;
	m_ItemSound = 0;
	m_CrashSound = 0;

	m_eStage = STAGE_TITLE;

	m_bIsCamMove = false;
	m_PolyCnt = 0;
	m_ObjCnt = 0;

	m_TimeLimit = 30.f;
	m_iNumCnt = 0;
	m_ColliCheck = 0.f;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	D3DXMATRIX baseViewMatrix;
	GameObject* gameObject = nullptr;

	if (!m_bInit)
	{
		// Create the Direct3D object.
		m_D3D = new D3DClass;
		if (!m_D3D)
		{
			return false;
		}

		// Initialize the Direct3D object.
		result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
			return false;
		}
		m_screenWidth = screenWidth;
		m_screenHeight = screenHeight;

		// Create the camera object.
		m_Camera = new CameraClass;
		if (!m_Camera)
		{
			return false;
		}

		// Create the texture shader object.
		m_TextureShader = new TextureShaderClass;
		if (!m_TextureShader)
		{
			return false;
		}

		// Initialize the texture shader object.
		result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
			return false;
		}

		// Create the bitmap object.
		m_Bitmap = new BitmapClass;
		if (!m_Bitmap)
		{
			return false;
		}

		// Initialize the bitmap object.
		result = m_Bitmap->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"../Engine/data/ETC/seafloor.dds", 256, 256);
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
			return false;
		}

		// Create the light shader object.
		m_LightShader = new LightShaderClass;
		if (!m_LightShader)
		{
			return false;
		}

		// Initialize the light shader object.
		result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
			return false;
		}

		// Create the light object.
		m_Light = new LightClass;
		if (!m_Light)
		{
			return false;
		}

		// Initialize the light object.
		m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
		m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
		m_Light->SetDirection(0.0f, 0.0f, 1.0f);
		m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
		m_Light->SetSpecularPower(16.f);

		m_BGMSound = new SoundClass;
		m_BGMSound->InitializeBGM(hwnd);

		m_SuccessSound = new SoundClass;
		m_SuccessSound->InitializeSound(hwnd, "../Engine/data/Sound/SuccessSound.wav");

		m_FailSound = new SoundClass;
		m_FailSound->InitializeSound(hwnd, "../Engine/data/Sound/FailSound.wav");

		m_ItemSound = new SoundClass;
		m_ItemSound->InitializeSound(hwnd, "../Engine/data/Sound/ItemSound.wav");

		m_CrashSound = new SoundClass;
		m_CrashSound->InitializeSound(hwnd, "../Engine/data/Sound/CrashSound.wav");

		m_bInit = true;
	}

	m_Camera->Update();
	m_Camera->GetViewMatrix(baseViewMatrix);

	switch (m_eStage)
	{
	case STAGE_TITLE:
	{
		if (m_bReInit[STAGE_TITLE])
			return true;

		m_UIObjectMgr = new GameObjectMgr;
		if (!m_UIObjectMgr)
			return false;

		gameObject = new SceneUIClass;
		dynamic_cast<SceneUIClass*>(gameObject)->InitializeTex(m_D3D->GetDevice());
		dynamic_cast<SceneUIClass*>(gameObject)->SetInput(m_Input);
		dynamic_cast<SceneUIClass*>(gameObject)->SetCamera(m_Camera);
		m_UIObjectMgr->PushGameObject(gameObject);
		m_SceneUIClass = dynamic_cast<SceneUIClass*>(gameObject);
	}
	break;
	case STAGE_MAIN:
	{
		if (m_bReInit[STAGE_MAIN])
			return true;

		// Create the text object.
		m_Text = new TextClass;
		if (!m_Text)
		{
			return false;
		}

		// Initialize the text object.
		result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
			return false;
		}

		m_GameObjectMgr = new GameObjectMgr;
		if (!m_GameObjectMgr)
			return false;

		m_CollisionMgr = new CollisionMgr;
		if (!m_CollisionMgr)
			return false;

		Collision* collision = nullptr;

		gameObject = new SkyboxClass;
		dynamic_cast<SkyboxClass*>(gameObject)->InitializeSkybox(m_D3D->GetDevice());
		m_GameObjectMgr->PushGameObject(gameObject);

		m_Map = new ModelClass;
		m_Map->InitializeObj(m_D3D->GetDevice(), "../Engine/data/Picnic/Picnic.obj", L"../Engine/data/Picnic/Picnic.png");
		m_PolyCnt += m_Map->GetIndexCount();
		m_ObjCnt += 1;

		gameObject = new GoalClass;
		gameObject->InitializeObj(m_D3D->GetDevice(), "../Engine/data/Goal/Goal.obj", L"../Engine/data/Goal/Goaltexture.png");
		gameObject->SetPos(D3DXVECTOR3(0.f, -10.3f, 35.f));
		m_GameObjectMgr->PushGameObject(gameObject);
		collision = gameObject->GetCollision();
		m_CollisionMgr->PushCollObject(collision);

		gameObject = new PlayerClass;
		dynamic_cast<PlayerClass*>(gameObject)->SetInputClass(m_Input);
		gameObject->InitializeObj(m_D3D->GetDevice(), "../Engine/data/Player/Player.obj", L"../Engine/data/Player/Playertexture.png");
		m_GameObjectMgr->PushGameObject(gameObject);

		collision = gameObject->GetCollision();
		m_CollisionMgr->PushCollObject(collision);

		m_Player = dynamic_cast<PlayerClass*>(gameObject);
		D3DXVECTOR3 vCamPos = D3DXVECTOR3(m_Player->GetPos().x, m_Player->GetPos().y + 0.5f, m_Player->GetPos().z - 1.5f);
		m_Camera->SetPosition(vCamPos);

		m_Boat = new ModelClass;
		m_Boat->InitializeObj(m_D3D->GetDevice(), "../Engine/data/Boat/Boat.obj", L"../Engine/data/Boat/Boat.png");

		m_Car = new ModelClass;
		m_Car->InitializeObj(m_D3D->GetDevice(), "../Engine/data/Car/Car.obj", L"../Engine/data/Car/Car.png");

		m_Unicorn = new ModelClass;
		m_Unicorn->InitializeObj(m_D3D->GetDevice(), "../Engine/data/Unicorn/Unicorn.obj", L"../Engine/data/Unicorn/Unicorntexture.png");

		vector<AIClass*> vecAI;
		for (int i = 0; i < 8; ++i)
		//for (int i = 0; i < 1; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				gameObject = new AIClass;

				if (j == 0) dynamic_cast<AIClass*>(gameObject)->SetNumType(AIClass::NUM_1);
				else if (j == 1) dynamic_cast<AIClass*>(gameObject)->SetNumType(AIClass::NUM_2);
				else if (j == 2) dynamic_cast<AIClass*>(gameObject)->SetNumType(AIClass::NUM_3);
				//else dynamic_cast<AIClass*>(gameObject)->SetNumType(AIClass::NUM_4);

				dynamic_cast<AIClass*>(gameObject)->SetModels(m_Boat, m_Car, m_Unicorn);
				dynamic_cast<AIClass*>(gameObject)->SetLine(AIClass::LINE(i));
				m_GameObjectMgr->PushGameObject(gameObject);

				collision = gameObject->GetCollision();
				m_CollisionMgr->PushCollObject(collision);

				vecAI.push_back(dynamic_cast<AIClass*>(gameObject));
			}
			m_GameObjectMgr->PushAI(i, vecAI);
			vecAI.clear();
		}

		for (int i = 0; i < 7; ++i)
		{
			bool bIsHidden = bool(rand() % 2);

			gameObject = new ItemClass;
			dynamic_cast<ItemClass*>(gameObject)->InitializeObj(m_D3D->GetDevice());
			dynamic_cast<ItemClass*>(gameObject)->SetLine(ItemClass::LINE(i));
			dynamic_cast<ItemClass*>(gameObject)->SetHidden(bIsHidden);
			m_GameObjectMgr->PushGameObject(gameObject);

			collision = gameObject->GetCollision();
			m_CollisionMgr->PushCollObject(collision);
		}

		for (int i = 0; i < 2; ++i)
		{
			gameObject = new UIClass;
			dynamic_cast<UIClass*>(gameObject)->SetUIID(UIClass::UIID(i));

			switch (i)
			{
			case 0: gameObject->Initialize(m_D3D->GetDevice(), L"../Engine/data/UI/TimerTextUI.png");	break;
			case 1:
			{
				dynamic_cast<UIClass*>(gameObject)->InitializeTexForNum(m_D3D->GetDevice());
				dynamic_cast<UIClass*>(gameObject)->SetTime((int)m_TimeLimit);
				m_TimerUIClass = dynamic_cast<UIClass*>(gameObject);
			}
			break;
			}

			dynamic_cast<UIClass*>(gameObject)->SetCamera(m_Camera);
			dynamic_cast<UIClass*>(gameObject)->SetPlayer(m_Player);
			m_GameObjectMgr->PushGameObject(gameObject);
		}
	}
	break;
	}
	m_bReInit[(int)m_eStage] = true;

	return true;
}


void GraphicsClass::Shutdown()
{
	if (m_SuccessSound)
	{
		m_SuccessSound->Shutdown();
		delete m_SuccessSound;
		m_SuccessSound = 0;
	}

	if (m_FailSound)
	{
		m_FailSound->Shutdown();
		delete m_FailSound;
		m_FailSound = 0;
	}
	
	if (m_ItemSound)
	{
		m_ItemSound->Shutdown();
		delete m_ItemSound;
		m_ItemSound = 0;
	}
	
	if (m_CrashSound)
	{
		m_CrashSound->Shutdown();
		delete m_CrashSound;
		m_CrashSound = 0;
	}

	if (m_BGMSound)
	{
		m_BGMSound->Shutdown();
		delete m_BGMSound;
		m_BGMSound = 0;
	}

	// Release the bitmap object.
	if (m_Bitmap)
	{
		m_Bitmap->Shutdown();
		delete m_Bitmap;
		m_Bitmap = 0;
	}

	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// Release the light object.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	if (m_CollisionMgr)
	{
		delete m_CollisionMgr;
		m_CollisionMgr = 0;
	}

	if (m_UIObjectMgr)
	{
		delete m_UIObjectMgr;
		m_UIObjectMgr = 0;
	}

	if (m_GameObjectMgr)
	{
		delete m_GameObjectMgr;
		m_GameObjectMgr = 0;
	}

	// Release the model object.
	if (m_Boat)
	{
		m_Boat->Shutdown();
		delete m_Boat;
		m_Boat = 0;
	}

	// Release the model object.
	if (m_Car)
	{
		m_Car->Shutdown();
		delete m_Car;
		m_Car = 0;
	}

	// Release the model object.
	if (m_Unicorn)
	{
		m_Unicorn->Shutdown();
		delete m_Unicorn;
		m_Unicorn = 0;
	}

	// Release the model object.
	if (m_Map)
	{
		m_Map->Shutdown();
		delete m_Map;
		m_Map = 0;
	}

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the D3D object.
	if (m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	// Release the text object.
	if (m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}

	return;
}


bool GraphicsClass::Frame(int mouseX, int mouseY)
{
	bool result;
	static float rotation = 0.0f;


	// Update the rotation variable each frame.
	rotation += (float)D3DX_PI * 0.005f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	// Set the location of the mouse.
	result = m_Text->SetMousePosition(mouseX, mouseY, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	// Render the graphics scene.
	result = Render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool GraphicsClass::Frame(int fps, int cpu, float frameTime)
{
	bool result;

	if (m_Input->IsKeyPressed(DIK_1))
	{
		m_BGMSound->SetVolume(-1000);
		m_SuccessSound->SetVolume(-1000);
		m_FailSound->SetVolume(-1000);
		m_ItemSound->SetVolume(-1000);
		m_CrashSound->SetVolume(-1000);
	}
	else if (m_Input->IsKeyPressed(DIK_2))
	{
		m_BGMSound->SetVolume(1000);
		m_SuccessSound->SetVolume(1000);
		m_FailSound->SetVolume(1000);
		m_ItemSound->SetVolume(1000);
		m_CrashSound->SetVolume(1000);
	}

	m_SceneUIClass->SetSceneID((int)m_eStage);
	switch (m_eStage)
	{
	case STAGE_TITLE:
		KeyUpdate(frameTime);
		m_UIObjectMgr->Frame(frameTime);
		Render();
		break;
	case STAGE_MAIN:
	{
		TextFrame(fps, cpu);

		if (m_TimeLimit > 0.f)
		{
			if (!m_bIsCamMove)
			{
				m_TimeLimit -= 0.01f;
				m_TimerUIClass->SetTime((int)m_TimeLimit);

				m_GameObjectMgr->Frame(frameTime);
				m_GameObjectMgr->AIFrame(frameTime);
				CollisionFrame(frameTime);

				D3DXVECTOR3 vCamPos = D3DXVECTOR3(m_Player->GetPos().x, m_Player->GetPos().y + 0.6f, m_Player->GetPos().z - 2.5f);
				m_Camera->SetPosition(vCamPos);
			}
		}
		else
		{
			if (m_Fail)
			{
				if (m_ChangeStageTime > 3.f)
				{
					m_eStage = STAGE_RESULT;
					m_SceneUIClass->SetTexID(SceneUIClass::TEX_FAIL);
					SetEffectDead();

					m_TimeLimit = 30.f;
					m_Player->ReInit();
					Initialize(m_screenWidth, m_screenHeight, 0);
					ResetItems();

					m_CrashSound->StopSound();
					m_BGMSound->StopSound();
					m_FailSound->PlayGameSound();
				
					m_Player->SetDead(false);
					m_ChangeStageTime = 0;
					m_Fail = false;
					return true;
				}

				m_ChangeStageTime += 0.01f;
				m_Player->SetDead(true);
				
				m_TimerUIClass->SetTime((int)m_TimeLimit);
				m_GameObjectMgr->Frame(frameTime);
				m_GameObjectMgr->AIFrame(frameTime);
			}

			if (!m_Fail)
				m_Fail = true;
		}

		// Render the graphics scene.
		result = Render();
		if (!result)
		{
			return false;
		}

		if (m_Input->IsKeyPressed(DIK_C))
		{
			m_Player->SetCanMove(m_bIsCamMove);
			m_bIsCamMove = !m_bIsCamMove;

			if (!m_bIsCamMove)
			{
				D3DXVECTOR3 vCamPos = D3DXVECTOR3(m_Player->GetPos().x, m_Player->GetPos().y + 0.5f, m_Player->GetPos().z - 1.5f);
				m_Camera->Reset();
				m_Camera->SetPosition(vCamPos);
			}
		}

		if (m_bIsCamMove)
			KeyUpdate(frameTime);
	}
	break;
	case STAGE_RESULT:
		KeyUpdate(frameTime);
		m_UIObjectMgr->Frame(frameTime);
		Render();
		break;
	}

	return true;
}

bool GraphicsClass::Render()
{
	D3DXMATRIX matScale, matRotX, matRotY, matRotZ, matTrans;
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	D3DXVECTOR4 pointDiffuseColor[3];
	D3DXVECTOR4 lightPosition[3];

	pointDiffuseColor[0] = D3DXVECTOR4(1.f, 1.f, 1.f, 1.f);
	pointDiffuseColor[1] = D3DXVECTOR4(1.f, 1.f, 1.f, 1.f);
	pointDiffuseColor[2] = D3DXVECTOR4(1.f, 1.f, 1.f, 1.f);

	lightPosition[0] = D3DXVECTOR4(0.f, -9.5f, -1.f, 1.f);
	lightPosition[1] = D3DXVECTOR4(0.f, -8.f, 35.f, 1.f);
	lightPosition[2] = D3DXVECTOR4(100.f, 1.f, -1.f, 1.f);

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Update();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	switch (m_eStage)
	{
	case STAGE_TITLE:
		m_UIObjectMgr->Render(m_D3D, viewMatrix, projectionMatrix, m_TextureShader, m_LightShader, m_Light, m_Camera->GetPosition(), pointDiffuseColor, lightPosition);
		break;
	case STAGE_MAIN:
	{
		D3DXMatrixScaling(&matScale, 20.f, 10.f, 20.f);
		D3DXMatrixTranslation(&matTrans, -0.1f, -10.16f, 26.f);

		m_Map->Render(m_D3D->GetDeviceContext());
		m_LightShader->Render(m_D3D->GetDeviceContext(), m_Map->GetIndexCount(), matScale * matTrans * worldMatrix, viewMatrix, projectionMatrix, m_Map->GetTexture(),
			m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(), pointDiffuseColor, lightPosition);

		//m_GameObjectMgr->GetGameObject(GameObject::TAG_SKYBOX)->SetPos(m_Camera->GetPosition());
		m_GameObjectMgr->Render(m_D3D, viewMatrix, projectionMatrix, m_TextureShader, m_LightShader, m_Light, m_Camera->GetPosition(), pointDiffuseColor, lightPosition);

		// Turn off the Z buffer to begin all 2D rendering.
		m_D3D->TurnZBufferOff();

		m_D3D->TurnOnAlphaBlending();

		// Render the text strings.
		m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);

		// Turn off alpha blending after rendering the text.
		m_D3D->TurnOffAlphaBlending();

		// Turn the Z buffer back on now that all 2D rendering has completed.
		m_D3D->TurnZBufferOn();
	}
	break;
	case STAGE_RESULT:
		m_UIObjectMgr->Render(m_D3D, viewMatrix, projectionMatrix, m_TextureShader, m_LightShader, m_Light, m_Camera->GetPosition(), pointDiffuseColor, lightPosition);
		break;
	}

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}

void GraphicsClass::TextFrame(int fps, int cpu)
{
	// Set the frames per second.
	m_Text->SetFps(fps, m_D3D->GetDeviceContext());

	// Set the cpu usage.
	m_Text->SetCpu(cpu, m_D3D->GetDeviceContext());

	m_Text->SetPolyCnt(m_PolyCnt + m_GameObjectMgr->GetPolySize(), m_D3D->GetDeviceContext());

	m_Text->SetObjCnt(m_ObjCnt + m_GameObjectMgr->GetSize(), m_D3D->GetDeviceContext());

	m_Text->SetScreenSize(m_screenWidth, m_screenHeight, m_D3D->GetDeviceContext());
}

void GraphicsClass::CollisionFrame(float frameTime)
{
	if (m_bColliOn)
	{
		if (m_ColliCheck > 3.f)
		{
			m_bColliOn = false;
			m_ColliCheck = 0;
			m_Player->SetTouchedColl(nullptr);
			return;
		}
		
		m_ColliCheck += 0.01f;
	}

	if (m_CollisionMgr->UpdateCollsion(Collision::COL_PLAYER, Collision::COL_AI))
	{
		if (m_Player->GetTouchedColl() != m_CollisionMgr->GetTouchedColl())
		{
			m_TimeLimit -= 3.f;
			m_Player->SetTouchedColl(m_CollisionMgr->GetTouchedColl());

			CreateEffect(EffectClass::EFFECT_PARTI);
			m_CrashSound->PlayGameSound();
	}
		else
			m_bColliOn = true;
	}

	if (m_CollisionMgr->UpdateCollsion(Collision::COL_PLAYER, Collision::COL_ITEM))
	{
		if (m_Player->GetTouchedColl() != m_CollisionMgr->GetTouchedColl())
		{
			Collision* ItemCollider = m_CollisionMgr->GetTouchedColl();
			GameObject* gameObject = m_GameObjectMgr->GetGameObject(GameObject::TAG_ITEM, ItemCollider);
			if (dynamic_cast<ItemClass*>(gameObject)->GetHidden())
				return;

			m_Player->SetTouchedColl(ItemCollider);
			dynamic_cast<ItemClass*>(gameObject)->SetHidden(true);
			m_TimeLimit += 3.f;

			CreateEffect(EffectClass::EFFECT_SPARKL);
			m_ItemSound->PlayGameSound();
		}
	}

	if (m_CollisionMgr->UpdateCollsion(Collision::COL_PLAYER, Collision::COL_GOAL))
	{
		m_eStage = STAGE_RESULT;
		m_SceneUIClass->SetTexID(SceneUIClass::TEX_CLEAR);
		SetEffectDead();

		m_TimeLimit = 30.f;
		m_Player->ReInit();
		Initialize(m_screenWidth, m_screenHeight, 0);

		m_BGMSound->StopSound();
		m_SuccessSound->PlayGameSound();
	}
}

void GraphicsClass::KeyUpdate(float frameTime)
{
	DIMOUSESTATE mouseCurrState;

	switch (m_eStage)
	{
	case STAGE_TITLE:
	{			
		if (m_SceneUIClass->GetIsFirst())
			return;

		if (m_Input->IsKeyPressed(DIK_RETURN))
		{
			m_eStage = STAGE_MAIN;
			m_SceneUIClass->SetTexID(SceneUIClass::TEX_END);

			Initialize(m_screenWidth, m_screenHeight, 0);
		}
	}
	break;
	case STAGE_MAIN:
	{
		m_Input->GetDIMouse()->Acquire();
		m_Input->GetDIMouse()->GetDeviceState(sizeof(DIMOUSESTATE), &mouseCurrState);

		float speed = 0.01f * frameTime;

		if (m_Input->IsKeyPressing(DIK_A))
		{
			m_Camera->SetMoveLeftRight(-speed);
		}

		if (m_Input->IsKeyPressing(DIK_D))
		{
			m_Camera->SetMoveLeftRight(speed);
		}

		if (m_Input->IsKeyPressing(DIK_W))
		{
			m_Camera->SetMoveBackForward(speed);
		}

		if (m_Input->IsKeyPressing(DIK_S))
		{
			m_Camera->SetMoveBackForward(-speed);
		}

		if ((mouseCurrState.lX != m_Input->GetMouseState().lX) || (mouseCurrState.lY != m_Input->GetMouseState().lY))
		{
			m_Camera->SetCamYaw(mouseCurrState.lX * 0.001f);
			m_Camera->SetCamPitch(mouseCurrState.lY*0.001f);

			m_Input->SetMouseState(mouseCurrState);
		}
	}
	break;
	case STAGE_RESULT:
	{
		if (m_Input->IsKeyPressed(DIK_RETURN))
		{
			m_SuccessSound->StopSound();
			m_FailSound->StopSound();
			m_BGMSound->PlayBGM();

			m_eStage = STAGE_TITLE;
			m_SceneUIClass->SetTexID(SceneUIClass::TEX_TITLE);

			Initialize(m_screenWidth, m_screenHeight, 0);
			ResetItems();
		}
	}
	break;
	}
}

void GraphicsClass::ResetItems()
{
	list<GameObject*> itemList = m_GameObjectMgr->GetGameObjectListByTag((int)GameObject::TAG_ITEM);
	if (itemList.size() == 0)
		return;

	for (auto iter : itemList)
	{
		ItemClass* tempItem = dynamic_cast<ItemClass*>(iter);

		bool bIsHidden = bool(rand() % 2);
		tempItem->SetHidden(bIsHidden);
		tempItem->SetInit(false);
	}
}

void GraphicsClass::CreateEffect(EffectClass::EFFECTID eEffectID)
{
	switch (eEffectID)
	{
	case EffectClass::EFFECT_PARTI:
	{
		for (int i = 0; i < 7; ++i)
		{
			GameObject* gameObject = new EffectClass;
			dynamic_cast<EffectClass*>(gameObject)->SetGameObjectMgr(m_GameObjectMgr);
			dynamic_cast<EffectClass*>(gameObject)->SetPos(m_Player->GetPos());
			dynamic_cast<EffectClass*>(gameObject)->SetPlayer(m_Player);
			gameObject->InitializeObj(m_D3D->GetDevice(), "../Engine/data/Effect/Cube.obj", L"../Engine/data/Effect/MODELS.bin.014.000.png");
			dynamic_cast<EffectClass*>(gameObject)->SetEffectID(EffectClass::EFFECT_PARTI);
			m_GameObjectMgr->PushGameObject(gameObject);
		}
	}
		break;
	case EffectClass::EFFECT_SPARKL:
	{
		GameObject* gameObject = new EffectClass;
		dynamic_cast<EffectClass*>(gameObject)->SetEffectID(EffectClass::EFFECT_SPARKL);
		gameObject->Initialize(m_D3D->GetDevice(), L"../Engine/data/Effect/Sparkle.png");
		dynamic_cast<EffectClass*>(gameObject)->SetPlayer(m_Player);
		dynamic_cast<EffectClass*>(gameObject)->SetGameObjectMgr(m_GameObjectMgr);
		m_GameObjectMgr->PushGameObject(gameObject);
	}
		break;
	}
}

void GraphicsClass::SetEffectDead()
{
	list<GameObject*> effectList = m_GameObjectMgr->GetGameObjectListByTag((int)GameObject::TAG_EFFECT);
	if (effectList.size() == 0)
		return;

	for (auto iter : effectList)
		dynamic_cast<EffectClass*>(iter)->SetDead(true);
}
