#include "stdafx.h"
#include "PianoKeyboard.h"
//Attention:
//		 The last key of a MIDIKeyboard always tend to be a white key

CPianoKeyboard::CPianoKeyboard()
{
}


CPianoKeyboard::~CPianoKeyboard()
{
	//delete[] m_key;
}


void CPianoKeyboard::m_Initialize(CRect m_client)
{
	this->m_client = m_client;
	// Preparation before Initialize Tracks' Position
	CRect m_TrackPosition;
	m_Trackcounter = 0;

	m_TrackPosition.top = (int)(m_client.bottom*TRACK_TOP);
	m_TrackPosition.bottom = (int)(m_client.bottom*TRACK_BOTTOM);
	m_TrackPosition.left = 0;
	// Preparation before Initialize Keys' Position
	CRect m_WhitePosition;
	CRect m_BlackPosition;

	//此部分为原版本代码
	//m_WhiteKeyNumber = ((int)KEY_NUMBER / 12) * 7;
	//for (int counter = 0; counter < (KEY_NUMBER % 12); counter++)
	//{
	//	if (!piano_flag[counter])m_WhiteKeyNumber++;
	//	else if (counter == (KEY_NUMBER % 12) - 1)m_WhiteKeyNumber++;
	//}

	//此部分为改动后代码，改动时间：2016年6月10日
	if (KEY_NUMBER == 61) {
		m_WhiteKeyNumber = 36;
	}
	if (KEY_NUMBER == 88) {
		m_WhiteKeyNumber = 52;
	}

	float m_Step = (float)((float)this->m_client.right / (float)m_WhiteKeyNumber);
	float buffer = 0.0;
	m_WhitePosition.top = (int)(this->m_client.bottom)*KEY_BOARD_TOP;
	m_WhitePosition.bottom = (int)(this->m_client.bottom)*KEY_BOARD_BOTTOM;
	m_WhitePosition.right = 0;
	m_WhitePosition.left = 0;
	m_BlackPosition.top = m_WhitePosition.top;
	m_BlackPosition.bottom = (int)(m_WhitePosition.top + 0.7*m_WhitePosition.Height());

	//Initialize Keys Positions

	//改动时间：2016年6月10日
	m_Print88PianoKey(m_WhitePosition, m_BlackPosition, m_TrackPosition, buffer, m_Step);

	// Initialize PianoBar
	m_PianoBar.Load("res\\pianobar.png");
	m_PianoBarPosition.left = this->m_client.left;
	m_PianoBarPosition.right = this->m_client.right;
	m_PianoBarPosition.top = (int)(this->m_client.bottom * KEY_BOARD_BOTTOM);
	m_PianoBarPosition.bottom = (int)(this->m_client.bottom * PIANO_BAR_BOTTOM);
}

void CPianoKeyboard::m_Print88PianoKey(CRect m_WhitePosition, CRect m_BlackPosition, CRect &m_TrackPosition, float &buffer, float m_Step) {

	for (int counter = 0; counter < KEY_NUMBER; counter++)
	{
		if (counter == 0) {
			m_SetWhiteKeyPosition(m_WhitePosition, buffer, m_Step, counter);
			m_SetTrackPosition(m_WhitePosition, m_TrackPosition, m_Track, m_Trackcounter);
		}
		else if (counter == 1) {
			m_SetBlackKeyPosition(m_WhitePosition, m_BlackPosition, buffer, m_Step, counter);
		}
		else if (counter == 2) {
			m_SetWhiteKeyPosition(m_WhitePosition, buffer, m_Step, counter);
			m_SetTrackPosition(m_WhitePosition, m_TrackPosition, m_Track, m_Trackcounter);
		}
		else {
			int flag = counter % 12;
			if ((piano_flag_88keys[flag] == 0) || (counter == KEY_NUMBER - 1))
			{
				m_SetWhiteKeyPosition(m_WhitePosition, buffer, m_Step, counter);
				m_SetTrackPosition(m_WhitePosition, m_TrackPosition, m_Track, m_Trackcounter);
			}
			if (piano_flag_88keys[flag] == 1)
			{
				m_SetBlackKeyPosition(m_WhitePosition, m_BlackPosition, buffer, m_Step, counter);
			}
		}
	}
	// Intialize Spark
	for (int counter = 0; counter < KEY_NUMBER; counter++)
	{
		m_Spray[counter].m_Initialize(m_client);
	}
}

void CPianoKeyboard::m_SetWhiteKeyPosition(CRect &m_WhitePosition, float &buffer, float m_Step, int counter) {
	//Set White Piano key Position
	m_WhitePosition.left = m_WhitePosition.right;
	buffer += m_Step;
	m_WhitePosition.right = (int)buffer;
	m_key[counter].m_SetPosition(m_WhitePosition);// 传入设置的琴键位置
	m_key[counter].m_SetColor(0);				  // 设置琴键的颜色 0：白色 1：黑色
	 // Load Key Pictures
	m_key[counter].m_LoadPicture();

	CImage dol;
	CRect pos = m_WhitePosition;
	if (counter == 3)dol.Load("res\\do_1.png");
}

void CPianoKeyboard::m_SetBlackKeyPosition(CRect &m_WhitePosition, CRect m_BlackPosition, float &buffer, float m_Step, int counter) {
	//Set Black Piano key Position
	//m_BlackPosition.left = (int)m_WhitePosition.left + m_Step*3.0 / 4.0;
	//m_BlackPosition.right = (int)m_BlackPosition.left + m_Step / 2.0;
	m_BlackPosition.left = (int)m_WhitePosition.right - m_Step / 4.0;
	m_BlackPosition.right = (int)m_WhitePosition.right + m_Step / 4.0;
	m_key[counter].m_SetPosition(m_BlackPosition);
	m_key[counter].m_SetColor(1);
	// Load Key Pictures
	m_key[counter].m_LoadPicture();
}

void CPianoKeyboard::m_SetTrackPosition(CRect &m_WhitePosition, CRect &m_TrackPosition, CTrack &m_Track, int &m_Trackcounter) {
	// Set Track Position
	m_TrackPosition.right = m_WhitePosition.right;
	m_Track.m_SetPosition(m_TrackPosition, m_Trackcounter++);
}

void CPianoKeyboard::m_KeyDown(int m_InputPitch,bool m_InputColor)
{
	int buf = m_InputPitch - LOW_PITCH;
	if(m_InputColor)
	m_key[buf].m_ChangePicture(1); // Right Key
	else 
	m_key[buf].m_ChangePicture(3); // Wrong Key

	m_Spray[buf].m_SetPosition(m_GetKeyPosition(buf));
	m_Spray[buf].m_ChangeState(true);
}

void CPianoKeyboard::m_KeyUp(int m_InputPitch)
{
	int buf = m_InputPitch - LOW_PITCH;
	m_key[buf].m_ChangePicture(2); // UnPressed Key
	m_Spray[buf].m_ChangeState(false);
}
CRect CPianoKeyboard::m_GetKeyPosition(int m_Index)
{
	return m_key[m_Index].m_GetKeyPosition();
}

void CPianoKeyboard::m_Reset()
{
	// Simulate the KeyUp Event to Every Key
	for (int counter = 0; counter < KEY_NUMBER; counter++)
	{
		m_key[counter].m_ChangePicture(2);
		m_Spray[counter].m_ChangeState(false);
	}
}

void CPianoKeyboard::m_Print(CDC &m_cacheDC)
{
	// Print tracks
	m_Track.m_Print(m_cacheDC, m_Trackcounter);
	// Print keyboard
	//首先判断键盘类型，此处处理88键类型
	if (KEY_NUMBER == 88) {
		for (int counter = 0; counter < KEY_NUMBER; counter++)
			if (!piano_flag_88keys[counter % 12] || counter == KEY_NUMBER - 1)m_key[counter].m_Print(m_cacheDC);
		for (int counter = 0; counter < KEY_NUMBER - 1; counter++)
			if (piano_flag_88keys[counter % 12])m_key[counter].m_Print(m_cacheDC);
	}
	// Print PianoBar
	m_PianoBar.Draw(m_cacheDC, m_PianoBarPosition);
	// Print Spray
	for (int counter = 0; counter < KEY_NUMBER; counter++)
	{
		if (m_Spray[counter].m_CanShow())m_Spray[counter].m_Display(m_cacheDC);
	}
}

