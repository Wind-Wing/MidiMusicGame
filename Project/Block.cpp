#include "stdafx.h"
#include "Block.h"


CBlock::CBlock(int m_Pitch, CRect m_client)
{
	this->m_Pitch = m_Pitch;
	this->m_client = m_client;

	// Load the picture of the block
	if (m_Pitch == 108)
	{
		m_Picture[0].Load("res\\block7.png");
		m_Picture[1].Load("res\\blockchange7.png");
	}
	else if (m_Pitch < 24)
	{
		m_Picture[0].Load("res\\block1.png");
		m_Picture[1].Load("res\\blockchange1.png");
	}
	else
	{
		CString buffer;
		buffer.Format("res\\block%d.png", m_Pitch / 12 - 1);
		m_Picture[0].Load(buffer);
		buffer.Format("res\\blockchange%d.png", m_Pitch / 12 - 1);
		m_Picture[1].Load(buffer);
	}

	m_DestroyMark = 0;
	m_StavePointLock = 1;
	m_ButtonDownDistance = 0;
	m_Score = 0;
	m_ScoreLock = false;
	m_CurrentPicture = 0;
}

CBlock::~CBlock()
{
}

bool CBlock::m_ChangePosition(int step)
{
	m_Position.top += step;
	m_Position.bottom += step;
	// Judge Whether the right Position to Destroy

	//原版本
	//if (m_Position.top >= (int)(m_client.bottom*KEY_BOARD_BOTTOM))
		//m_DestroyMark = TRUE;

	//修改后版本，时间：2016年6月14日
	//修改块销毁的地方
	if (m_Position.top >= (int)(m_client.bottom*KEY_BOARD_TOP))
	{
		m_DestroyMark = TRUE;
	}

	if (m_Position.bottom >= (int)(m_client.bottom*KEY_BOARD_TOP))return TRUE;
	else return FALSE;
}

int CBlock::m_ChangePicture(bool m_InputCommand,int m_BlockUnitLength)
{	
	// 2 Means unchanged
	int m_ReturnValue = 2;
	// Check whether the Allowed Position to change Picture
	if (m_Position.bottom >= (int)(m_client.bottom*KEY_BOARD_TOP - m_BlockUnitLength*ALLOWED_ERROR) && m_Position.top <= (int)(m_client.bottom*KEY_BOARD_TOP + m_BlockUnitLength*ALLOWED_ERROR))
	{
		//m_ReturnValue = FALSE;
		if (m_InputCommand)
		{
			m_ReturnValue = TRUE;
			//m_Picture.Destroy();
			//(m_Pitch>60) ? m_Picture.Load("res\\blockchangeright.png") : m_Picture.Load("res\\blockchangeleft.png");
			m_CurrentPicture = 1;
			// this value is used to calculate the score
			if (m_ScoreLock == FALSE) // Avoid the repeat message of keydown
			{
				m_ButtonDownDistance = m_GetDistance();
				m_ScoreLock = TRUE;
			}
		}
		else
		{
			m_ReturnValue = FALSE;
			/// Must be careful to handle this value
			/// Such operator is for the time being
				//m_DestroyMark = TRUE;
			//m_Picture.Destroy();
			//(m_Pitch>60) ? m_Picture.Load("res\\blockright.png") : m_Picture.Load("res\\blockleft.png");
			m_CurrentPicture = 0;
			if (m_ScoreLock == TRUE)
			{
				m_Score += (m_GetDistance() - m_ButtonDownDistance);
				m_ScoreLock = FALSE;
			}
		}
	}
	return m_ReturnValue;
}

bool CBlock::m_IsStavePointMove()
{
	if (m_StavePointLock && m_Position.bottom >= (int)(KEY_BOARD_TOP*m_client.Height()))
	{
		m_StavePointLock = 0;
		return TRUE;
	}
	else return FALSE;
}

int CBlock::m_GetDistance()
{
	int m_Border = (int)(m_client.bottom*KEY_BOARD_TOP);

	if (m_Position.bottom <= m_Border)return 0;
	else if (m_Position.top >= m_Border)return m_Position.Height();
	else return (m_Position.bottom - m_Border);
}

int CBlock::m_GetScore()
{
	if(m_ScoreLock)m_Score += (m_GetDistance() - m_ButtonDownDistance);
	// 1 means miss
	// 2 means good
	// 3 means great
	// 4 means perfect
	/*if (m_Position.Height() == m_ButtonDownDistance)return 4;
	if (m_Score < 1) return 1;
	return (int)((m_Score * 4 - 1) / (int)m_Position.Height())+1;*/
	if		(4*m_Score >= 3*m_Position.Height())    return 4;//4
	else if (4 * m_Score >= 2 * m_Position.Height())return 3;//3
	else if (4 * m_Score >= 1 * m_Position.Height())return 2;//2
	else return 1;
}

bool CBlock::m_IsDestroy()
{
	
	return m_DestroyMark;
}

void CBlock::m_Print(CDC& m_cacheDC)
{
	m_Picture[m_CurrentPicture].Draw(m_cacheDC, m_Position);
}