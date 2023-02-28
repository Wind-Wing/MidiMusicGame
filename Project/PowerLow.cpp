#include "stdafx.h"
#include "PowerLow.h"


CPowerLow::CPowerLow()
{
	m_Picture.Load("res\\coordinate.png");
}

CPowerLow::~CPowerLow()
{

}

void CPowerLow::m_Initialize(CRect m_client)
{
	this->m_client = m_client;
	m_Position.top = ((this->m_client.bottom)*POWERLOW_TOP);
	m_Position.bottom = this->m_client.bottom;
	m_Position.left = ((this->m_client.right)*POWERLOW_LEFT);
	m_Position.right = ((this->m_client.right)*POWERLOW_RIGHT);
	m_BackGroundPosition = m_Position;
	m_BackGroundPosition.left = 0;

	m_PenThick = (int)(m_Position.Width()*COORDINATE_PEN_THICK_RATE);

	m_PointStep = (double)1.00;
	// if (!m_PointStep)m_PointStep = (double)1.0;// Avoid Zero

	// Initialize Coordinate Position
	m_OriginPointX = (int)(m_Position.Width()*0.2 + m_Position.left);
	m_OriginPointY = (int)(m_Position.Height()*0.8 + m_Position.top);
	m_YAsixX = (int)(m_Position.Width()*0.2 + m_Position.left);
	m_YAsixY = (int)(m_Position.Height()*0.2 + m_Position.top);
	m_XAsixX = (int)(m_Position.Width()*0.8 + m_Position.left);
	m_XAsixY = (int)(m_Position.Height()*0.8 + m_Position.top);
	m_KeyDownLock = TRUE;

	m_Reset(CHALLENGE_MODEL);
}

void CPowerLow::m_Reset(int m_Input)
{
	// Set Model
	m_Model = m_Input;
	// Reset Points
	for (int counter = 0; counter < MAX_PITCH_DIFFERENCE; counter++)
	{
		m_PointX[counter] = (double)((counter + 1) *1000.0f / (MAX_PITCH_DIFFERENCE));
		//(m_Position.Width()*(0.225 + 0.025*counter) + m_Position.left);
		m_PointY[counter][BLOCK_POINT] = (double)1.00f;
		m_PointY[counter][PRESSED_POINT] = (double) 1.00f;
	}
	// Reset Fitting Line
	m_Slope[BLOCK_POINT] = 0.0f;
	m_Intercept[BLOCK_POINT] = 0.0f;
	m_Slope[PRESSED_POINT] = 0.0f;
	m_Intercept[PRESSED_POINT] = 0.0f;
	// Reset FomerKey
	m_FormerKeyDown = 0;
}

void CPowerLow::m_DrawCoordinate(CDC &m_cacheDC)
{
	// Create a Pen
	m_Pen.CreatePen(PS_SOLID, m_PenThick, RGB(255, 20, 147));
	m_OldPen = m_cacheDC.SelectObject(&m_Pen);

	//绿色的RGB：RGB(0, 238, 0)
	//红色的RGB：RGB(255, 20, 147)

	//添加线条说明，红线为弹奏拟合，绿线为乐曲拟合
	//m_Pen.CreatePen(PS_SOLID, m_PenThick, RGB(255, 20, 147);
	//m_cacheDC.LineTo(m_YAsixX, m_YAsixY);
	int red_x1 = (int)(m_Position.Width()*0.30 + m_Position.left);
	int red_x2 = (int)(m_Position.Width()*0.40 + m_Position.left);
	int y = (int)(m_Position.Height()*0.15 + m_Position.top);
	m_cacheDC.MoveTo(red_x1, y);
	m_cacheDC.LineTo(red_x2, y);

	//设置字体颜色和文本框透明
	SetBkMode(m_cacheDC, TRANSPARENT);
	SetTextColor(m_cacheDC, RGB(105, 105, 105));

	//SetBkColor(m_cacheDC, RGB(105, 105, 105));//深灰色
	m_cacheDC.TextOutA(red_x2 + 10, y - 10, "弹奏拟合");

	m_Pen.DeleteObject();
	m_Pen.CreatePen(PS_SOLID, m_PenThick, RGB(0, 238, 0));
	m_OldPen = m_cacheDC.SelectObject(&m_Pen);
	int green_x1 = (int)(m_Position.Width()*0.55 + m_Position.left);
	int green_x2 = (int)(m_Position.Width()*0.65 + m_Position.left);
	m_cacheDC.MoveTo(green_x1, y);
	m_cacheDC.LineTo(green_x2, y);
	m_cacheDC.TextOutA(green_x2 + 10, y - 10, "乐曲拟合");

	// Draw Coordinate
		// Draw Y Coordinate
	m_cacheDC.MoveTo(m_OriginPointX, m_OriginPointY);
	m_cacheDC.LineTo(m_YAsixX, m_YAsixY);

	//此部分为新修改添加内容,修改时间：2016年6月11日
	//设置Y轴注释
	m_cacheDC.TextOutA(m_YAsixX - 15, m_YAsixY - 30, "LogY");

	m_cacheDC.TextOutA(m_YAsixX - 30, m_YAsixY , "频");
	m_cacheDC.TextOutA(m_YAsixX - 30, m_YAsixY + 20, "度");
	/*m_cacheDC.TextOutA(m_YAsixX - 30, m_YAsixY + 40, "轴");
	m_cacheDC.TextOutA(m_YAsixX - 30, m_YAsixY + 60, "轴");*/


	m_cacheDC.LineTo((int)(m_YAsixX - m_Position.Width()*0.01), (int)(m_YAsixY + m_Position.Width()*0.01));
	m_cacheDC.MoveTo(m_YAsixX, m_YAsixY);
	m_cacheDC.LineTo((int)(m_YAsixX + m_Position.Width()*0.01), (int)(m_YAsixY + m_Position.Width()*0.01));
	// Draw X Coordinate
	m_cacheDC.MoveTo(m_OriginPointX, m_OriginPointY);
	m_cacheDC.LineTo(m_XAsixX, m_XAsixY);

	//此部分为新修改添加内容，修改时间：2016年6月11日
	//m_cacheDC.SetTextColor(RGB(0, 238, 0));
	m_cacheDC.TextOutA(m_XAsixX + 15, m_XAsixY - 10, "LogX");

	m_cacheDC.TextOutA(m_XAsixX -70, m_XAsixY+10, "音");
	m_cacheDC.TextOutA(m_XAsixX - 50, m_XAsixY + 10, "值");
	m_cacheDC.TextOutA(m_XAsixX - 30, m_XAsixY + 10, "差");
	//m_cacheDC.TextOutA(m_XAsixX-10, m_XAsixY + 10, "轴");

	m_cacheDC.LineTo((int)(m_XAsixX - m_Position.Width()*0.01), (int)(m_XAsixY - m_Position.Width()*0.01));
	m_cacheDC.MoveTo(m_XAsixX, m_XAsixY);
	m_cacheDC.LineTo((int)(m_XAsixX - m_Position.Width()*0.01), (int)(m_XAsixY + m_Position.Width()*0.01));




	// Delete the Pen
	m_cacheDC.SelectObject(m_OldPen);
	m_Pen.DeleteObject();
}

void CPowerLow::m_ChangePointPosition(int m_InputIndex)
{
	// Because the Powerlow is calculated from 3
	// the number lower than 3 is abandoned
	if (m_InputIndex >= 3 && m_InputIndex < MAX_PITCH_DIFFERENCE + 3)
		for (int counter = 0; counter <= m_InputIndex - 3; counter++)
		{
			m_PointY[counter][BLOCK_POINT] += m_PointStep;
		}

	m_FittingPoints(BLOCK_POINT);
}

void CPowerLow::m_FittingPoints(int m_Input)
{
	double m_XTotal = 0.0f;
	double m_YTotal = 0.0f;
	double m_XYTotal = 0.0f;
	double m_XXTotal = 0.0f;
	double m_XZero = (double)m_OriginPointX;
	double m_YZero = (double)m_OriginPointY;
	int m_Number = FITTING_POINT_NUMBER;
	double m_XBuffer = 0.0f;
	double m_YBuffer = 0.0f;
	for (int counter = 0; counter < FITTING_POINT_NUMBER; counter++)
	{
		m_XBuffer = (log10(m_PointX[counter]) - 1.5) * 2.0 / 3.0*m_Position.Width()*0.6;
		m_YBuffer = log10(m_PointY[counter][m_Input]) / 3.0*m_Position.Height()*0.6;
		m_XTotal += m_XBuffer;
		m_YTotal += m_YBuffer;
		m_XXTotal += m_XBuffer * m_XBuffer;
		m_XYTotal += m_XBuffer * m_YBuffer;
	}
	m_Slope[m_Input] = (m_Number*m_XYTotal - m_XTotal*m_YTotal) / (m_Number*m_XXTotal - m_XTotal*m_XTotal);
	m_Intercept[m_Input] = (m_YTotal - m_XTotal*m_Slope[m_Input]) / m_Number;
}

void CPowerLow::m_DrawPoint(CDC & m_cacheDC, int m_Input)
{
	// Create a Pen
	if (m_Input == BLOCK_POINT)
		m_Pen.CreatePen(PS_SOLID, m_PenThick, RGB(0, 238, 0));
	else
		m_Pen.CreatePen(PS_SOLID, m_PenThick, RGB(255, 255, 0));

	// Attach the Pen to the DC
	m_OldPen = m_cacheDC.SelectObject(&m_Pen);
	// Draw the Point
	int YBuffer;
	int XBuffer;
	for (int counter = 0; counter < MAX_PITCH_DIFFERENCE; counter++)
	{
		// -0.5 and *2 is used to move all points to a more suitable place 
		// according to the proportion
		XBuffer = (int)(m_OriginPointX + (log10(m_PointX[counter]) - 1.5) * 2 / 3.0*m_Position.Width()*0.6);
		YBuffer = (int)(m_OriginPointY - log10(m_PointY[counter][m_Input]) / 3.0*m_Position.Height()*0.6);
		if (YBuffer > m_Position.top)
		{
			m_cacheDC.MoveTo(XBuffer, YBuffer);
			m_cacheDC.LineTo(XBuffer, YBuffer);
		}
	}
	// Deattach the Pen from the DC
	m_cacheDC.SelectObject(m_OldPen);
	m_Pen.DeleteObject();

}

void CPowerLow::m_DrawFittingLine(CDC &m_cacheDC, int m_Input)
{
	int XBuffer;
	int YBuffer;

	// Create a Pen
	if (m_Input == BLOCK_POINT)
		m_Pen.CreatePen(PS_SOLID, (int)(m_PenThick / 2.0f), RGB(0, 238, 0));
	else
		m_Pen.CreatePen(PS_DOT, (int)(m_PenThick / 2.0f), RGB(255, 20, 147));
	// Attach the Pen to the DC
	m_OldPen = m_cacheDC.SelectObject(&m_Pen);

	// Draw the Line
	XBuffer = (log10(m_PointX[0]) - 1.5) * 2 / 3.0*m_Position.Width()*0.6;
	YBuffer = (m_OriginPointY - m_Slope[m_Input] * XBuffer - m_Intercept[m_Input]);
	XBuffer = (int)(m_OriginPointX + (log10(m_PointX[0]) - 1.5) * 2 / 3.0*m_Position.Width()*0.6);
	m_cacheDC.MoveTo(XBuffer, YBuffer);
	XBuffer = (log10(m_PointX[FITTING_POINT_NUMBER]) - 1.5) * 2 / 3.0*m_Position.Width()*0.6;
	YBuffer = (m_OriginPointY - m_Slope[m_Input] * XBuffer - m_Intercept[m_Input]);
	XBuffer = (int)(m_OriginPointX + (log10(m_PointX[FITTING_POINT_NUMBER]) - 1.5) * 2 / 3.0*m_Position.Width()*0.6);
	m_cacheDC.LineTo(XBuffer, YBuffer);

	// Deattach the Pen from the DC
	m_cacheDC.SelectObject(m_OldPen);
	m_Pen.DeleteObject();
}
void CPowerLow::m_Print(CDC &m_cacheDC)
{
	m_Picture.Draw(m_cacheDC, m_BackGroundPosition);
	m_DrawPoint(m_cacheDC, BLOCK_POINT);
	m_DrawFittingLine(m_cacheDC, BLOCK_POINT);
	//if (m_Model != CREATE_MODEL)
	{
		m_DrawPoint(m_cacheDC, PRESSED_POINT);
		m_DrawFittingLine(m_cacheDC, PRESSED_POINT);
	}
	m_DrawCoordinate(m_cacheDC);
}

void CPowerLow::m_KeyDown(int m_Input)
{
	int m_InputIndex = abs(m_Input - m_FormerKeyDown);
	if (m_KeyDownLock&&m_InputIndex >= 3 && m_InputIndex < MAX_PITCH_DIFFERENCE + 3)
		for (int counter = 0; counter <= m_InputIndex - 3; counter++)
		{
			m_PointY[counter][PRESSED_POINT] += m_PointStep;
		}
	m_FormerKeyDown = m_Input;
	m_KeyDownLock = FALSE;
	m_FittingPoints(PRESSED_POINT);
}

void CPowerLow::m_KeyUp(int m_Input)
{
	m_KeyDownLock = TRUE;
}