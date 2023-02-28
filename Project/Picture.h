#pragma once
class CPicture
{
public:
	CPicture();
	~CPicture();
	virtual void m_SetPosition(CRect m_Position);
	void m_Print(CDC &m_cacheDC);
protected:
	CRect m_Position;
	CImage m_Picture;
};

