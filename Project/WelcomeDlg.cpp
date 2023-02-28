// WelcomeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Project.h"
#include "WelcomeDlg.h"
#include "afxdialogex.h"
#include "ProjectDlg.h"



// CWelcomeDlg 对话框

IMPLEMENT_DYNAMIC(CWelcomeDlg, CDialogEx)

CWelcomeDlg::CWelcomeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_WELCOME_DIALOG, pParent)
{
	m_FrontPage.Load("res\\frontpage.png");	
}

CWelcomeDlg::~CWelcomeDlg()
{

}

void CWelcomeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWelcomeDlg, CDialogEx)

	ON_STN_CLICKED(IDC_CHALLENGEMODE, &CWelcomeDlg::OnStnClickedChallengemode)
	ON_STN_CLICKED(IDC_LEARNINGMODE, &CWelcomeDlg::OnStnClickedLearningmode)
	ON_STN_CLICKED(IDC_CREATEMODE, &CWelcomeDlg::OnStnClickedCreatemode)

	ON_STN_CLICKED(IDC_PICTURE_EXIT, &CWelcomeDlg::OnClickedPictureExit)
	ON_BN_CLICKED(IDCANCEL, &CWelcomeDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CWelcomeDlg::OnBnClickedOk)

	ON_WM_CTLCOLOR()


	ON_STN_CLICKED(IDC_LEARNMORE, &CWelcomeDlg::OnStnClickedLearnmore)
END_MESSAGE_MAP()


// CWelcomeDlg 消息处理程序


BOOL CWelcomeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	ShowWindow(SW_MAXIMIZE);
	// TODO:  在此添加额外的初始化

// My Code
	// Get Clinet Size
	GetClientRect(&m_client);
	// Initilize Botton Position
	m_BottonPosition.top = (int)(m_client.bottom * WELCOMEDLG_BOTTOM_TOP);
	m_BottonPosition.bottom = (int)(m_client.bottom * WELCOMEDLG_BOTTOM_BOTTOM);
	m_BottonPosition.right = (int)(m_client.right*WELCOMEDLG_BOTTOM_LEFT);

	// Move Bottons
	m_MoveButtons();

	// Create cacheDC
	CDC *cDC = this->GetDC();
	m_cacheDC.CreateCompatibleDC(NULL);
	m_cacheCBitmap.CreateCompatibleBitmap(cDC, m_client.Width(), m_client.Height());
	m_cacheDC.SelectObject(&m_cacheCBitmap);

	// Draw FrontPage
	m_FrontPage.Draw(m_cacheDC, m_client);

	// Print on Screen
	cDC->BitBlt(0, 0, m_client.Width(), m_client.Height(), &m_cacheDC, 0, 0, SRCCOPY);

	// Release 
	ValidateRect(&m_client);

	//Invalidate(FALSE);
	m_cacheDC.DeleteDC();
	m_cacheCBitmap.DeleteObject();
	ReleaseDC(cDC);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CWelcomeDlg::m_MoveButtons()
{
	m_BottonPosition.top = m_client.bottom * 0.15;
	m_BottonPosition.bottom = m_client.bottom * 0.30;
	m_BottonPosition.left = m_client.right * 0.50;
	m_BottonPosition.right = m_client.right * 0.81;
	GetDlgItem(IDC_CHALLENGEMODE)->MoveWindow(m_BottonPosition);

	m_BottonPosition.top = m_client.bottom * 0.28;
	m_BottonPosition.bottom = m_client.bottom * 0.43;
	m_BottonPosition.left = m_client.right * 0.60;
	m_BottonPosition.right = m_client.right * 0.91;
	GetDlgItem(IDC_LEARNINGMODE)->MoveWindow(m_BottonPosition);

	m_BottonPosition.top = m_client.bottom * 0.41;
	m_BottonPosition.bottom = m_client.bottom * 0.56;
	m_BottonPosition.left = m_client.right * 0.70;
	m_BottonPosition.right = m_client.right * 0.99;
	GetDlgItem(IDC_CREATEMODE)->MoveWindow(m_BottonPosition);

	m_BottonPosition.top = m_client.bottom * 0.55;
	m_BottonPosition.bottom = m_client.bottom * 0.70;
	m_BottonPosition.left = m_client.right * 0.80;
	m_BottonPosition.right = m_client.right * 0.99;
	GetDlgItem(IDC_LEARNMORE)->MoveWindow(m_BottonPosition);

	/*m_BottonPosition.top = m_client.bottom * 0.1;
	m_BottonPosition.bottom = m_client.bottom * 0.245;
	m_BottonPosition.left = m_client.right * 0.05;
	m_BottonPosition.right = m_client.right * 0.36;
	GetDlgItem(IDC_CHALLENGEMODE)->MoveWindow(m_BottonPosition);

	m_BottonPosition.top = m_client.bottom * 0.1;
	m_BottonPosition.bottom = m_client.bottom * 0.245;
	m_BottonPosition.left = m_client.right * 0.64;
	m_BottonPosition.right = m_client.right * 0.95;
	GetDlgItem(IDC_LEARNINGMODE)->MoveWindow(m_BottonPosition);

	m_BottonPosition.top = m_client.bottom * 0.755;
	m_BottonPosition.bottom = m_client.bottom * 0.9;
	m_BottonPosition.left = m_client.right * 0.05;
	m_BottonPosition.right = m_client.right * 0.36;
	GetDlgItem(IDC_CREATEMODE)->MoveWindow(m_BottonPosition);

	m_BottonPosition.top = m_client.bottom * 0.755;
	m_BottonPosition.bottom = m_client.bottom * 0.9;
	m_BottonPosition.left = m_client.right * 0.64;
	m_BottonPosition.right = m_client.right * 0.95;
	GetDlgItem(IDC_LEARNMORE)->MoveWindow(m_BottonPosition);*/

}

void CWelcomeDlg::m_SetBottomPosition()
{
	m_BottonPosition.left = m_BottonPosition.right + (int)(m_client.right * WELCOMEDLG_BOTTOM_INTERVAL);
	m_BottonPosition.right = m_BottonPosition.left + (int)(m_client.right * WELCOMEDLG_BOTTOM_WIDTH);
}

void CWelcomeDlg::m_SendMessage(int m_Mode)
{
	HWND hWnd = this->GetParent()->GetSafeHwnd();
	// 向父窗口发送消息  
	if (hWnd == NULL) return (void)MessageBox(_T("获得父窗口句柄失败！"));
	::SendNotifyMessage(hWnd, WM_MODE, (WPARAM)&m_Mode, NULL);
}

void CWelcomeDlg::OnClickedPictureExit()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}

void CWelcomeDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}
// Button
void CWelcomeDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


HBRUSH CWelcomeDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH   hBrush = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	//设置得分字体
	//if (pWnd->GetDlgCtrlID() == IDC_CHALLENGEMODE) {
	pDC->SetBkMode(TRANSPARENT);
	//pDC->SetBkColor(RGB(0, 238, 0));
	//pDC->SetTextColor(RGB(34, 139, 34));
	//pDC->SetTextColor(RGB(255, 160, 122));
	pDC->SetTextColor(RGB(93, 137, 13));

	CFont *font = new CFont();
	font->CreatePointFont(480, "华文行楷", NULL);
	pDC->SelectObject(font);
	//return   (HBRUSH)::GetStockObject(NULL_BRUSH);
//}

	return   (HBRUSH)::GetStockObject(NULL_BRUSH);
	//return hBrush;

}

void CWelcomeDlg::OnStnClickedChallengemode()
{
	/*CProjectDlg dlg;
	INT_PTR m_DlgReturn;
	m_DlgReturn = dlg.DoModal();*/
	/*if (m_DlgReturn == IDCANCEL) { exit(0); }
	else dlg.DestroyWindow();*/

	// TODO: 在此添加控件通知处理程序代码
	m_SendMessage(WELCOMEDLG_CHALLENGE_MODE);
	CDialogEx::OnOK();
}

void CWelcomeDlg::OnStnClickedLearningmode()
{
	m_SendMessage(WELCOMEDLG_LEARNING_MODE);
	CDialogEx::OnOK();
}


void CWelcomeDlg::OnStnClickedCreatemode()
{
	m_SendMessage(WELCOMEDLG_CREATE_MODE);
	CDialogEx::OnOK();
}


void CWelcomeDlg::OnStnClickedLearnmore()
{
	CLearnMoreDlg m_LearnMoreDlg;
	INT_PTR m_LearnMoreDlgReturn;
	m_LearnMoreDlgReturn = m_LearnMoreDlg.DoModal();
	UpdateData(TRUE);
	m_LearnMoreDlg.DestroyWindow();
}


BOOL CWelcomeDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	// Press Alt + Control + M + Z + Y to end this program
	// This function is also used in WelcomeDlg
	// VK_MENU means Alt
	if (pMsg->message == WM_KEYDOWN && ::GetKeyState(VK_MENU) < 0
		&& ::GetAsyncKeyState(VK_CONTROL) && ::GetKeyState('M')<0 && ::GetKeyState('Z') < 0 && ::GetKeyState('Y')<0
		)
	{
		MessageBox("退出游戏");
		exit(0);
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
