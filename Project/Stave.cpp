#include "stdafx.h"
#include "Stave.h"


CStave::CStave()
{
	m_Picture.Load("res\\stave.png");

	m_RisingToneSymbol.Load("res\\#.png");
	m_FailingToneSymbol.Load("res\\b.png");
	m_ModulationPicture.Load("res\\modulationpoint.png");

	for (int counter = 0; counter < 10; counter++)
	{
		CString buf;
		buf.Format("res\\%d.png", counter);
		m_NumberPictures[counter].Load(buf);
	}
}

CStave::~CStave()
{
}

void CStave::m_Initialize(CRect m_client)
{
	// Initialize items about Stave
	this->m_client = m_client;
	// Set Stave's Position ( also Area )
	m_Position.top = (int)(this->m_client.bottom)*STAVE_TOP;
	m_Position.bottom = (int)m_client.bottom*STAVE_BOTTOM;
	m_Position.left = 0;
	m_Position.right = (int)m_client.right * STAVE_RIGHT;
	// Set Point Area
	m_PointArea = m_Position;
	m_PointArea.top = (int)(m_Position.top + m_Position.Height()*POINT_TOP);
	m_PointArea.bottom = (int)(m_Position.top + m_Position.Height()*POINT_BOTTOM);
	// Initialize Pen's Thick
	m_PointStep = (int)(m_PointArea.Height() / (POINT_LINE_NUMBER - 1.0));
	m_LineThick = (int)(m_PointArea.Width()*LINE_THICK_RATE);
	// Initialize Point's Moving area
	m_CenterCHeight = (int)(m_PointArea.top + CENTER_C_HEIGHT_RITE*m_PointArea.Height());

	// Initialize items about Numbered Musical Notation
	// Initialize Number's Height
	m_NumberPosition.top = (int)(this->m_client.bottom)*NUMBER_TOP;
	m_NumberPosition.bottom = (int)(this->m_client.bottom)*NUMBER_BOTTOM;
	// Initialize Number's Width
	m_NumberWidth = (int)(m_Position.Width() * NUMBER_WIDTH);
	if (m_NumberWidth < 1) { m_NumberWidth = 1; }
	// Initialize ToneSymbol's Height
	m_ToneSymbolPosition = m_NumberPosition;
	// Initialize ToneSymbol's Width
	m_ToneSymbolWidth = m_NumberWidth;
	// Initialize ModulationPoint's Height
	m_TopModulationPosition.top = m_Position.bottom;
	m_TopModulationPosition.bottom = m_NumberPosition.top;
	m_BottonModulationPosition.top = m_NumberPosition.bottom;
	m_BottonModulationPosition.bottom = (this->m_client).bottom;

	m_Reset();
}

void CStave::m_Reset()
{
	for (int counter = 0; counter < POINT_NUMBER; counter++)
	{
		m_PointPosition[counter].x = (int)(m_PointArea.Width()*(0.15*counter + 0.25));
		m_PointPosition[counter].y = m_CenterCHeight;
		m_StavePitch[counter] = CENTER_C_PITCH;
	}
}

void CStave::m_LinePrint(CDC &m_cacheDC)
{
	m_Pen.CreatePen(PS_SOLID, m_LineThick, RGB(192, 255, 62));
	m_OldPen = m_cacheDC.SelectObject(&m_Pen);
	m_cacheDC.MoveTo((int)(m_Position.Width()*LINE_POSITION), m_Position.top);
	m_cacheDC.LineTo((int)(m_Position.Width()*LINE_POSITION), m_Position.bottom);
	m_Pen.DeleteObject();
}

void CStave::m_PointPrint(CDC &m_cacheDC)
{
	m_Pen.CreatePen(PS_SOLID, m_PointStep, RGB(124, 252, 0));
	m_cacheDC.SelectObject(&m_Pen);
	for (int counter = 0; counter < POINT_NUMBER; counter++)
	{
		m_cacheDC.MoveTo(m_PointPosition[counter].x, m_PointPosition[counter].y);
		m_cacheDC.LineTo(m_PointPosition[counter].x, m_PointPosition[counter].y);
	}
	m_Pen.DeleteObject();
}

void CStave::m_Print(CDC &m_cacheDC)
{
	m_Picture.Draw(m_cacheDC, m_Position);
	// m_LinePrint() must be first called among the function using Pen
	// because in this function,m_OldPen is Record
	m_LinePrint(m_cacheDC);
	m_PointPrint(m_cacheDC);
	// When all the function had use the Pen,recover the Old pen
	m_cacheDC.SelectObject(m_OldPen);

	// Print Numbered musical notation
	m_NumberPrint(m_cacheDC);
	m_ToneSymbolPrint(m_cacheDC);
	m_ModulationPointPrint(m_cacheDC);
}

void CStave::m_PointMove(int m_InputPitch)
{
	for (int counter = POINT_NUMBER - 1; counter > 0; counter--)
	{
		m_PointPosition[counter].y = m_PointPosition[counter - 1].y;
		m_StavePitch[counter] = m_StavePitch[counter - 1];
	}
	// Set the new income Pitch's Position
	int m_MusicalScale = (int)((m_InputPitch - CENTER_C_PITCH) / 12);
	/*CString Buf,buffer;
	Buf.Format("%d", m_MusicalScale);
	buffer.Format("%d", m_InputPitch);
	MessageBox( NULL,Buf, buffer,NULL);*/
	m_MusicalScale = (m_InputPitch > CENTER_C_PITCH) ? m_MusicalScale : (m_MusicalScale - 1);
	int m_MusicalHeight = (int)((number_map[m_InputPitch % 12] - 1)*m_PointStep / 2.0);
	m_PointPosition[0].y = m_CenterCHeight - m_MusicalScale*3.5*m_PointStep - m_MusicalHeight;
	// Record the Pitchs display in the stave
	m_StavePitch[0] = m_InputPitch;
}

// Print Numbered musical notation
void CStave::m_NumberPrint(CDC& m_cacheDC)
{
	for (int counter = 0; counter < POINT_NUMBER; counter++)
	{
		// Set Number Position
		m_NumberPosition.left = m_PointPosition[counter].x - m_NumberWidth;
		m_NumberPosition.right = m_PointPosition[counter].x + m_NumberWidth;
		// Draw Numbers
		m_NumberPictures[number_map[m_StavePitch[counter] % 12]].Draw(m_cacheDC, m_NumberPosition);
	}
}

void CStave::m_ToneSymbolPrint(CDC& m_cacheDC)
{
	for (int counter = 0; counter < POINT_NUMBER; counter++)
	{
		// Set ToneSymbol Position
		m_ToneSymbolPosition.left = m_PointPosition[counter].x - m_ToneSymbolWidth * 3;
		m_ToneSymbolPosition.right = m_PointPosition[counter].x - m_ToneSymbolWidth;

		//此部分为修改内容，调整了临时升降号在简谱对应音符的位置
		m_ToneSymbolPosition.top = m_NumberPosition.top;
		m_ToneSymbolPosition.bottom = m_NumberPosition.top + (m_NumberPosition.bottom - m_NumberPosition.top) / 3 * 2;

		if (tone_map[m_StavePitch[counter] % 12] == -1)//#
		{
			// Print ToneSymbols in the Numbered musical notation
			m_RisingToneSymbol.Draw(m_cacheDC, m_ToneSymbolPosition);
			// Print ToneSymbols in the stave
			//五线谱中临时记号与音符原点的距离
			m_RisingToneSymbol.Draw(m_cacheDC, m_ToneSymbolPosition.left + 15, (m_PointPosition[counter].y - m_PointStep / 2), m_PointStep, m_PointStep);
		}
		else if (tone_map[m_StavePitch[counter] % 12] == 1)//b
		{
			// Print ToneSymbols in the Numbered musical notation
			m_FailingToneSymbol.Draw(m_cacheDC, m_ToneSymbolPosition);
			// Print ToneSymbols in the stave
			//五线谱中临时记号与音符原点的距离
			m_FailingToneSymbol.Draw(m_cacheDC, m_ToneSymbolPosition.left + 15, (m_PointPosition[counter].y - m_PointStep / 2), m_PointStep, m_PointStep);
		}
	}
}

// UNFINISHED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void CStave::m_Octaves(CDC& m_cacheDC)
{
	// Used to mark whether the former pitch is a HighPitch
	bool m_DottedLineMark = FALSE;
	// PS_DASH draw a Dotted line
	// PS_DAHS will Only take effect when PenThick <= 1
	m_Pen.CreatePen(PS_DASH, 1, RGB(192, 255, 62));
	m_OldPen = m_cacheDC.SelectObject(&m_Pen);
	for (int counter = 0; counter < POINT_NUMBER; counter++)
	{

	}
}
// UNFINISHED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

// Unfinished!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void CStave::m_ModulationPointPrint(CDC& m_cacheDC)
{
	int buf;
	for (int counter = 0; counter < POINT_NUMBER; counter++)
	{
		buf = (m_StavePitch[counter] - CENTER_C_PITCH) / 12;
		if (buf > 0)
		{
		}
		else if (buf < 0)
		{
			buf = abs(buf) + 1;

		}
	}
}
// Unfinished!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
