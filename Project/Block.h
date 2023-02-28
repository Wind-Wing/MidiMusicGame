#pragma once
#include "Picture.h"
class CBlock//:public CPicture
{
#define KEY_BOARD_BOTTOM 0.67//0.7
#define KEY_BOARD_TOP 0.47//0.5
	// The range that Players allowed to have mistake
#define ALLOWED_ERROR 0
	// Three Models
#define CHALLENGE_MODEL 1
#define LEARNING_MODEL 2
#define CREATE_MODEL 3
public:
	CBlock(int,CRect);
	~CBlock();
	bool m_ChangePosition(int);
	int  m_ChangePicture(bool,int);
	bool m_IsDestroy();
	bool m_IsStavePointMove();
// Function used for scores calculating
	int m_GetDistance();
	// m_GetScore should be called when the block is destroy 
	// to adjust to the situation that a block is button down 
	// twice
	int m_GetScore();
	void m_SetPosition(CRect m_Input) { m_Position = m_Input; }
	void m_Print(CDC&);
	int m_Pitch;

private:
	CRect m_client;
	// Mark whether should be Destroyed
	bool m_DestroyMark;
	// Avoid Stave Point Move too fast
	bool m_StavePointLock;
	CRect m_Position;
	CImage m_Picture[2];
	bool m_ScoreLock;		// Used to avoid the situation when the block is destroy
							// while the player has not let the button go
	int m_ButtonDownDistance;
	int m_Score;
	int m_CurrentPicture;

};

