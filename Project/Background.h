#pragma once
#include "Picture.h"
#include"Track.h"
class CBackground:public CPicture
{
#define BACKGROUND_TOP 0.07//0.1
#define BACKGROUND_BOTTOM 0.7


public:
	CBackground();
	~CBackground();
	void m_SetPosition(CRect,int);

private:
	CRect m_client;
};

