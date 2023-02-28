#pragma once
#include"Picture.h"

class CTrack
{
#define TRACK_NUMBER 88//61
public:
	CTrack();
	~CTrack();
	void m_SetPosition(CRect,int);
	void m_Print(CDC &,int);
	/*int m_GetPosition(int);*/
private:
	//Using TRACK_NUMBRE instead of TRACK_NUMBER-1 in order to cater to the Position set 
	// in class Keyboard.But the last member in this array will not be used when printing
	CRect m_Position[TRACK_NUMBER ];
	CImage m_Picture;
	CRect m_client;
	//const bool piano_flag[12] = { 0,1,0,0,1,0,1,0,0,1,0,1 };
};

