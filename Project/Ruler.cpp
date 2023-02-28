#include "stdafx.h"
#include "Ruler.h"


CRuler::CRuler()
{

	//Load Picture
	m_Ruler.Load("res\\ruler.png");
	m_Arrow1.Load("res\\arrow1.png");
	m_Arrow2.Load("res\\arrow2.png");
	m_RulerText.Load("res\\rulertext.png");
	for (int counter = 0; counter < 10; counter++)
	{
		CString buf;
		buf.Format("res\\%d.png", counter);
		m_NumberPictures[counter].Load(buf);
	}
}

CRuler::~CRuler() {}

void CRuler::m_Initialize(CRect m_client)
{
	this->m_client;
	// Set Position
	m_RulerSpacePosition.top = (int)(m_client.bottom * RULER_SPACE_TOP);
	m_RulerSpacePosition.bottom = (int)(m_client.bottom);
	m_RulerSpacePosition.left = (int)(m_client.right*RULER_SPACE_LEFT);
	m_RulerSpacePosition.right = (int)(m_client.right*RULER_SPACE_RIGHT);
	// Set Ruler Position
	m_RulerPosition.top = m_RulerSpacePosition.top;
	//m_RulerPosition.bottom = m_RulerSpacePosition.bottom*0.93;
	//调整尺子长度
	m_RulerPosition.bottom = m_RulerSpacePosition.bottom*0.94;
	m_RulerPosition.left = (int)(m_RulerSpacePosition.left + m_RulerSpacePosition.Width()*0.25);
	m_RulerPosition.right = (int)(m_RulerSpacePosition.left + m_RulerSpacePosition.Width()*0.5);
	// Set Ruler Text Position
	m_RulerTextPosition.left = m_RulerPosition.right;
	m_RulerTextPosition.right = m_RulerSpacePosition.right;
	m_RulerTextPosition.top = (int)(m_RulerSpacePosition.top + m_RulerSpacePosition.Height()*0.8);
	m_RulerTextPosition.bottom = m_RulerSpacePosition.bottom;

	// Set Arrows Position
	m_ArrowPosition1.left = m_RulerSpacePosition.left;
	m_ArrowPosition1.right = m_RulerPosition.left;
	m_ArrowHeight = (int)(m_ArrowPosition1.Width() *0.25);
	if (!m_ArrowHeight)m_ArrowHeight = 1;//Avoid zero
	m_ArrowPosition1.top = m_RulerSpacePosition.bottom;
	m_ArrowPosition1.bottom = m_ArrowPosition1.top + m_ArrowHeight;
	m_ArrowPosition2 = m_ArrowPosition1;
	// Set Pitch Step
	int m_PitchStartHeight = (int)(m_RulerSpacePosition.top + m_ArrowHeight*0.5);
	int m_PitchEndHeight = (int)(m_RulerSpacePosition.bottom - m_ArrowHeight*0.5);
	m_PitchStep = (float)((m_PitchEndHeight - m_PitchStartHeight) / (MIDI_KEYNUMBER - 1));
	// Set Number Position
	//移动刻度尺数值显示位置
	//m_NumberPosition.left = m_RulerPosition.right;
	m_NumberPosition.left = m_RulerPosition.right + m_RulerSpacePosition.Width()*0.05;
	m_NumberPosition.right = m_RulerSpacePosition.right;
	m_NumberHeight = m_NumberPosition.Width();
	m_NumberWidth = (int)(m_NumberHeight / 2.0);
	m_NumberPosition.top = m_RulerSpacePosition.bottom;
	m_NumberPosition.bottom = m_RulerSpacePosition.bottom + m_NumberHeight;
	// Set PitchInterval and FormerPitch
	m_PitchInterval = 0;
	m_FormerPitch = 0;
}

void CRuler::m_ChangeArrowPosition(int m_Pitch)
{
	m_ArrowPosition2 = m_ArrowPosition1;
	m_ArrowPosition1.top = m_RulerPosition.top + (int)(m_Pitch - LOW_PITCH)*m_PitchStep;
	m_ArrowPosition1.bottom = m_ArrowPosition1.top + m_ArrowHeight;
	m_ChangeNumberPosition();
	if (m_FormerPitch)
	{
		m_PitchInterval = abs(m_Pitch - m_FormerPitch);
	}
	m_FormerPitch = m_Pitch;
}

void CRuler::m_ChangeNumberPosition()
{
	float buf = (m_ArrowPosition1.top + m_ArrowPosition2.bottom) / 2.0;
	//m_NumberPosition.top = (int)(buf - m_NumberHeight / 2.0);
	//m_NumberPosition.bottom = m_NumberPosition.top + m_NumberHeight;
	m_NumberPosition.top = m_NumberHeight;
	m_NumberPosition.bottom = m_NumberHeight;
	if (m_NumberPosition.top < m_RulerSpacePosition.top)
	{
		m_NumberPosition.top = m_RulerSpacePosition.top + m_ArrowHeight;
		m_NumberPosition.bottom = m_NumberPosition.top + m_NumberHeight;
	}
	else if (m_NumberPosition.bottom>m_RulerSpacePosition.bottom)
	{
		m_NumberPosition.bottom = m_RulerSpacePosition.bottom;
		m_NumberPosition.top = m_NumberPosition.bottom - m_NumberHeight;
	}
}

void CRuler::m_NumberPrint(CDC& m_cacheDC)
{
	int buf = (int)(m_PitchInterval / 10);
	if (buf == 0)
	{
		m_NumberPictures[m_PitchInterval % 10].Draw(m_cacheDC, m_NumberPosition);
	}
	else if (buf > 0)
	{
		m_NumberPictures[buf].Draw(m_cacheDC, m_NumberPosition.left, m_NumberPosition.top, m_NumberWidth, m_NumberHeight);
		m_NumberPictures[m_PitchInterval % 10].Draw(m_cacheDC, m_NumberPosition.left + m_NumberWidth, m_NumberPosition.top, m_NumberWidth, m_NumberHeight);
	}
}

void CRuler::m_Print(CDC &m_cacheDC)
{
	m_Ruler.Draw(m_cacheDC, m_RulerPosition);
	m_Arrow1.Draw(m_cacheDC, m_ArrowPosition1);
	m_Arrow2.Draw(m_cacheDC, m_ArrowPosition2);
	m_NumberPrint(m_cacheDC);
	m_RulerText.Draw(m_cacheDC, m_RulerTextPosition);
}

