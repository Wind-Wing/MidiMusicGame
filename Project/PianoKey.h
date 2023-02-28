#pragma once
#include "Picture.h"

class CPianoKey
{
public:
	CPianoKey();
	~CPianoKey();
	//void m_SetPosition(CRect);
	void m_LoadPicture();
	void m_SetColor(bool);
	void m_ChangePicture(int);
	void m_Print(CDC&);
	void m_SetPosition(CRect);
	// Function :InterFace Function
	CRect m_GetKeyPosition(void);
	//void m_Print(CDC &m_cacheDC);
private:
	CRect m_Position;
	//CImage m_Picture;
	bool color;
	CImage m_KeyPicture[4];
	int m_CurrentKey;


};

