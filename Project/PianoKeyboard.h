#pragma once
#include "PianoKey.h"
#include "Track.h"
#include "Spray.h"

class CPianoKeyboard
{
#define KEY_BOARD_BOTTOM 0.67//0.7
	// KEY_BOARD_POSITION is calculate from the keyboard top
#define KEY_BOARD_TOP 0.47//0.50
	// KEY_BOARD_TOP is also used in Spray
#define PIANO_BAR_BOTTOM 0.7
#define KEY_NUMBER 88//61
#define TRACK_TOP 0.07//0.1
#define TRACK_BOTTOM 0.47//0.5
	// They are also used in BlockControl.h and ProjectDlg.h
#define LOW_PITCH 21//14
#define HIGH_PITCH 108//84

public:
	CPianoKeyboard();
	~CPianoKeyboard();
	void m_Print(CDC &m_cacheDC);
	void m_Initialize(CRect);
	void m_KeyDown(int,bool);
	void m_KeyUp(int);
	// Temporarily put Class Track in public in order to Initialize Track Position
	CTrack m_Track;
	// Function :InterFace Function
	CRect m_GetKeyPosition(int);
	void m_Reset();

	//ÉèÖÃ°×¼üºÍºÚ¼üÎ»ÖÃ
	void CPianoKeyboard::m_SetWhiteKeyPosition(CRect &m_WhitePositio, float &buffer, float m_Step, int counter);
	void CPianoKeyboard::m_SetBlackKeyPosition(CRect &m_WhitePosition, CRect m_BlackPosition, float &buffer, float m_Step, int counter);

	void CPianoKeyboard::m_SetTrackPosition(CRect &m_WhitePosition, CRect &m_TrackPosition, CTrack &m_Track, int &m_Trackcounter);

	//»­88¼ü¸ÖÇÙ
	void CPianoKeyboard::m_Print88PianoKey(CRect m_WhitePosition, CRect m_BlackPosition, CRect &m_TrackPosition, float &buffer, float m_Step);
private:
	CImage m_PianoBar;
	CRect m_PianoBarPosition;
	CPianoKey m_key[KEY_NUMBER];
	CSpray m_Spray[KEY_NUMBER];
	CRect m_client;
	int m_WhiteKeyNumber;
	int m_Trackcounter;
	//const int piano_flag[12] = { 0,1,0,1,0,0,1,0,1,0,1,0 };
	const int piano_flag_88keys[12] = { 0,1,0,0,1,0,1,0,0,1,0,1 };
};