#include "stdafx.h"
#include "PianoKey.h"


CPianoKey::CPianoKey()
{
}


CPianoKey::~CPianoKey()
{
}

//void CPianoKey::m_SetPosition(CRect m_Position)
//{
//	this->m_Position = m_Position;
//}

void CPianoKey ::m_SetColor(bool color)
{
	this->color = color;
}

void CPianoKey::m_ChangePicture(int m_Inputcolor)
{
	switch (m_Inputcolor)
	{
	//case 0: m_CurrentKey = this->color; break;
	// Key Down
	case 1: m_CurrentKey = 2; break; // Right Key
	case 3: m_CurrentKey = 3; break; // Wrong Key
	// Key Up
	case 2: m_CurrentKey = this->color; break;
	}
}

void CPianoKey::m_LoadPicture()
{
	/*if (this->color)m_KeyPicture[0].Load("res\\black.png");
	else if (!(this->color))m_KeyPicture[0].Load("res\\white.png");
	else MessageBox(NULL, "Keyboard picture load fail,please check file .\\res", "Error", MB_ICONERROR | MB_OK);*/
	m_KeyPicture[0].Load("res\\pianokeywhite.png");
	m_KeyPicture[1].Load("res\\pianokeyblack.png");
	m_KeyPicture[2].Load("res\\pianokeygreen.png");
	m_KeyPicture[3].Load("res\\pianokeyred.png");
	m_CurrentKey = this->color;
}
//
//void CPianoKey::m_Print(CDC &m_cacheDC)
//{
//	m_Picture.Draw(m_cacheDC, m_Position);
//}



// Function :InterFace Function
// This function is used to Initialize the Position of Blocks
CRect CPianoKey::m_GetKeyPosition()
{
	return m_Position;
}
void CPianoKey:: m_Print(CDC& m_cacheDC)
{
	m_KeyPicture[m_CurrentKey].Draw(m_cacheDC, m_Position);
}
void CPianoKey::m_SetPosition(CRect m_Input)
{
	m_Position = m_Input;
}


