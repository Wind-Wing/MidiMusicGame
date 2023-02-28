#include "stdafx.h"
#include "Track.h"


CTrack::CTrack()
{
	m_Picture.Load("res\\track.png");
}

CTrack::~CTrack()
{
}

void CTrack::m_SetPosition(CRect m_Position, int index)
{
	m_Position.left = m_Position.right -2;
	this->m_Position[index] = m_Position;
}

void CTrack::m_Print(CDC& m_cacheDC,int m_InputTrackNumber)
{
	for (int counter = 0; counter < m_InputTrackNumber; counter++)
	{
		m_Picture.Draw(m_cacheDC, m_Position[counter]);
	}
}

//int CTrack::m_GetPosition(int index)
//{
//	if (index < TRACK_NUMBER)
//		return (m_Position[index].left);
//	else if (index == TRACK_NUMBER)
//		return m_client.right;
//}