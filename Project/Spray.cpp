#include "stdafx.h"
#include "Spray.h"

CSpray::CSpray():m_Spraycounter(0),m_isShow(FALSE)
{
	// Load picture must finished in this place 
	// or will error
	CString buf;
	for (int counter = 0; counter < SPRAY_NUMBER; counter++)
	{
		buf.Format("res\\spray%d.png", counter + 1);
		m_SprayPicture[counter].Load(buf);
	}
}
CSpray::~CSpray()
{
}
void CSpray::m_Initialize(CRect m_client)
{
	m_SprayPosition.top = (int)(m_client.bottom*SPRAY_TOP);
	m_SprayPosition.bottom = (int)(m_client.bottom*KEY_BOARD_TOP);
}

void CSpray::m_SetPosition(CRect& m_Input)
{
	m_SprayPosition.left = m_Input.left;
	m_SprayPosition.right = m_Input.right;
}

void CSpray::m_Display(CDC& m_cacheDC)
{
	m_Spraycounter = (m_Spraycounter >= SPRAY_NUMBER*SPRAY_DELAY) ? 0 : m_Spraycounter;
	m_SprayPicture[int(m_Spraycounter/SPRAY_DELAY)].Draw(m_cacheDC, m_SprayPosition);
	m_Spraycounter++;
}
