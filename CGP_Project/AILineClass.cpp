#include "AILineClass.h"

AILineClass::AILineClass()
{
}

AILineClass::AILineClass(const AILineClass &)
{
}

AILineClass::~AILineClass()
{
}

bool AILineClass::Frame(float)
{
	if (!m_bInit)
	{
		D3DXVECTOR3 vPos;
		switch (m_eLine)
		{
		case LINE_1:
			vPos = D3DXVECTOR3(-5.f, -9.995f, 2.f);
			break;
		case LINE_2:
			vPos = D3DXVECTOR3(-5.f, -9.995f, 6.f);
			break;
		case LINE_3:
			vPos = D3DXVECTOR3(-5.f, -9.995f, 10.f);
			break;
		case LINE_4:
			vPos = D3DXVECTOR3(-5.f, -9.995f, 14.f);
			break;
		case LINE_5:
			vPos = D3DXVECTOR3(-5.f, -9.995f, 18.f);
			break;
		case LINE_6:
			vPos = D3DXVECTOR3(-5.f, -9.995f, 22.f);
		break; 
		case LINE_7:
			vPos = D3DXVECTOR3(-5.f, -9.995f, 26.f);
		break; 
		case LINE_8:
			vPos = D3DXVECTOR3(-5.f, -9.995f, 30.f);
		break; 
		case LINE_END:
		default:
			return true;
		}

		SetPos(vPos);
		m_bInit = true;
	}

	return true;
}

void AILineClass::Initialize()
{
	m_eLine = LINE_END;
}

void AILineClass::Shutdown()
{
}
