#pragma once
#include "Picture.h"

class CPowerLow :public CPicture
{
#define POWERLOW_TOP 0.7	
#define POWERLOW_LEFT 0.6
#define POWERLOW_RIGHT 1
#define COORDINATE_PEN_THICK_RATE 0.01
// Attention:This Const Do NOT cover all the values,something else needed to be changed
//			in the call of this function
#define MAX_PITCH_DIFFERENCE 22
#define FITTING_POINT_NUMBER 8
#define BLOCK_POINT 0
#define PRESSED_POINT 1

#define CHALLENGE_MODEL 1
#define LEARNING_MODEL 2
#define CREATE_MODEL 3

public:
	CPowerLow();
	~CPowerLow();
	void m_Initialize(CRect);
	void m_DrawCoordinate(CDC &);
	void m_Print(CDC &);
	void m_ChangePointPosition(int);
	void m_DrawPoint(CDC &,int);
	void m_FittingPoints(int);
	void m_DrawFittingLine(CDC&,int);
	void m_Reset(int);
	void m_KeyDown(int);
	void m_KeyUp(int);

private:
	CRect m_BackGroundPosition;
	CRect m_client;
	CPen m_Pen;
	CPen *m_OldPen;
	int m_PenThick;
	double m_PointX[MAX_PITCH_DIFFERENCE];
	double m_PointY[MAX_PITCH_DIFFERENCE][2];
	double m_PointStep;
	// Coordinate Position
	int m_OriginPointX;
	int m_OriginPointY;
	int m_YAsixX;
	int m_YAsixY;
	int m_XAsixX;
	int m_XAsixY;
	bool m_KeyDownLock;
	int m_FormerKeyDown;
	int m_Model;
	// Prepared for fitting
	double m_Slope[2];
	double m_Intercept[2];
};

