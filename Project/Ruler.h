#pragma once
class CRuler
{
#define RULER_SPACE_TOP 0.7
#define RULER_SPACE_LEFT 0.4
#define RULER_SPACE_RIGHT 0.65//0.6
#define MIDI_KEYNUMBER 88.0
#define LOW_PITCH 21//24
public:
	CRuler();
	~CRuler();

	void m_Initialize(CRect);
	void m_Print(CDC &);
	void m_ChangeArrowPosition(int);
	void m_ChangeNumberPosition();
	void m_NumberPrint(CDC&);
private:
	int m_ArrowHeight;
	int m_NumberHeight;
	int m_NumberWidth;
	int m_FormerPitch;
	int m_PitchInterval;
	float m_PitchStep;
	CImage m_Ruler;
	CImage m_Arrow1;
	CImage m_Arrow2;
	CImage m_NumberPictures[10];
	CImage m_RulerText;
	CRect m_RulerTextPosition;
	CRect m_RulerSpacePosition;
	CRect m_RulerPosition;
	CRect m_ArrowPosition1;
	CRect m_ArrowPosition2;
	CRect m_NumberPosition;
	CRect m_client;
};

