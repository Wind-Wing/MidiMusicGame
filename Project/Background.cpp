#include "stdafx.h"
#include "Background.h"


CBackground::CBackground()
{
}


CBackground::~CBackground()
{
}

void CBackground::m_SetPosition(CRect m_client,int kind)
{
	this->m_client = m_client;
	m_Position.left = 0;
	m_Position.right = this->m_client.right;
	m_Position.top = (int)(this->m_client.bottom*BACKGROUND_TOP);
	m_Position.bottom = (int)(this->m_client.bottom*BACKGROUND_BOTTOM);
	if (kind == 1)
	{
		m_Picture.Load("res\\back.png");
	}
	if (kind ==2)
	{
		m_Picture.Load("res\\back.png");
	}
	
}
