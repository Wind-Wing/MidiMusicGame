#include "stdafx.h"
#include "BlockControl.h"



CBlockControl::CBlockControl()
{
	//m_CurrentBlockNumber = 0;
	//m_PitchFilecounter = 0;
	//m_TimesFilecounter = 0;
	//// Used to Control Time Interval of neighbouring Blocks
	//m_Timescounter = 0;
	//m_FormPitch = 0;
	m_Reset(CHALLENGE_MODEL);
}

CBlockControl::~CBlockControl()
{
	for (int counter = 0; counter < m_CurrentBlockNumber; counter++)
	{
		delete m_BlockPointer[counter];
	}
}

int CBlockControl::m_GetText()
{
	FILE *m_music;
	if (m_Model == CHALLENGE_MODEL || m_Model == LEARNING_MODEL)
	{
		fopen_s(&m_music, "exe\\MaxT.out", "r");
	}
	if (m_Model == CREATE_MODEL)
	{
		switch (m_CreateModelOption)
		{
		case CREATE_MODEL_FREEPLAY:fopen_s(&m_music, "res\\freeplay.music", "r"); break;
		case CREATE_MODEL_PIE:fopen_s(&m_music, "res\\pie.music", "r"); break;
		case CREATE_MODEL_E:fopen_s(&m_music, "res\\e.music", "r");  break;
		case CREATE_MODEL_SQUARE2:fopen_s(&m_music, "res\\square2.music", "r"); break;
		}
	}
	// Drop value c
	float Buffer;
	fscanf_s(m_music, "%f", &Buffer);
	for (int counter = 0; counter < MUSIC_LENGTH; counter++)
	{	
		fscanf_s(m_music, "%d", &m_Pitch[counter]);
		if (-1 == m_Pitch[counter])break;
		fscanf_s(m_music, "%f", &m_StartTime[counter]);
		fscanf_s(m_music, "%d", &m_Times[counter]);
	}
	fclose(m_music);
	return (int)(1000.0 * m_StartTime[0]);
}

int CBlockControl::m_Initialize(CRect m_client, CPianoKeyboard& m_Keyboard)
{
	// Get the Size of Client
	this->m_client = m_client;
	// Set the LearnModel Flags
	m_LearningAndCreateModelKeyDownFlag = FALSE;
	m_LearningAndCreateModelTouchLineFlag = FALSE;
	m_ChangeCreateModelOption(CREATE_MODEL_FREEPLAY);
	// Set the CreateModel Lock
	m_CreateModelPitchToNumberLock = TRUE;
	// Copy the Position of Keys
	for (int counter = 0; counter < KEY_NUMBER; counter++)
	{
		m_KeyPosition[counter] = m_Keyboard.m_GetKeyPosition(counter);
		////Test Code
		//CString buf;
		//buf.Format("%d",m_TrackPosition[counter]);
		//MessageBox(NULL, buf, "Client", true);
	}
	// Use a WhiteKey's Width as UnitLength
	//m_BlockUnitLength = m_KeyPosition[0].right;
	m_BlockUnitLength = m_KeyPosition[0].right*2;
	return (int)m_BlockUnitLength;
}

// Function Usage:
//		This function control the Size of Block and the Time Interval of Two Blocks
void CBlockControl::m_BlockCreate()
{
	if ((m_Model == LEARNING_MODEL || m_Model == CREATE_MODEL) 
		&& m_LearningAndCreateModelKeyDownFlag == FALSE
		&&m_LearningAndCreateModelTouchLineFlag == TRUE) return ;

	if (m_Model == CREATE_MODEL && m_CreateModelOption == CREATE_MODEL_FREEPLAY)return ;


	// When the Pitch is 0 means a short pause in a song
	if (m_Pitch[m_PitchFilecounter] == 0)
	{
		if (m_Timescounter == 0)
		{
			// Reset m_Timescounter
			m_Timescounter = m_Times[m_TimesFilecounter++] - 1;
			// Move the Key of Array to Next Position
			m_PitchFilecounter++;
			m_IsMusicEndLock = FALSE;
		}
		else if (m_Timescounter > 0)m_Timescounter--;
		return;
	}

	// When the Pitch is not 0
	if (m_PitchFilecounter < MUSIC_LENGTH && m_Pitch[m_PitchFilecounter] != -1 )
		// Some small problem
	{
		if (m_Pitch[m_PitchFilecounter] >= LOW_PITCH&&m_Pitch[m_PitchFilecounter] <= HIGH_PITCH)
		{
			if (m_Timescounter == 0)
			{
				// Create a New Block Object
				m_BlockPointer[m_CurrentBlockNumber] = new CBlock(m_Pitch[m_PitchFilecounter], m_client);

				// Set This Block's initial Position
				int m_Index = m_Pitch[m_PitchFilecounter] - LOW_PITCH;
				m_Position.top = -m_Times[m_TimesFilecounter] * m_BlockUnitLength;
				m_Position.bottom = 0;
				m_Position.left = m_KeyPosition[m_Index].left;
				m_Position.right = m_KeyPosition[m_Index].right;

				m_BlockPointer[m_CurrentBlockNumber]->m_SetPosition(m_Position);

				// Reset m_Timescounter
				m_Timescounter = m_Times[m_TimesFilecounter++] - 1;
				// Move the Key of Array to Next Position
				m_CurrentBlockNumber++;
				m_PitchFilecounter++;
				m_IsMusicEndLock = FALSE;
			}
			else if (m_Timescounter > 0)m_Timescounter--;
		}
		else m_PitchFilecounter++;
	}
}

void CBlockControl::m_BlockDropping()
{
	if (m_Model == CHALLENGE_MODEL )
	{
		for (int counter = 0; counter < m_CurrentBlockNumber; counter++)
		{
			m_BlockPointer[counter]->m_ChangePosition(BLOCK_DROPPING_STEP);
		}
	}
	if (m_Model == LEARNING_MODEL || m_Model == CREATE_MODEL)
	{
		int buf;
		if (m_LearningAndCreateModelKeyDownFlag == TRUE || m_LearningAndCreateModelTouchLineFlag == FALSE)
		{
			for (int counter = 0; counter < m_CurrentBlockNumber; counter++)
			{
				buf = m_BlockPointer[counter]->m_ChangePosition(BLOCK_DROPPING_STEP);
				if (!m_LearningAndCreateModelTouchLineFlag)m_LearningAndCreateModelTouchLineFlag = buf;
			}
		}
	}
	
}

int CBlockControl::m_BlockDestroy(CPowerLow &m_PowerLow)
{
	int buffer = m_CurrentBlockNumber;
	int m_ReturnValue =0;
	for (int counter = 0; counter < buffer; counter++)
	{
		if (m_BlockPointer[counter]->m_IsDestroy())
		{
			// Statistic Data for PowerLow 
				// -1 is usd to Corresponding the Lower bound of Array
			m_PowerLowValue = abs(m_FormPitch - m_BlockPointer[counter]->m_Pitch) -1;
			m_PowerLow.m_ChangePointPosition(m_PowerLowValue);
			m_FormPitch = m_BlockPointer[counter]->m_Pitch;
			// Get the Score
				// Attention:
				// Function m_GetScore() only will be called when the block is destroyed
			m_ReturnValue = m_BlockPointer[counter]->m_GetScore();
			// Delete Block
			delete m_BlockPointer[counter];
			m_LearningAndCreateModelKeyDownFlag = FALSE;
			m_LearningAndCreateModelTouchLineFlag = FALSE;
			m_CreateModelPitchToNumberLock = FALSE;
			// Move Pointer in order to replace the Empty Pointer Caursed by Delete
			if (counter != m_CurrentBlockNumber)
			for (int buf = counter; buf < m_CurrentBlockNumber-1; buf++)
				m_BlockPointer[buf] = m_BlockPointer[buf + 1];
			//
			m_CurrentBlockNumber--;
		}
	}
	return m_ReturnValue;
}

void CBlockControl::m_ChangeOtherObjectPosition(CStave &m_Stave,CRuler &m_Ruler)
{
	for (int counter = 0; counter < m_CurrentBlockNumber;counter++)
	{
		if (m_BlockPointer[counter]->m_IsStavePointMove())
		{
			m_Stave.m_PointMove(m_BlockPointer[counter]->m_Pitch);
			m_Ruler.m_ChangeArrowPosition(m_BlockPointer[counter]->m_Pitch);
		}
	}
}

bool CBlockControl::m_KeyDown(int m_InputPitch)
{
	bool ReturnValue = FALSE;
	for (int counter = 0; counter < m_CurrentBlockNumber; counter++)
	{
		if (m_InputPitch == m_BlockPointer[counter]->m_Pitch)
		{
			int buf;
			buf = m_BlockPointer[counter]->m_ChangePicture(TRUE, m_BlockUnitLength);
			if (buf != 2)m_LearningAndCreateModelKeyDownFlag = buf;
			if (buf == TRUE)ReturnValue = TRUE;
		}
	}
	return ReturnValue;
}

void CBlockControl::m_KeyUp(int m_InputPitch)
{
	for (int counter = 0; counter < m_CurrentBlockNumber; counter++)
	{
		if (m_InputPitch == m_BlockPointer[counter]->m_Pitch)
		{
			int buf;
			// this value is used to calculate the score
			buf = m_BlockPointer[counter]->m_ChangePicture(FALSE, m_BlockUnitLength);
			if (buf != 2)m_LearningAndCreateModelKeyDownFlag = buf;
		}
	}
}


void CBlockControl::m_Print(CDC& m_cacheDC)
{
	for (int counter = 0; counter < m_CurrentBlockNumber; counter++)
	{
		m_BlockPointer[counter]->m_Print(m_cacheDC);
	}
}

void CBlockControl::m_Reset(int m_Input)
{
	m_Model = m_Input;
	m_PitchFilecounter = 0;
	// Set m_Pitch[0] = 1 is to avoid the problem of 
	// at the beginning of game,the condition allow 
	// the Rank dialog to popup
	m_Pitch[m_PitchFilecounter] = 1;
	// Set m_Pitch[1] = 0 is base on  m_Pitch[0] = 1
	// due to  m_Pitch[0] is used,we use m_Pitch[1] =0
	// to avoid the music array isn't emptied when you 
	// enter the same game Mode in the second time
	m_Pitch[1] = 0;
	
	m_TimesFilecounter = 0;
	for (int counter = 0; counter < m_CurrentBlockNumber; counter++)
	{
		delete m_BlockPointer[counter];
	}
	m_CurrentBlockNumber = 0;
	// Used to Control Time Interval of neighbouring Blocks
	m_Timescounter = 0;
	m_FormPitch = 0;
	m_LearningAndCreateModelKeyDownFlag = FALSE;
	m_LearningAndCreateModelTouchLineFlag = FALSE;
	m_CreateModelPitchToNumberLock = TRUE;
	m_IsMusicEndLock = TRUE;
	m_CreateModelPitchToNumbercounter = 7;
	m_GetText();
}

void CBlockControl::m_ChangeCreateModelOption(int m_Input)
{
	m_CreateModelOption = m_Input;
}

int CBlockControl::m_CreateModelPitchToNumber()
{
	// 10 means error
	// 11 means coming to the end of Music
	if (m_CreateModelPitchToNumberLock)return 10;
	if (m_CreateModelPitchToNumbercounter >= MUSIC_LENGTH)return 11;
	switch (m_Pitch[m_CreateModelPitchToNumbercounter++])
	{
	case 68 :m_CreateModelPitchToNumberLock= TRUE; return 0;
	case 69 :m_CreateModelPitchToNumberLock= TRUE; return 1 ;
	case 71 :m_CreateModelPitchToNumberLock= TRUE; return 2 ;
	case 72 :m_CreateModelPitchToNumberLock= TRUE; return 3 ;
	case 74 :m_CreateModelPitchToNumberLock= TRUE; return 4 ;
	case 76 :m_CreateModelPitchToNumberLock= TRUE; return 5 ;
	case 77 :m_CreateModelPitchToNumberLock= TRUE; return 6 ;
	case 80 :m_CreateModelPitchToNumberLock= TRUE; return 7 ;
	case 81 :m_CreateModelPitchToNumberLock= TRUE; return 8 ;
	case 83 :m_CreateModelPitchToNumberLock= TRUE; return 9 ;
	default: return 10;
		
	}
}

bool CBlockControl::m_IsMusicEnd()
{
	if (m_IsMusicEndLock == FALSE && m_Model == CHALLENGE_MODEL
		&& m_CurrentBlockNumber == 0 &&
		(m_PitchFilecounter >= MUSIC_LENGTH || m_Pitch[m_PitchFilecounter] == -1))
		// chech m_PitchFilecounter should be put in front of m_Pitch[m_PitchFilecounter]
		// in order to avoid array out of border (Using || shortcuit)
	{
		m_IsMusicEndLock = TRUE;
		return TRUE;
	}
	else
		return FALSE;

}