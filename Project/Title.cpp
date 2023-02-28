#include "stdafx.h"
#include "Title.h"


CTitle::CTitle()
{
	m_Picture.Load("res\\title.png");
	m_Pointer.Load("res\\pointer.png");
}


CTitle::~CTitle()
{
}

void CTitle::m_SetPosition(CRect m_client)
{
	this->m_client = m_client;
	m_Position = (this->m_client);
	m_Position.bottom = (int)(this->m_client).bottom*TITLE_BOTTOM;
	m_PointerPosition.top = (int)(m_client.bottom * 0.075);
	m_PointerPosition.bottom = (int)(m_client.bottom * 0.11);
	m_PointerPosition.left = (int)(m_client.right * 0.76);
	m_PointerPosition.right = (int)(m_client.right * 0.78);
}

void CTitle::m_DrawBox(CDC &m_Input)
{
	m_Pointer.Draw(m_Input, m_PointerPosition);
}
