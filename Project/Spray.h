#pragma once
#define SPRAY_NUMBER 4
#define SPRAY_DELAY 5
#define SPRAY_TOP 0.44//0.47
#define KEY_BOARD_TOP 0.47//0.5
class CSpray
{
public:
	CSpray();
	~CSpray();
	void m_Initialize(CRect);
	void m_Display(CDC &);
	void m_SetPosition(CRect&);
	bool m_CanShow() { return m_isShow; }
	void m_ChangeState(bool m_Input) { m_isShow = m_Input; }
private:
	CImage m_SprayPicture[SPRAY_NUMBER];
	CRect m_SprayPosition;
	int m_Spraycounter = 0;
	bool m_isShow;
};

