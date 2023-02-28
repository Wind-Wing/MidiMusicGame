#pragma once
#include"Picture.h"

class CTitle:public CPicture
{
#define TITLE_BOTTOM 0.07

public:
	CTitle();
	~CTitle();
	void m_SetPosition(CRect);
	void m_DrawBox(CDC&);
private:
	CRect m_client;
	CImage m_Pointer;
	CRect m_PointerPosition;
};

