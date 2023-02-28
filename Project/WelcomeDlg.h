#pragma once


// CWelcomeDlg 对话框

class CWelcomeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWelcomeDlg)

public:
	CWelcomeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWelcomeDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WELCOME_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();


// My Code
#define WELCOMEDLG_BOTTOM_TOP 0.69//0.85
#define WELCOMEDLG_BOTTOM_BOTTOM 0.83//0.90
#define WELCOMEDLG_BOTTOM_LEFT 0.35
	//LEFT = 0.5 -WIDTH*2-2.5*INTERVAL
#define WELCOMEDLG_BOTTOM_WIDTH 0.31
#define WELCOMEDLG_BOTTOM_INTERVAL 0.02
#define WELCOMEDLG_CHALLENGE_MODE 1
#define WELCOMEDLG_LEARNING_MODE 2
#define WELCOMEDLG_CREATE_MODE 3

public:
	void m_MoveButtons();
	void m_SetBottomPosition();
	void m_SendMessage(int);
private:
	CImage m_FrontPage;
	CRect m_client;
	CDC m_cacheDC;
	CBitmap m_cacheCBitmap;
	CRect m_BottonPosition;
public:
	afx_msg void OnStnClickedLearningmode();
	afx_msg void OnStnClickedChallengemode();
	afx_msg void OnStnClickedCreatemode();
	afx_msg void OnStnClickedLearnmore();

	afx_msg void OnClickedPictureExit();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	
	
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
