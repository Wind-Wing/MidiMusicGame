#pragma once


// RankDlg 对话框

class CRankDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRankDlg)

public:
	CRankDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRankDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RANK_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnRankMsg(WPARAM wParam, LPARAM lParam);
public:

	CRect m_client;
	CDC m_cacheDC;
	CBitmap m_cacheCBitmap;
	CImage m_RankDlgBackground;

	afx_msg void OnPaint();
	afx_msg void OnClose();
	afx_msg void OnStnClickedOk();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	// This value describe the max score and rank records
	// Also Define in ProjectDlg
	#define RECORD_NUMBER 100000

	void m_DrawRankDlgBackGround();
	void m_MoveRankDlg();
	void ReadAndSetRank();
};
