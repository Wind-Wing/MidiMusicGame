#include "stdafx.h"
#include "Picture.h"


CPicture::CPicture()
{
}

CPicture::~CPicture()
{
	//m_Picture.Destroy();
}

 void CPicture::m_SetPosition(CRect m_Position)
{
	this->m_Position = m_Position;
}

void CPicture::m_Print(CDC &m_cacheDC)
{
	m_Picture.Draw(m_cacheDC, m_Position);
}
