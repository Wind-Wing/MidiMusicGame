#pragma once
#include "media_player.h"
#include "CWMPPlayer4.h" 
#include "afxwin.h"

//#define WM_MY_SELECT WM_USER+0x001
// LearnMoreDlg 对话框

class CLearnMoreDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLearnMoreDlg)

public:
	CLearnMoreDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLearnMoreDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LEARNMORE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	// My Code
public:
	CWMPPlayer4 m_MediaPlayer;
	virtual BOOL OnInitDialog();
private:
	CFont m_Font;
	CStatic m_LearnmoreIntroduction1;
	CStatic m_LearnmoreIntroduction2;


public:
	CRect m_client;
	CDC m_cacheDC;
	CImage m_learnMoreDlgBackground;

	void m_DrawLearnMoreDlgBackGround();

	afx_msg void OnPaint();

};
