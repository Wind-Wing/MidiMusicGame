#pragma once
#include"Block.h"
#include"Track.h"
#include "PianoKeyboard.h"
#include "PowerLow.h"
#include "Stave.h"
#include "Ruler.h"
//#include "math.h"

class CBlockControl
{
#define KEY_NUMBER 88//61
#define MUSIC_LENGTH 1000
	// There is also a dropping speed in CProjectDlg
	// BLOCK_DROPPING_STEP设置的是块下落的步长
#define BLOCK_DROPPING_STEP 1 //之前的设置值为10
	// They are also used in PianoKeyBoard.h and ProjectDlg.h
#define LOW_PITCH 21//24
#define HIGH_PITCH 108//84
	// Three Models
#define CHALLENGE_MODEL 1
#define LEARNING_MODEL 2
#define CREATE_MODEL 3
	// Four Options of Create Model
#define CREATE_MODEL_FREEPLAY 0
#define CREATE_MODEL_PIE 1
#define	CREATE_MODEL_E 2
#define	CREATE_MODEL_SQUARE2 3

public:
	CBlockControl();
	~CBlockControl();
	int m_Initialize(CRect,CPianoKeyboard &);
	void m_BlockCreate();
	int m_BlockDestroy(CPowerLow &);
	void m_BlockDropping();
	bool m_KeyDown(int);
	void m_KeyUp(int);
	int m_GetText();
	void m_Print(CDC&);
	void m_ChangeOtherObjectPosition(CStave &,CRuler&);
	void m_Reset(int);
	void m_ChangeCreateModelOption(int);
	int m_CreateModelPitchToNumber();
	bool m_IsMusicEnd();
	
private:
	CBlock *m_BlockPointer[KEY_NUMBER*5];
	CRect m_client;
	int m_CurrentBlockNumber;
	int m_PitchFilecounter;	// The Index of array m_Pitch
	int m_TimesFilecounter;	// The Index of array m_Times
	int m_Timescounter;		// Used to control time interval according to array m_Times
	CRect m_Position;		// Used to hold the data that is used to set the position of blocks
	int m_BlockUnitLength;	// Used to record the Unit Length of Blocks
	
	int m_Pitch[MUSIC_LENGTH];
	int m_Times[MUSIC_LENGTH];
	// m_StartTime is used to narrow error
	float m_StartTime[MUSIC_LENGTH];
	CRect m_KeyPosition[KEY_NUMBER];

	int m_FormPitch;// Used to calculate the value of PowerLow Statistics
	int m_PowerLowValue;
	int m_Model;
	// This two values is used to control the LearningModel 
	// BlockDropping stop when the block reach the piano line
	// with nobody to press the corresponding key
	bool m_LearningAndCreateModelKeyDownFlag;
	bool m_LearningAndCreateModelTouchLineFlag;
	// This value is used to record the option of Create Model
	int m_CreateModelOption;
	// This value is used to check whether we have new block to drop
	bool m_CreateModelPitchToNumberLock;
	// This Value is used to record the Show Number of Create Model Index
	int m_CreateModelPitchToNumbercounter;
	// This Value is used to check has show the rankdlg
	bool m_IsMusicEndLock;
};

