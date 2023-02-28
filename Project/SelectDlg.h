#pragma once
#include "afxwin.h"
#include"windows.h"
#include "shellapi.h"
#include "Background.h"
#define FOLDER_LEVEL 0
#define FILE_LEVEL 1
#define ALL_MUSIC 0
#define ANIME_MUSIC 1
#define CLASSICAL_MUSIC 2
#define POP_MUSIC 3
#define SHORT_MUSIC 4
#define UNCLASSIFIED_MUSIC 5
// CSelectDlg 对话框

class CSelectDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectDlg)

private:
	CImage m_SelectBackground;
	CRect m_client;
	CRect m_SelectListPosition;
	CDC m_cacheDC;
	CBitmap m_cacheCBitmap;
	// Used for show whether is in or outside the folder
	int m_DocumentLevel;
	CString m_CurrentFolder;
	int m_Classify;
public:
	CSelectDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSelectDlg();
	void m_Initialize(CString, int );

	
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SELECT_DIALOG };
#endif

	void m_DrawSelectDlgBackGround();
	void m_MoveSelectDlg();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_SelectList;
	virtual BOOL OnInitDialog();
	void m_SendMessage(bool);

// My Code
public:
	void m_GetFile(int);
	void m_CallExecute();
	int m_GetMidiNumber(CString);
	void m_SetClassify(int);
	void m_SetPressedButton(int);
	void m_ShowLevel();
protected:
//	afx_msg LRESULT OnMySelect(WPARAM wParam, LPARAM lParam);
public:

	//设置搜索类型点击高亮显示
	int choose_classfy_before;//上一次点击的按钮
	int choose_classfy_current;//当前点击按钮
	void Show_ClassfyClicked(int kind);

	afx_msg void OnDblclkSelectList();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnStnClickedOk();
	afx_msg void OnStnClickedCancel();
	afx_msg void OnLbnSelchangeSelectList();
	afx_msg void OnStnClickedClassify0();
	afx_msg void OnStnClickedClassify1();
	afx_msg void OnStnClickedClassify5();
	afx_msg void OnStnClickedClassify4();
	afx_msg void OnStnClickedClassify2();
	afx_msg void OnStnClickedClassify3();
};