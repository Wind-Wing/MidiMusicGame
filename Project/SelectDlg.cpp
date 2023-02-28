// SelectDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Project.h"
#include "SelectDlg.h"
#include "afxdialogex.h"


// CSelectDlg 对话框

IMPLEMENT_DYNAMIC(CSelectDlg, CDialogEx)

CSelectDlg::CSelectDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SELECT_DIALOG, pParent)
{
}

CSelectDlg::~CSelectDlg()
{
}

void CSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SELECT_LIST, m_SelectList);
}


BEGIN_MESSAGE_MAP(CSelectDlg, CDialogEx)
	//The ID of WM_MY_SELECT is Defined in the File Project.h
	ON_LBN_DBLCLK(IDC_SELECT_LIST, &CSelectDlg::OnDblclkSelectList)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_STN_CLICKED(IDOK, &CSelectDlg::OnStnClickedOk)
	ON_STN_CLICKED(IDCANCEL, &CSelectDlg::OnStnClickedCancel)
	ON_LBN_SELCHANGE(IDC_SELECT_LIST, &CSelectDlg::OnLbnSelchangeSelectList)
	ON_STN_CLICKED(IDC_CLASSIFY_0, &CSelectDlg::OnStnClickedClassify0)
	ON_STN_CLICKED(IDC_CLASSIFY_1, &CSelectDlg::OnStnClickedClassify1)
	ON_STN_CLICKED(IDC_CLASSIFY_5, &CSelectDlg::OnStnClickedClassify5)
	ON_STN_CLICKED(IDC_CLASSIFY_4, &CSelectDlg::OnStnClickedClassify4)
	ON_STN_CLICKED(IDC_CLASSIFY_2, &CSelectDlg::OnStnClickedClassify2)
	ON_STN_CLICKED(IDC_CLASSIFY_3, &CSelectDlg::OnStnClickedClassify3)
END_MESSAGE_MAP()


// CSelectDlg 消息处理程序


BOOL CSelectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//ShowWindow(SW_MAXIMIZE);
	// TODO:  在此添加额外的初始化

	// Get the client 
	GetClientRect(&m_client);
	// Load the background Picture
	m_SelectBackground.Load("res\\SelectDlgBackGround.png");
	// Set the SelectListPosition
	//m_SelectListPosition.top = (int)(m_client.bottom * 0.06);
	//m_SelectListPosition.bottom = (int)(m_client.bottom * 0.95);
	//m_SelectListPosition.left = (int)(m_client.right * 0.03);
	//m_SelectListPosition.right = (int)(m_client.right * 0.97);
	m_MoveSelectDlg();
	m_ShowLevel();

	// Read the name of mid files
	m_GetFile(m_DocumentLevel);
	m_SelectList.SetCurSel(0);
	SetFocus();
	return TRUE;
	// return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CSelectDlg::m_Initialize(CString m_InputPath, int m_InputLevel)
{
	m_DocumentLevel = m_InputLevel;
	m_CurrentFolder = m_InputPath;
}

void CSelectDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CDialogEx::OnPaint()
					   // Set the Initialize value
	m_DrawSelectDlgBackGround();
}


void CSelectDlg::m_DrawSelectDlgBackGround()
{
	CDC *cDC = this->GetDC();
	m_SelectBackground.Draw(cDC->m_hDC, m_client);
}

//afx_msg LRESULT CSelectDlg::OnMySelect(WPARAM wParam, LPARAM lParam)
//{
//	m_SelectList.AddString(*(CString*)wParam);
//	return 0;
//}

void CSelectDlg::m_GetFile(int FileLevel)
{
	int counter = 0;
	CFileFind m_finder;
	bool m_judge;

	if (FileLevel == FOLDER_LEVEL)
	{
		m_judge = m_finder.FindFile("MidiLib\\(*");
	}
	else if (FileLevel == FILE_LEVEL)
	{
		CString buf;
		buf = "MidiLib\\" + m_CurrentFolder;// +"\\*.mid";
		switch (m_Classify)
		{
		case ALL_MUSIC:			buf = buf + "\\*.mid"; break;
		case ANIME_MUSIC:		buf = buf + "\\[动漫乐曲]*.mid"; break;
		case CLASSICAL_MUSIC:	buf = buf + "\\[古典乐曲]*.mid"; break;
		case POP_MUSIC:			buf = buf + "\\[流行乐曲]*.mid"; break;
		case SHORT_MUSIC:		buf = buf + "\\[简短乐曲]*.mid"; break;
		case UNCLASSIFIED_MUSIC:buf = buf + "\\*.mid"; break;
		}
		m_judge = m_finder.FindFile(buf);
	}

	m_SelectList.ResetContent();

	while (m_judge)
	{
		// Attention:The order of this two sentences can not be changed,
		// for m_finder.FindFile(..) point to the blank 
		m_judge = m_finder.FindNextFile();
		// Add a long empty string is used to fix the problem of selectlist:
		// when the user change his select,the former select will leave a 
		// blue bar which is not so suitable with the background picture
		CString Buffer = m_finder.GetFileTitle();
		if (FileLevel == FOLDER_LEVEL)
		{
			CString buf = "MidiLib\\" + Buffer + "\\*.mid";
			Buffer.Format("%s[%d]", Buffer, m_GetMidiNumber(buf));
		}
		if (m_Classify != UNCLASSIFIED_MUSIC || Buffer.GetAt(0) != '[')
		{
			m_SelectList.AddString(Buffer + "                                                                                            ");
			// Change the Height of each line
			m_SelectList.SetItemHeight(counter, 33);
			counter++;
		}
	}
	m_finder.Close();

	CString Buffer;
	Buffer.Format("%d", counter);
	GetDlgItem(IDC_MIDI_NUMBER)->SetWindowTextA(" 含 "+Buffer+" 首 乐 曲 ");
	// Check Whether there are MIDI files
	//if (!counter)
	//MessageBox("No MIDI files avaliable,Please check the folder of \\MidiLib", "Error", MB_ICONERROR | MB_OK);
}

int CSelectDlg::m_GetMidiNumber(CString m_InputPath)
{
	int counter = 0;
	CFileFind m_finder;
	bool m_judge;
	// Traverse the Folder
	m_judge = m_finder.FindFile(m_InputPath);

	while (m_judge)
	{
		m_judge = m_finder.FindNextFile();
		counter++;
	}
	m_finder.Close();
	return counter;
}

void CSelectDlg::OnDblclkSelectList()
{
	OnStnClickedOk();
}

void CSelectDlg::m_SendMessage(bool m_Input)
{
	CString Buffer;
	Buffer.Format("%d", m_Input);

	//HWND hWnd = this->FindWindow("CProjectDlg", NULL)->GetSafeHwnd();
	// 向父窗口发送消息  
	//if (hWnd == NULL) return (void)MessageBox(_T("获得父窗口句柄失败！"));
	//::SendNotifyMessage(hWnd, WM_SELECT, (WPARAM)&m_Input, NULL);
	::SendNotifyMessage(::AfxGetMainWnd()->m_hWnd, WM_SELECT, (WPARAM)&m_Input, NULL);
}

void CSelectDlg::m_CallExecute()
{
	CString buf;
	int m_index = m_SelectList.GetCurSel();
	if (m_index >= 0)m_SelectList.GetText(m_index, buf);
	// Trim() 去除多余的空格
	buf.Trim();
	buf = " MidiLib\\" + m_CurrentFolder + "\\" + buf + ".mid exe\\MaxT.txt";
	//MessageBox(buf);
	SHELLEXECUTEINFO  ShExecInfo = { 0 };
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = _T("open");
	ShExecInfo.lpFile = _T("exe\\MIDI2TXT.exe");
	ShExecInfo.lpParameters = buf;
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_HIDE;
	ShExecInfo.hInstApp = NULL;
	ShellExecuteEx(&ShExecInfo);
	WaitForSingleObject(ShExecInfo.hProcess, INFINITE);

	ShExecInfo = { 0 };
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = _T("open");
	ShExecInfo.lpFile = _T("exe\\readmidi.exe");
	ShExecInfo.lpParameters = NULL;
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_HIDE;
	ShExecInfo.hInstApp = NULL;
	ShellExecuteEx(&ShExecInfo);
	WaitForSingleObject(ShExecInfo.hProcess, INFINITE);

}

void CSelectDlg::m_SetClassify(int m_Input)
{
	m_Classify = m_Input;
	m_SetPressedButton(m_Input);
}

void CSelectDlg::m_SetPressedButton(int m_Input)
{
	switch (m_Input)
	{
	case ALL_MUSIC: choose_classfy_before = choose_classfy_current = IDC_CLASSIFY_0; break;
	case ANIME_MUSIC: choose_classfy_before = choose_classfy_current = IDC_CLASSIFY_1; break;
	case CLASSICAL_MUSIC: choose_classfy_before = choose_classfy_current = IDC_CLASSIFY_2; break;
	case POP_MUSIC: choose_classfy_before = choose_classfy_current = IDC_CLASSIFY_3; break;
	case SHORT_MUSIC: choose_classfy_before = choose_classfy_current = IDC_CLASSIFY_4; break;
	case UNCLASSIFIED_MUSIC: choose_classfy_before = choose_classfy_current = IDC_CLASSIFY_5; break;
	}
}

void CSelectDlg::m_ShowLevel()
{
	if (m_DocumentLevel == FOLDER_LEVEL)
	{
		GetDlgItem(IDC_MIDI_NUMBER)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CLASSIFY_0)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CLASSIFY_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CLASSIFY_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CLASSIFY_3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CLASSIFY_4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CLASSIFY_5)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CLASSIFY_5)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SELECT_LIST)->ShowWindow(SW_SHOW);
	}
	else if (m_DocumentLevel == FILE_LEVEL)
	{
		GetDlgItem(IDC_MIDI_NUMBER)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CLASSIFY_0)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CLASSIFY_1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CLASSIFY_2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CLASSIFY_3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CLASSIFY_4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CLASSIFY_5)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CLASSIFY_5)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SELECT_LIST)->ShowWindow(SW_SHOW);
	}
}


void CSelectDlg::m_MoveSelectDlg()
{
	CRect position;
	if (m_DocumentLevel == FOLDER_LEVEL)
	{
		position.top = (int)(m_client.bottom*0.03);
		position.bottom = (int)(m_client.bottom * 0.88);
		position.left = (int)(m_client.right * 0.03);
		position.right = (int)(m_client.right * 0.97);
	}
	else if (m_DocumentLevel == FILE_LEVEL)
	{
		position.top = (int)(m_client.bottom*0.03);
		position.bottom = (int)(m_client.bottom * 0.88);
		position.left = (int)(m_client.right * 0.2);
		position.right = (int)(m_client.right * 0.97);
	}
	
	GetDlgItem(IDC_SELECT_LIST)->MoveWindow(position);
	
	


	position.left = (int)(m_client.right * 0.01);
	position.right = (int)(m_client.right * 0.20);

	position.top = (int)(m_client.bottom*0.03);
	position.bottom = (int)(m_client.bottom * 0.20);
	GetDlgItem(IDC_CLASSIFY_0)->MoveWindow(position);

	position.top = (int)(m_client.bottom*0.20);
	position.bottom = (int)(m_client.bottom * 0.37);
	GetDlgItem(IDC_CLASSIFY_1)->MoveWindow(position);

	position.top = (int)(m_client.bottom*0.37);
	position.bottom = (int)(m_client.bottom * 0.54);
	GetDlgItem(IDC_CLASSIFY_2)->MoveWindow(position);

	position.top = (int)(m_client.bottom*0.54);
	position.bottom = (int)(m_client.bottom * 0.71);
	GetDlgItem(IDC_CLASSIFY_3)->MoveWindow(position);

	position.top = (int)(m_client.bottom*0.71);
	position.bottom = (int)(m_client.bottom * 0.88);
	GetDlgItem(IDC_CLASSIFY_4)->MoveWindow(position);

	position.top = (int)(m_client.bottom*0.88);
	position.bottom = (int)(m_client.bottom * 1);
	GetDlgItem(IDC_CLASSIFY_5)->MoveWindow(position);


	position.top = (int)(m_client.bottom * 0.88);
	position.bottom = (int)(m_client.bottom);
	position.left = (int)(m_client.right * 0.86);
	position.right = (int)(m_client.right * 0.98);
	GetDlgItem(IDCANCEL)->MoveWindow(position);


	position.top = (int)(m_client.bottom * 0.88);
	position.bottom = (int)(m_client.bottom);
	position.left = (int)(m_client.right * 0.70);
	position.right = (int)(m_client.right * 0.82);
	GetDlgItem(IDOK)->MoveWindow(position);

	position.top = (int)(m_client.bottom * 0.92);
	position.bottom = (int)(m_client.bottom);
	position.left = (int)(m_client.right * 0.25);
	position.right = (int)(m_client.right * 0.50);
	GetDlgItem(IDC_MIDI_NUMBER)->MoveWindow(position);
}

HBRUSH CSelectDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if (pWnd->GetDlgCtrlID() == IDC_SELECT_LIST)
	{

		//pDC->SetBkMode(TRANSPARENT);
		pDC->SetBkColor(RGB(183, 229, 92));
		//pDC->SetTextColor(RGB(144, 238, 144));
		CFont *m_Font = new CFont();
		pDC->SetTextColor(RGB(0, 0, 0));

		m_Font->CreatePointFont(240, "楷体", NULL);
		pDC->SelectObject(m_Font);

		delete m_Font;

		return   (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	if ((pWnd->GetDlgCtrlID() == IDOK) || (pWnd->GetDlgCtrlID() == IDCANCEL))
	{

		pDC->SetBkMode(TRANSPARENT);
		//pDC->SetBkColor(RGB(144, 238, 144));
		//pDC->SetTextColor(RGB(144, 238, 144));
		CFont *m_Font = new CFont();
		pDC->SetTextColor(RGB(0, 0, 0));

		m_Font->CreatePointFont(240, "华文行楷", NULL);
		pDC->SelectObject(m_Font);

		delete m_Font;

		return   (HBRUSH)::GetStockObject(NULL_BRUSH);
	}

	//设置分类排序字体大小和颜色
	if ((pWnd->GetDlgCtrlID() == IDC_CLASSIFY_0) || (pWnd->GetDlgCtrlID() == IDC_CLASSIFY_1)
		|| (pWnd->GetDlgCtrlID() == IDC_CLASSIFY_2) || (pWnd->GetDlgCtrlID() == IDC_CLASSIFY_3)
		|| (pWnd->GetDlgCtrlID() == IDC_CLASSIFY_4) || (pWnd->GetDlgCtrlID() == IDC_CLASSIFY_5)) {

		pDC->SetBkMode(TRANSPARENT);
		CFont *m_Font = new CFont();
		pDC->SetTextColor(RGB(105, 105, 105));

		//设置分类排序点击变色
		if (pWnd->GetDlgCtrlID() == choose_classfy_current)
		{
			pDC->SetTextColor(RGB(218, 165, 32));
		}

		m_Font->CreatePointFont(300, "华文行楷", NULL);
		pDC->SelectObject(m_Font);
		delete m_Font;

		return   (HBRUSH)::GetStockObject(NULL_BRUSH);
	}

	//设置显示乐曲数字体颜色和大小
	if (pWnd->GetDlgCtrlID() == IDC_MIDI_NUMBER)
	{

		//pDC->SetBkMode(TRANSPARENT);
		pDC->SetBkColor(RGB(197, 231, 92));
		pDC->SetTextColor(RGB(46, 139, 87
			));
		CFont *m_Font = new CFont();
		

		m_Font->CreatePointFont(180, "楷体", NULL);
		pDC->SelectObject(m_Font);
		delete m_Font;

		return   (HBRUSH)::GetStockObject(NULL_BRUSH);
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CSelectDlg::OnStnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_DocumentLevel == FOLDER_LEVEL)
	{
		m_SendMessage(0);
		//m_DocumentLevel = FILE_LEVEL;
		int m_index = m_SelectList.GetCurSel();
		CString Buffer;
		if (m_index >= 0)m_SelectList.GetText(m_index, m_CurrentFolder);
		m_CurrentFolder.Trim();

		int m_StringLength = m_CurrentFolder.GetLength();
		for (int counter = 0; counter < m_StringLength; counter++)
		{
			if (m_CurrentFolder.GetAt(counter) == '[')break;
			Buffer += m_CurrentFolder.GetAt(counter);
		}
		m_CurrentFolder = Buffer;

		CSelectDlg m_SelectDlg;
		m_SelectDlg.m_Initialize(m_CurrentFolder, FILE_LEVEL);
		m_SelectDlg.m_SetClassify(ALL_MUSIC);
		m_SelectDlg.DoModal();

		CDialogEx::OnOK();
	}
	else if (m_DocumentLevel == FILE_LEVEL)
	{
		m_CallExecute();
		m_SendMessage(1);
		CDialogEx::OnOK();
	}
}


void CSelectDlg::OnStnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	m_SendMessage(0);
	if (m_DocumentLevel == FOLDER_LEVEL)
	{
		
		CDialogEx::OnCancel();
	}
	else if (m_DocumentLevel == FILE_LEVEL)
	{
		CSelectDlg m_SelectDlg;
		m_SelectDlg.m_Initialize(m_CurrentFolder, FOLDER_LEVEL);
		m_SelectDlg.m_SetClassify(ALL_MUSIC);
		m_SelectDlg.DoModal();

		CDialogEx::OnCancel();
	}
}


void CSelectDlg::OnLbnSelchangeSelectList()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CSelectDlg::OnStnClickedClassify0()
{
	// TODO: 在此添加控件通知处理程序代码
	m_SendMessage(0);
	CSelectDlg m_SelectDlg;
	m_SelectDlg.m_Initialize(m_CurrentFolder, FILE_LEVEL);
	m_SelectDlg.m_SetClassify(ALL_MUSIC);
	m_SelectDlg.DoModal();
	CDialogEx::OnCancel();

	int buf = choose_classfy_current;
	choose_classfy_before = buf;
	choose_classfy_current = IDC_CLASSIFY_0;
	Show_ClassfyClicked(choose_classfy_before);
	Show_ClassfyClicked(choose_classfy_current);

	//UpdateData(TRUE);
	//m_SelectDlg.DestroyWindow();
}


void CSelectDlg::OnStnClickedClassify1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_SendMessage(0);
	CSelectDlg m_SelectDlg;
	m_SelectDlg.m_Initialize(m_CurrentFolder, FILE_LEVEL);
	m_SelectDlg.m_SetClassify(ANIME_MUSIC);
	m_SelectDlg.DoModal();
	CDialogEx::OnCancel();

	int buf = choose_classfy_current;
	choose_classfy_before = buf;
	choose_classfy_current = IDC_CLASSIFY_1;
	Show_ClassfyClicked(choose_classfy_before);
	Show_ClassfyClicked(choose_classfy_current);
	//UpdateData(TRUE);
	//m_SelectDlg.DestroyWindow();
}


void CSelectDlg::OnStnClickedClassify5()
{
	// TODO: 在此添加控件通知处理程序代码
	m_SendMessage(0);
	CSelectDlg m_SelectDlg;
	m_SelectDlg.m_Initialize(m_CurrentFolder, FILE_LEVEL);
	m_SelectDlg.m_SetClassify(UNCLASSIFIED_MUSIC);
	m_SelectDlg.DoModal();
	CDialogEx::OnCancel();

	int buf = choose_classfy_current;
	choose_classfy_before = buf;
	choose_classfy_current = IDC_CLASSIFY_5;
	Show_ClassfyClicked(choose_classfy_before);
	Show_ClassfyClicked(choose_classfy_current);
	//UpdateData(TRUE);
	//m_SelectDlg.DestroyWindow();
}


void CSelectDlg::OnStnClickedClassify4()
{
	// TODO: 在此添加控件通知处理程序代码
	m_SendMessage(0);
	CSelectDlg m_SelectDlg;
	m_SelectDlg.m_Initialize(m_CurrentFolder, FILE_LEVEL);
	m_SelectDlg.m_SetClassify(SHORT_MUSIC);
	m_SelectDlg.DoModal();
	CDialogEx::OnCancel();

	int buf = choose_classfy_current;
	choose_classfy_before = buf;
	choose_classfy_current = IDC_CLASSIFY_4;
	Show_ClassfyClicked(choose_classfy_before);
	Show_ClassfyClicked(choose_classfy_current);
	//UpdateData(TRUE);
	//m_SelectDlg.DestroyWindow();
}


void CSelectDlg::OnStnClickedClassify2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_SendMessage(0);
	CSelectDlg m_SelectDlg;
	m_SelectDlg.m_Initialize(m_CurrentFolder, FILE_LEVEL);
	m_SelectDlg.m_SetClassify(CLASSICAL_MUSIC);
	m_SelectDlg.DoModal();
	CDialogEx::OnCancel();

	int buf = choose_classfy_current;
	choose_classfy_before = buf;
	choose_classfy_current = IDC_CLASSIFY_2;
	Show_ClassfyClicked(choose_classfy_before);
	Show_ClassfyClicked(choose_classfy_current);
	//UpdateData(TRUE);
	//m_SelectDlg.DestroyWindow();
}


void CSelectDlg::OnStnClickedClassify3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_SendMessage(0);
	CSelectDlg m_SelectDlg;
	m_SelectDlg.m_Initialize(m_CurrentFolder, FILE_LEVEL);
	m_SelectDlg.m_SetClassify(POP_MUSIC);
	m_SelectDlg.DoModal();
	CDialogEx::OnCancel();

	int buf = choose_classfy_current;
	choose_classfy_before = buf;
	choose_classfy_current = IDC_CLASSIFY_3;
	Show_ClassfyClicked(choose_classfy_before);
	Show_ClassfyClicked(choose_classfy_current);
	//UpdateData(TRUE);
	//m_SelectDlg.DestroyWindow();
}

void  CSelectDlg::Show_ClassfyClicked(int kind) {
	CString flag;

	switch (kind)
	{
	case IDC_CLASSIFY_0:flag.Format("%s", "所有乐曲"); break;
	case IDC_CLASSIFY_1:flag.Format("%s", "动漫乐曲"); break;
	case IDC_CLASSIFY_2:flag.Format("%s", "古典乐曲"); break;
	case IDC_CLASSIFY_3:flag.Format("%s", "流行乐曲"); break;
	case IDC_CLASSIFY_4:flag.Format("%s", "简短乐曲"); break;
	case IDC_CLASSIFY_5:flag.Format("%s", "其它乐曲"); break;
	}
	GetDlgItem(kind)->SetWindowTextA(flag);
}
