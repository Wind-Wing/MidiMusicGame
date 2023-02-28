#pragma once
#include "Picture.h"
class CStave:public CPicture
{
// Define the stave area
#define STAVE_TOP 0.7
#define STAVE_RIGHT 0.4
#define STAVE_BOTTOM 0.92
// Define Parameters about Points in stave
	// There are 12 unit distance in Numbered Musical Notation
#define POINT_LINE_NUMBER 11.0
	// Point top and bottom is relative to the area of stave
#define POINT_TOP (41.0/280.0)
#define POINT_BOTTOM (239.0/280.0)
#define POINT_NUMBER 5
#define CENTER_C_HEIGHT_RITE 0.50
#define CENTER_C_PITCH 60
// Define Parameters about Line in stave
#define LINE_POSITION 0.55
#define LINE_THICK_RATE 0.01
// Define the Numbered Musical Notation
#define NUMBER_TOP 0.94
#define NUMBER_BOTTOM 0.98
#define NUMBER_WIDTH 0.02
#define CHALLENGE_MODEL 1
#define LEARNING_MODEL 2
#define CREATE_MODEL 3

struct Point
{
	int x;
	int y;
};

public:
	CStave();
	~CStave();
	void m_Initialize(CRect);
	void m_Print(CDC&);
	void m_LinePrint(CDC &);
	void m_PointPrint(CDC &);
	void m_NumberPrint(CDC &);
	void m_PointMove(int);
	void m_Octaves(CDC&);
	void m_ModulationPointPrint(CDC&);
	void m_ToneSymbolPrint(CDC&);
	void m_Reset();
private:
	CRect m_client;
	int m_Model;
	// an  Area that Point allowed to exist
	CRect m_PointArea;
	CPen m_Pen;
	CPen *m_OldPen;
	// m_PointStep record the distance between to lines in the stave
	int m_PointStep;
	int m_LineThick;
	Point m_PointPosition[POINT_NUMBER];
	int m_CenterCHeight;

	//Numbered musical notation
	int m_StavePitch[POINT_NUMBER];
	int m_NumberWidth;
	int m_ToneSymbolWidth;
	CImage m_NumberPictures[10];
	CImage m_ModulationPicture;
	CImage m_RisingToneSymbol;
	CImage m_FailingToneSymbol;
	CRect m_NumberPosition;
	CRect m_ToneSymbolPosition;
	// Those CRect Objects are used to record the top and bottom Position of Numbered musical notations
	CRect m_TopModulationPosition;
	CRect m_BottonModulationPosition;
	// Used to record the Modulation Mark used by black keys
		// 0 means a white key
		// -1 means a black key with #
		// 1 means a black key with b
	const int tone_map[12]   = {0,-1,0,1,0,0,-1,0,1,0,1,0};
	const int number_map[12] = {1,1,2,3,3,4,4,5,6,6,7,7 };
};

