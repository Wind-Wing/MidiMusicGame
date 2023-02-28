
// ProjectDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Project.h"
#include "ProjectDlg.h"
#include "afxdialogex.h"

// The head file of function timeSetEvent()
#include <windows.h>
#pragma comment(lib,"Winmm.lib")
#include <MMSystem.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CProjectDlg 对话框
// Global Function that used to be ReCall
void PASCAL m_BlockDroppingTimer(UINT wTimerID, UINT msg, DWORD dwUser, DWORD dwl, DWORD dw2)
{

	CProjectDlg *ptr = (CProjectDlg*)dwUser;
	if (ptr->m_CanStart())
	{
		// Drop blocks
		ptr->m_CallBlockDropping();
		////Destroy blocks
		//ptr->m_CallBlockDestroy();
		ptr = NULL;
	}
}

void PASCAL m_BlockCreateTimer(UINT wTimerID, UINT msg, DWORD dwUser, DWORD dwl, DWORD dw2)
{
	CProjectDlg *ptr = (CProjectDlg*)dwUser;
	if (ptr->m_CanStart())
	{
		// Drop blocks
		ptr->m_CallBlockCreate();
		ptr = NULL;
	}
}

CProjectDlg::CProjectDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PROJECT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CProjectDlg::~CProjectDlg()
{
	// Destroy Timer
	if (m_BlockDroppingTimeHandle)
		timeKillEvent(this->m_BlockDroppingTimeHandle);
	if (m_BlockCreateTimeHandle)
		timeKillEvent(this->m_BlockCreateTimeHandle);
	KillTimer(TIMER_PRINT);
	// Close Device
	if (m_InDevice.IsOpen())
	{
		if (m_InDevice.IsRecording())
			m_InDevice.StopRecording();
		m_InDevice.Close();
	}
	if (m_OutDevice.IsOpen())
		m_OutDevice.Close();

	//if (m_RankDlg != NULL)
	//	m_RankDlg->DestroyWindow();

}

void CProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_SPEED_SLIDER, m_SpeedSlider);

	//添加乐曲速度控制，简单、普通、困难
	//此处为设置数据更新
	/*DDX_Control(pDX, IDC_SPEED_CONTROL_1, m_SpeedSlider);
	DDX_Control(pDX, IDC_SPEED_CONTROL_2, m_SpeedSlider);
	DDX_Control(pDX, IDC_SPEED_CONTROL_3, m_SpeedSlider);*/

	DDX_Control(pDX, IDC_RETURN, m_Buffer);
	DDX_Control(pDX, IDC_SCORE_TEXT, m_ScoreText);
}

BEGIN_MESSAGE_MAP(CProjectDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_CREATE()

	//	ON_WM_KEYDOWN()
	//	ON_WM_KEYUP()
	//ON_BN_CLICKED(IDC_BUTTON_SELECT, &CProjectDlg::OnBnClickedButtonSelect)
	//ON_BN_CLICKED(IDC_BUTTON_LEARNMORE, &CProjectDlg::OnBnClickedButtonLearnMore)
	//ON_BN_CLICKED(IDC_BUTTON_RETURN, &CProjectDlg::OnBnClickedButtonReturn)
	//ON_NOTIFY(NM_CUSTOMDRAW, IDC_SPEED_SLIDER, &CProjectDlg::OnNMCustomdrawSpeedSlider)
	ON_MESSAGE(WM_SELECT, &CProjectDlg::OnSelect)
	ON_MESSAGE(WM_MODE, &CProjectDlg::OnModel)
	ON_WM_DRAWITEM()

	//注册监听事件

	/*
	将图片形式的打开、返回，以及速度调节全部换成Static Text
	*设置背景透明
	*/
	// ON_STN_CLICKED(IDC_OPEN, &CProjectDlg::OnClickedPictureOpen)
	//ON_STN_CLICKED(IDC_PICTURE_LEARNMORE, &CProjectDlg::OnClickedPictureLearnmore)
	//ON_STN_CLICKED(IDC_PICTURE_RETURN, &CProjectDlg::OnClickedPictureReturn)

	ON_STN_CLICKED(IDC_OPEN, &CProjectDlg::OnStnClickedOpen)
	ON_STN_CLICKED(IDC_RETURN, &CProjectDlg::OnStnClickedReturn)
	ON_STN_CLICKED(IDC_LEARNMORE, &CProjectDlg::OnStnClickedLearnmore)

	ON_STN_CLICKED(IDC_SPEED_CONTROL_1, &CProjectDlg::OnClickedSpeedControl_1)
	ON_STN_CLICKED(IDC_SPEED_CONTROL_2, &CProjectDlg::OnClickedSpeedControl_2)
	ON_STN_CLICKED(IDC_SPEED_CONTROL_3, &CProjectDlg::OnClickedSpeedControl_3)

	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, &CProjectDlg::OnBnClickedOk)
	ON_STN_CLICKED(IDC_FREE_PLAY, &CProjectDlg::OnStnClickedFreePlay)
	ON_STN_CLICKED(IDC_CYCLERATIO, &CProjectDlg::OnStnClickedCycleratio)
	ON_STN_CLICKED(IDC_NATURALNUM, &CProjectDlg::OnStnClickedNaturalnum)
	ON_STN_CLICKED(IDC_IRRATIONALNUM, &CProjectDlg::OnStnClickedIrrationalnum)
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CProjectDlg 消息处理程序

BOOL CProjectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	ShowWindow(SW_MAXIMIZE);
	// Set the size of Windows
	//int x = ::GetSystemMetrics(SM_CXSCREEN);
	//int y = ::GetSystemMetrics(SM_CYSCREEN);
	//::MoveWindow(this->m_hWnd, 0, 0, x, y, true);
	//TODO: 在此添加额外的初始化代码

// My Code
	// Initialize Auto Return CountDown
	m_ReturnCountDown = RETURN_COUNTDOWN_TIME;

	// Initialize Highlight Mark
	navigation_flag_before = navigation_flag_current = IDC_SPEED_CONTROL_3;

	//重置得分栏
	m_ScoreText.ResetContent();
	m_ScoreText.AddString("0                   ");

	// Create MaxT.out
	CreateTxetFile();

	// Initialize State (Stop)
	m_State = FALSE;

	// Get Client Size
	GetClientRect(&m_client);

	// Initialize Objects
	m_Background.m_SetPosition(m_client, 1);//1代表projectDlg中的背景图片设置
	m_Stave.m_Initialize(m_client);
	m_PowerLow.m_Initialize(m_client);
	m_Title.m_SetPosition(m_client);
	m_PianoKeyboard.m_Initialize(m_client);
	m_Ruler.m_Initialize(m_client);
	m_BlockLength = m_BlockControl.m_Initialize(m_client, m_PianoKeyboard);

	m_BlockCreateTimeHandle = NULL;
	m_BlockDroppingTimeHandle = NULL;

	// Show Welcome Dialog
	//Show Welcome Dialog must be at this place or may cause problems

	CWelcomeDlg m_WelcomeDlg;
	INT_PTR m_WelcomeDlgReturn;
	m_WelcomeDlgReturn = m_WelcomeDlg.DoModal();
	m_WelcomeDlg.DestroyWindow();

	// Set Bottoms Position

	// m_ButtonPosition 导航栏四个按钮的Position
	m_ButtonPosition.top = (int)m_client.bottom * BOTTOM_TOP;
	m_ButtonPosition.bottom = (int)m_client.bottom * BOTTOM_BOTTON;

	// Move My Bottoms
	m_MoveButtons();
	Show_Model(m_Model);

	//// Set Slider
	//m_SpeedSlider.SetRange(100, 300);
	//m_SpeedSlider.SetTicFreq(100);
	//// Set Slider Lock
	//m_SliderLock = TRUE;

	// Set CountDown Position
	m_CountDownPosition.top = (int)m_client.bottom*0.3;
	m_CountDownPosition.bottom = (int)m_client.bottom*0.7;
	m_CountDownPosition.left = (int)m_client.right *0.4;
	m_CountDownPosition.right = (int)m_client.right *0.6;
	// Set CountDownCounter
	m_CountDownCounter = COUNTERDOWN_TIME;
	m_CountDownLock = TRUE;
	//Set perfect
	int perfect_flag = 0;
	int perfect_flag_2 = 0;
	int perfect_counter = COUNTERDOWN_STEP / 5;

	// Set Score
	for (int counter = 0; counter < 5; counter++)
	{
		m_Scores[counter] = 0;
	}

	// Attetion: LinkMIDIDevice Must be put behind Set Timer 
	//			 or will cause timer is not accurate
	// Initialize MIDIErrorLock
	m_MIDIErrorLock = FALSE;
	// Link MIDI Device
	m_LinkDevice();
	// To Pass Model into Objects
	// and Reset the Moving Items
	m_Restart();
	// Change State to Start
	//m_State = TRUE;

	//加载Perfect图片
	m_PerfectPicture.Load("res\\perfect.png");

	// Import New Midi
		// Get New MidiPath
	CString Buffer = m_GetNewMidiPath();
	// Classify New Midi
	m_ClassifyNewMidi(Buffer);

	// Must be Put behind m_LinkDevice
	// Because SelectDlg can leave time for Initialize
	// PopUp a SelectDlg after the user choose the Model
	if (m_Model == CHALLENGE_MODEL || m_Model == LEARNING_MODEL)
	{
		OnStnClickedOpen();
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。 

//绘制游戏主界面
void CProjectDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);


	}
	/*
	else{}语句  绘图入口
	*/
	else
	{
		m_BlockControl.m_ChangeOtherObjectPosition(m_Stave, m_Ruler);
		// Create CacheDC
		CDC *cDC = this->GetDC();
		m_cacheDC.CreateCompatibleDC(NULL);
		m_cacheCBitmap.CreateCompatibleBitmap(cDC, m_client.Width(), m_client.Height());
		m_cacheDC.SelectObject(&m_cacheCBitmap);

		// Challenge_Model 
		if (m_Model == CHALLENGE_MODEL)
		{
			// 修改块消失时的位置
			perfect_flag = m_BlockControl.m_BlockDestroy(m_PowerLow);
			m_CalculateScore(perfect_flag);
		}
		// Learning_Model 
		if (m_Model == LEARNING_MODEL)
		{
			m_BlockControl.m_BlockDestroy(m_PowerLow);
		}
		// Create_Model
		if (m_Model == CREATE_MODEL)
		{
			//设置默认自由弹奏高亮显示
			free_play = IDC_FREE_PLAY;
			m_BlockControl.m_BlockDestroy(m_PowerLow);
			m_ShowCreateModelNumber();
		}

		m_Background.m_Print(m_cacheDC);
		m_BlockControl.m_Print(m_cacheDC);
		m_PowerLow.m_Print(m_cacheDC);
		m_Stave.m_Print(m_cacheDC);
		m_Ruler.m_Print(m_cacheDC);

		//m_NumberedMusicNotation.m_Print(m_cacheDC);
		m_PianoKeyboard.m_Print(m_cacheDC);

		//绘制Title
		m_Title.m_Print(m_cacheDC);
		// Draw a arrow pointer to show the current number
		if (m_Model == CREATE_MODEL)
		{
			m_Title.m_DrawBox(m_cacheDC);
		}
		//弹Perfect
		//OnPaint()方法为一个时钟周期，每次固定时间刷新。块下落时的值要延迟一定时间再刷新传入
		//修改时间：2016年6月15日
		m_Perfect(m_cacheDC);

		// CountDown when nessary
		//m_CountDown : 3、2、1倒计时
		if ((!m_CountDownLock) && m_State) { m_CountDown(m_cacheDC); }

		// Print on Screen
		cDC->BitBlt(0, 0, m_client.Width(), m_client.Height(), &m_cacheDC, 0, 0, SRCCOPY);

		// Adjust m_Slider to achieve classification
		//if (m_SliderLock) { m_AdjustSilder(); }

	// Release 
		ValidateRect(&m_client);
		//Invalidate(FALSE);
		m_cacheDC.DeleteDC();
		m_cacheCBitmap.DeleteObject();
		ReleaseDC(cDC);
		//CDialogEx::OnPaint();

		// Must be put a place that is behind in some degree to avoid 
		// Dialog popup stop other functions' running
		if (m_Model == CHALLENGE_MODEL&&m_BlockControl.m_IsMusicEnd())
		{
			m_ShowRank();
		}

		// Check whether fit the condition to auto return 
		if (m_State&&!m_ReturnCountDown)
		{
			OnStnClickedReturn();
		}
		else if (m_State)
		{
			m_ReturnCountDown--;
		}
	}
}


void CProjectDlg::m_CalculateScore(int m_Input)
{
	// m_Input == 0 means invalid input
	if (m_Input == 0)return;
	// Vital statistics of all levels of grades	
	m_Scores[m_Input] ++;
	// Calc the total score

	//以下为原版本
	//if((m_Input * 20 - 30) > 0)                
	//m_Scores[0] += (m_Input * 20 - 30);

	//此为新改动部分,改为按到就算分,且基数扩大10倍。
	//首先，判断m_Input的输入类型，根据相应类型计算得分
	srand((unsigned)time(NULL));
	//Miss不得分
	if (m_Input == 1) {
		m_Scores[0] += 0;
	}
	//一个Good加100分
	if (m_Input == 2) {
		m_Scores[0] += (100 + (rand() % 100));
	}
	//一个Great加200分
	if (m_Input == 3) {
		m_Scores[0] += (200 + (rand() % 100));
	}
	//一个Perfect加500分
	if (m_Input == 4) {
		m_Scores[0] += (500 + (rand() % 100));
	}
}

void CProjectDlg::UpdateScore(int score) {
	score_buf.Format("%d", score);
	m_ScoreText.ResetContent();
	m_ScoreText.AddString(score_buf);
}

void CProjectDlg::m_CreateModelUpdateNumber(int m_Input)
{
	CString buf;
	buf.Format("%d", m_Input);
	m_ScoreText.ResetContent();
	m_ScoreText.AddString(buf);
}

void CProjectDlg::m_CallBlockDropping()
{
	m_BlockControl.m_BlockDropping();
}

void CProjectDlg::m_CallBlockCreate()
{
	m_BlockControl.m_BlockCreate();
}


//导航栏四个按钮的界面布局
void CProjectDlg::m_MoveButtons()
{
	m_ButtonPosition.left = m_client.right * 0.02;
	m_ButtonPosition.right = m_client.right * 0.10;
	GetDlgItem(IDC_OPEN)->MoveWindow(m_ButtonPosition);

	m_ButtonPosition.left = m_client.right * 0.10;
	m_ButtonPosition.right = m_client.right * 0.18;
	GetDlgItem(IDC_RETURN)->MoveWindow(m_ButtonPosition);

	m_ButtonPosition.left = m_client.right * 0.18 + m_client.right *0.03;
	m_ButtonPosition.right = m_client.right * 0.22 + m_client.right *0.03;
	GetDlgItem(IDC_CYCLERATIO)->MoveWindow(m_ButtonPosition);

	m_ButtonPosition.left = m_client.right * 0.30 + m_client.right *0.03;
	m_ButtonPosition.right = m_client.right * 0.34 + m_client.right *0.03;
	GetDlgItem(IDC_NATURALNUM)->MoveWindow(m_ButtonPosition);

	m_ButtonPosition.left = m_client.right * 0.42 + m_client.right *0.03;
	m_ButtonPosition.right = m_client.right * 0.48 + m_client.right *0.03;
	GetDlgItem(IDC_IRRATIONALNUM)->MoveWindow(m_ButtonPosition);

	m_ButtonPosition.left = m_client.right * 0.56 + m_client.right *0.03;
	m_ButtonPosition.right = m_client.right * 0.68 + m_client.right *0.03;
	GetDlgItem(IDC_FREE_PLAY)->MoveWindow(m_ButtonPosition);

	m_ButtonPosition.left = m_client.right * 0.22 + m_client.right *0.03;
	m_ButtonPosition.right = m_client.right * 0.30 + m_client.right *0.03;
	GetDlgItem(IDC_SPEED_CONTROL_1)->MoveWindow(m_ButtonPosition);

	m_ButtonPosition.left = m_client.right * 0.34 + m_client.right *0.03;
	m_ButtonPosition.right = m_client.right * 0.42 + m_client.right *0.03;
	GetDlgItem(IDC_SPEED_CONTROL_2)->MoveWindow(m_ButtonPosition);

	m_ButtonPosition.left = m_client.right * 0.48 + m_client.right *0.03;
	m_ButtonPosition.right = m_client.right * 0.56 + m_client.right *0.03;
	GetDlgItem(IDC_SPEED_CONTROL_3)->MoveWindow(m_ButtonPosition);

	m_ButtonPosition.left = m_client.right * 0.71;
	m_ButtonPosition.right = m_client.right * 0.76;
	GetDlgItem(IDB_SCORE)->MoveWindow(m_ButtonPosition);

	m_ButtonPosition.left = m_client.right * 0.76;
	m_ButtonPosition.right = m_client.right * 0.91;
	m_ButtonPosition.bottom = m_ButtonPosition.bottom*0.90;
	GetDlgItem(IDC_SCORE_TEXT)->MoveWindow(m_ButtonPosition);

	//显示键盘do
	CRect pos;
	pos.top = (int)(this->m_client.bottom)*(KEY_BOARD_TOP + 0.16);
	pos.bottom = (int)(this->m_client.bottom)*KEY_BOARD_BOTTOM;

	pos.left = m_client.right * 2 / 52;
	pos.right = m_client.right * 3 / 52;
	GetDlgItem(IDC_DO_1)->MoveWindow(pos);

	pos.left = m_client.right * 9 / 52;
	pos.right = m_client.right * 10 / 52;
	GetDlgItem(IDC_DO_2)->MoveWindow(pos);

	pos.left = m_client.right * 16 / 52;
	pos.right = m_client.right * 17 / 52;
	GetDlgItem(IDC_DO_3)->MoveWindow(pos);

	pos.left = m_client.right * 23 / 52;
	pos.right = m_client.right * 24 / 52;
	GetDlgItem(IDC_DO_4)->MoveWindow(pos);

	pos.left = m_client.right * 30 / 52;
	pos.right = m_client.right * 31 / 52;
	GetDlgItem(IDC_DO_5)->MoveWindow(pos);

	pos.left = m_client.right * 37 / 52;
	pos.right = m_client.right * 38 / 52;
	GetDlgItem(IDC_DO_6)->MoveWindow(pos);

	pos.left = m_client.right * 44 / 52;
	pos.right = m_client.right * 45 / 52;
	GetDlgItem(IDC_DO_7)->MoveWindow(pos);

	pos.left = m_client.right * 51 / 52;
	pos.right = m_client.right * 52 / 52;
	GetDlgItem(IDC_DO_8)->MoveWindow(pos);

	//m_PointerPosition.right = (int)(m_PointerPosition.left + m_PointerPosition.Width()*0.1);


	/*m_ButtonPosition.left = m_client.right * 0.75;
	m_ButtonPosition.right = m_client.right * 0.90;
	GetDlgItem(IDC_FREE_PLAY)->MoveWindow(m_ButtonPosition);*/

	/*m_ButtonPosition.left = m_client.right * 0.90;
	m_ButtonPosition.right = m_client.right * 0.98;
	GetDlgItem(IDC_RETURN)->MoveWindow(m_ButtonPosition);*/

	/*m_ButtonPosition.left = m_client.right * 0.86;
	m_ButtonPosition.right = m_client.right * 0.99;
	GetDlgItem(IDC_GOLDNUM)->MoveWindow(m_ButtonPosition);*/
}

/*
	弹Perfect
*/
void CProjectDlg::m_Perfect(CDC& m_cacheDC)
{
	CRect position;
	position.top = (int)m_client.bottom*0.4;
	position.bottom = (int)m_client.bottom*0.6;
	position.left = (int)m_client.right *0.4;
	position.right = (int)m_client.right *0.6;
	if (perfect_flag == 4) {
		perfect_flag_2 = perfect_flag;
	}
	if ((perfect_flag_2 == 4) && (perfect_counter != 0)) {
		m_PerfectPicture.Draw(m_cacheDC, position);
		perfect_counter--;
	}
	else {
		//弹得perfect持续时间
		perfect_counter = COUNTERDOWN_STEP / 3;//5
		perfect_flag_2 = perfect_flag;
	}
}

void CProjectDlg::m_CountDown(CDC& m_cacheDC)
{
	m_CountDownCounter--;
	m_CountDownPicture.Destroy();
	CString buf;
	buf.Format("res\\countdown%d.png", (int)(m_CountDownCounter / COUNTERDOWN_STEP) + 1);
	m_CountDownPicture.Load(buf);
	m_CountDownPicture.Draw(m_cacheDC, m_CountDownPosition);

	// End CounterDown and Start
	if (!m_CountDownCounter)m_CountDownLock = TRUE;

}

void CProjectDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

//键盘按键响应事件
BOOL CProjectDlg::PreTranslateMessage(MSG* pMsg)
{
	//TODO: 在此添加专用代码和/或调用基类
	if (pMsg->wParam >= LOW_PITCH && pMsg->wParam < HIGH_PITCH)
	{
		//按下键时的情况
		if (pMsg->message == WM_KEYDOWN)
		{
			bool Buffer = FALSE;
			// Check whether the data is Zero
			Buffer = m_BlockControl.m_KeyDown(pMsg->wParam);//传递键盘对应的ASCII码
			m_PianoKeyboard.m_KeyDown(pMsg->wParam, Buffer);
			//统计幂律
			m_PowerLow.m_KeyDown(pMsg->wParam);

			return TRUE;
		}
		else if (pMsg->message == WM_KEYUP)
		{
			m_BlockControl.m_KeyUp(pMsg->wParam);
			m_PianoKeyboard.m_KeyUp(pMsg->wParam);
			m_PowerLow.m_KeyUp(pMsg->wParam);

			//显示得分
			//不可放在WM_KEYDOWN中！
			if (m_Model == CHALLENGE_MODEL)
			{
				UpdateScore(m_Scores[0]);
			}
			// Don't Put this reset in WM_KEYDOWN !!
			// Reset Return CountDown
			m_ReturnCountDown = RETURN_COUNTDOWN_TIME;
			return TRUE;
		}
	}

	//return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}

//void CProjectDlg::OnNMCustomdrawSpeedSlider(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
//	// TODO: 在此添加控件通知处理程序代码
//	m_SliderLock = TRUE;
//	*pResult = 0;
//}

//m_ChangeSpeed(int speedRate)用于乐曲中简单、普通、困难三个速度调节
//修改时间：2016年6月16日
void CProjectDlg::m_ChangeSpeed(int kind)
{
	// Record and Check CurrentSpeed
	if (m_CurrentSpeed == kind)return;
	else m_CurrentSpeed = kind;
	// Get Current Slider Position
	//int m_SpeedRate = (int)(400.0 - m_SpeedSlider.GetPos()) / 50.0;//之前设置为除以100.0
	int delay_create = m_CreateRefreshTime;
	int delay_drop = m_DropRefreshTime;
	if (kind == 1) {
		//delay_create = m_CreateRefreshTime >> 1;//1
		//delay_drop = m_DropRefreshTime >> 1;//1
		delay_create = m_CreateRefreshTime;// /8
		delay_drop = m_DropRefreshTime;// /8
	}
	else if (kind == 2) {
		delay_create = m_CreateRefreshTime * 1.5;//原来是原速
		delay_drop = m_DropRefreshTime * 1.5;//原来是原速
	}
	else if (kind == 3) {
		delay_create = m_CreateRefreshTime * 2;
		delay_drop = m_DropRefreshTime * 2;
	}

	// Stop for a while
	m_State = FALSE;
	// Kill Event
	if (m_BlockDroppingTimeHandle)
	{
		timeKillEvent(this->m_BlockDroppingTimeHandle);
		m_BlockDroppingTimeHandle = NULL;
	}
	if (m_BlockCreateTimeHandle)
	{
		timeKillEvent(this->m_BlockCreateTimeHandle);
		m_BlockCreateTimeHandle = NULL;
	}

	//???
		// Set New TimerEvent
	m_BlockCreateTimeHandle = timeSetEvent(delay_create, TIMING_ACCURACY, (LPTIMECALLBACK)m_BlockCreateTimer, (DWORD_PTR)this, TIME_PERIODIC);
	m_BlockDroppingTimeHandle = timeSetEvent(delay_drop, TIMING_ACCURACY, (LPTIMECALLBACK)m_BlockDroppingTimer, (DWORD_PTR)this, TIME_PERIODIC);
	// Set Lock
	//m_SliderLock = FALSE;
	// Start Again
	m_State = TRUE;
	// Test Code---------------------------------------
	//CString str;
	//str.Format("%d", m_SpeedRate);
	//GetDlgItem(IDC_BUTTON_LEARNMORE)->SetWindowText(str);
	//-------------------------------------------------

}

void CProjectDlg::OnBnClickedButtonSelect()
{
	// TODO: 在此添加控件通知处理程序代码

	// Don't forget to Destroy it !!!
	/*m_SelectDlg = new CSelectDlg();
	m_SelectDlg->Create(IDD_SELECT_DIALOG, this);*/
	/*CString buf;
	buf.Format("%s", "123456");
	m_SelectDlg->SendMessage(WM_MY_SELECT, (WPARAM)&buf);
	m_SelectDlg->ShowWindow(SW_SHOWNORMAL);
	*/
	m_State = FALSE;
	CSelectDlg m_SelectDlg;
	INT_PTR m_SelectDlgReturn;
	m_SelectDlgReturn = m_SelectDlg.DoModal();
	UpdateData(TRUE);
	m_State = TRUE;
	m_SelectDlg.DestroyWindow();

}

afx_msg LRESULT CProjectDlg::OnSelect(WPARAM wParam, LPARAM lParam)
{	
	bool m_Input = *(bool*)wParam;
	CString buf;
	buf.Format("%d", m_Input);

	//MessageBox(buf);
	if (m_Input == TRUE)
	{
		//MessageBox("ok");
		m_Restart();
		// Reset CountDown
		m_CountDownCounter = COUNTERDOWN_TIME;
		m_CountDownLock = FALSE;
		// 默认高级速度
		m_State = TRUE;
	}
	return 0;
}

afx_msg LRESULT CProjectDlg::OnModel(WPARAM wParam, LPARAM lParam)
{
	//MessageBox(*(CString*)wParam);
	m_Model = *(int*)wParam;
	return 0;
}

void CProjectDlg::m_ShowCreateModelNumber()
{
	CString buf;
	int buffer;
	buffer = m_BlockControl.m_CreateModelPitchToNumber();
	if (buffer == 11) { m_CreateModelNumber.Delete(0, 1); return; }
	if (buffer == 10)return;
	buf.Format("%d", buffer);
	m_CreateModelNumber.Delete(0, 1);
	m_CreateModelNumber.Insert(6, buf);
	m_ScoreText.ResetContent();
	m_ScoreText.AddString(m_CreateModelNumber);

}

CString CProjectDlg::m_GetNewMidiPath()
{
	// Get MidiPath
	CString m_NewMidiPathBuffer;
	CString m_NewMidiPath;
	CStdioFile m_File;
	int m_StringLength;
	bool m_StringFlag = FALSE;
	m_File.Open("conf\\MidiPath.conf", CFile::modeRead);
	m_File.ReadString(m_NewMidiPathBuffer);
	m_StringLength = m_NewMidiPathBuffer.GetLength();
	for (int counter = 0; counter < m_StringLength; counter++)
	{
		if (m_NewMidiPathBuffer.GetAt(counter) == '"' && !m_StringFlag)
			m_StringFlag = TRUE;
		else if (m_NewMidiPathBuffer.GetAt(counter) == '"' && m_StringFlag)
			m_StringFlag = FALSE;
		else
		{
			if (m_StringFlag)
				m_NewMidiPath += m_NewMidiPathBuffer.GetAt(counter);
			if (m_StringFlag && m_NewMidiPathBuffer.GetAt(counter) == '\\')
				m_NewMidiPath += m_NewMidiPathBuffer.GetAt(counter);
		}
	}
	if (m_NewMidiPath.GetAt(m_NewMidiPath.GetLength()) != '\\')
		m_NewMidiPath += "\\\\";
	//MessageBox(m_NewMidiPath);
	m_File.Close();
	return m_NewMidiPath;

}

void CProjectDlg::m_ClassifyNewMidi(CString m_InputPath)
{
	int counter = 0;
	CFileFind m_finder;
	bool m_judge;
	CString FolderPath;
	CString MidiPath;
	CString TargetMidiPath;
	int m_Class;
	CString ClassifyFolds[4];
	ClassifyFolds[1] = "(1)初级\\";
	ClassifyFolds[2] = "(2)中级\\";
	ClassifyFolds[3] = "(3)高级\\";

	FolderPath = m_InputPath + "*.mid";
	// Traverse the Folder
	m_judge = m_finder.FindFile(FolderPath);

	while (m_judge)
	{
		m_judge = m_finder.FindNextFile();
		//MessageBox(m_finder.GetFileTitle());
		MidiPath = m_InputPath + m_finder.GetFileTitle() + ".mid";
		m_Class = m_CallExecute(MidiPath);
		CString Buffer;
		Buffer.Format("%d", m_Class);
		TargetMidiPath = "MidiLib\\" + ClassifyFolds[m_Class] + m_finder.GetFileTitle() + ".mid";
		MoveFile(MidiPath, TargetMidiPath);
		counter++;
	}
	m_finder.Close();

	// Check Whether there are New MIDI files
	if (!counter)
		MessageBox("No New Midi", "Tip", MB_OK);
	else
		MessageBox("Update New Midi Finished", "Tip", MB_OK);

}

int CProjectDlg::m_CallExecute(CString m_InputPath)
{
	//MessageBox(m_InputPath);
	CString Buffer = m_InputPath + " exe\\MaxT.txt";
	FILE *m_music;
	float Buf;

	SHELLEXECUTEINFO  ShExecInfo = { 0 };
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = _T("open");
	ShExecInfo.lpFile = _T("exe\\MIDI2TXT.exe");
	ShExecInfo.lpParameters = Buffer;
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

	fopen_s(&m_music, "exe\\MaxT.out", "r");
	fscanf_s(m_music, "%f", &Buf);
	fclose(m_music);

	if (Buf < 1.7)return 1;
	else if (Buf < 4.0)return 2;
	else return 3;
}

//-----------------Hardly Changed Code------------------//

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CProjectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

bool CProjectDlg::m_CanStart()
{
	return (m_State&&m_CountDownLock);
}

// This function is Abandoned
void CProjectDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_State)
		switch (nIDEvent)
		{
		case TIMER_PRINT:OnPaint(); break;
		}

	CDialogEx::OnTimer(nIDEvent);
}

int CProjectDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	SetTimer(TIMER_PRINT, REFRESH_TIME, NULL);
	return 0;
}

void CProjectDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CProjectDlg::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnKeyUp(nChar, nRepCnt, nFlags);
}


void CProjectDlg::OnClickedSpeedControl_1()
{
	navigation_flag_before = navigation_flag_current;
	navigation_flag_current = IDC_SPEED_CONTROL_1;
	Show_NavigationClicked(navigation_flag_before);
	Show_NavigationClicked(navigation_flag_current);

	m_ChangeSpeed(3);
}

void CProjectDlg::OnClickedSpeedControl_2()
{
	navigation_flag_before = navigation_flag_current;
	navigation_flag_current = IDC_SPEED_CONTROL_2;
	Show_NavigationClicked(navigation_flag_before);
	Show_NavigationClicked(navigation_flag_current);

	m_ChangeSpeed(2);
}

void CProjectDlg::OnClickedSpeedControl_3()
{
	navigation_flag_before = navigation_flag_current;
	navigation_flag_current = IDC_SPEED_CONTROL_3;
	Show_NavigationClicked(navigation_flag_before);
	Show_NavigationClicked(navigation_flag_current);

	m_ChangeSpeed(1);
}


void CProjectDlg::m_Restart()
{
	// Reset the CurrentSpeed to 2 (the Default Speed)
	m_CurrentSpeed = 3;
	// Reset Return CountDown
	m_ReturnCountDown = RETURN_COUNTDOWN_TIME;
	// Setting Zero of CountDown
	// To avoid the sitution when the player press return before
	// countdown finished
	m_CountDownCounter = 0;
	m_CountDownLock = TRUE;
	//重置导航栏按钮点击状态
	navigation_flag_before = navigation_flag_current = IDC_SPEED_CONTROL_3;
	Show_NavigationClicked(navigation_flag_before);
	Show_NavigationClicked(navigation_flag_current);
	//重置得分栏
	m_ScoreText.ResetContent();
	m_ScoreText.AddString("0                   ");

	// Reset Score
	for (int counter = 0; counter < 5; counter++)
	{
		m_Scores[counter] = 0;
	}

	//设置载入MIDI时的初始速度
	//修改后版本,修改时间：2016年6月15日
	// Reset m_BlockControl
	m_BlockControl.m_Reset(m_Model);
	// Get my RefreshTime
	m_CreateRefreshTime = m_BlockControl.m_GetText() * 4;
	// Reset Stave
	m_Stave.m_Reset();
	// Reset Powerlow
	m_PowerLow.m_Reset(m_Model);
	// Reset Ruler
	m_Ruler.m_Initialize(m_client);
	// Reset PianoKeyBoard
	m_PianoKeyboard.m_Reset();
	// Reset Score
	for (int counter = 0; counter < 5; counter++)
	{
		m_Scores[counter] = 0;
	}
	// Destroy Timer
	if (m_BlockDroppingTimeHandle)
	{
		timeKillEvent(this->m_BlockDroppingTimeHandle);
		m_BlockDroppingTimeHandle = NULL;
	}
	if (m_BlockCreateTimeHandle)
	{
		timeKillEvent(this->m_BlockCreateTimeHandle);
		m_BlockCreateTimeHandle = NULL;
	}
	// Set Timer 
	// Warning :When the TimePeriod is smaller than the running time that is needed by Recallfunction
	//			timeSetEvent will go wrong
	m_BlockCreateTimeHandle = timeSetEvent(m_CreateRefreshTime, TIMING_ACCURACY, (LPTIMECALLBACK)m_BlockCreateTimer, (DWORD_PTR)this, TIME_PERIODIC);
	// Block_DROPPING_STEP is defiened in BlockControl.h
	m_DropRefreshTime = (int)(m_CreateRefreshTime*BLOCK_DROPPING_STEP / m_BlockLength);
	m_BlockDroppingTimeHandle = timeSetEvent(m_DropRefreshTime, TIMING_ACCURACY, (LPTIMECALLBACK)m_BlockDroppingTimer, (DWORD_PTR)this, TIME_PERIODIC);
}

void CProjectDlg::CreateTxetFile()
{
	CFile  m_TextFile("exe\\MaxT.out ", CFile::modeCreate);
	m_TextFile.Close();
}

void CProjectDlg::m_ShowRank()
{

	// Create a rank temp file
	CFile  m_CreateTempRankFile("res\\rank.temp ", CFile::modeCreate);
	m_CreateTempRankFile.Close();

	FILE *m_RankFile;
	FILE *m_TempRankFile;
	// open Files
	fopen_s(&m_RankFile, "res\\rank.music", "r");
	fopen_s(&m_TempRankFile, "res\\rank.temp", "w");

	int m_Rank = 0;			// Used to record the rank of this player
	int m_HighestScore = 0; // Used to record the Highiest historical score
	int m_BufferScore = 0;  // Used to store the historical score of each line temporarily
	bool m_IsFind = FALSE;  // Mark whether the right rank is found
	CString buf;
	// Traverse the histroy rank to find this player's rank
	while (m_Rank < RECORD_NUMBER)
	{
		// read a line
		fscanf_s(m_RankFile, "%d", &m_BufferScore);
		// check whether is the end of file
		if (m_BufferScore == -1)break;
		// Get the Highest Score
		if (m_Rank == 0)
		{
			m_HighestScore = m_BufferScore;
			if (m_HighestScore < m_Scores[0]) m_HighestScore = m_Scores[0];
		}
		// check whether found the right rank
		if (m_IsFind == FALSE)
		{
			if (m_BufferScore <= m_Scores[0])
			{
				// Insert the this player's score into file "rank.temp" if his in RECORD_NUMBER
				buf.Format("%d\n", m_Scores[0]);
				fprintf_s(m_TempRankFile, buf);
				m_IsFind = TRUE;
			}
			m_Rank++;
		}
		// Copy the Historical ranks into file "rank.temp"
		buf.Format("%d\n", m_BufferScore);
		fprintf_s(m_TempRankFile, buf);
	}
	if (m_IsFind == FALSE && m_Rank < RECORD_NUMBER) {
		buf.Format("%d\n", m_Scores[0]);
		fprintf_s(m_TempRankFile, buf);
	}
	// Mark the end of Temp File
	buf = "-1";
	fprintf_s(m_TempRankFile, buf);
	// Files Close
	fclose(m_TempRankFile);
	fclose(m_RankFile);

	// Replace file "rank.music" with "rank.temp"
	DeleteFileA("res\\rank.music");
	rename("res\\rank.temp", "res\\rank.music");

	//将当前玩家的得分写入到CurrentRank.temp文件中，RankDlg在弹出是将读取该文件
	FILE *m_CurrentRankFile;
	fopen_s(&m_CurrentRankFile, "res\\CurrentRank.temp", "w");
	buf.Format("%d\n", m_HighestScore);
	fprintf_s(m_CurrentRankFile, buf);
	buf.Format("%d\n", m_Scores[0]);
	fprintf_s(m_CurrentRankFile, buf);
	buf.Format("%d\n", m_Rank);
	fprintf_s(m_CurrentRankFile, buf);
	fprintf_s(m_CurrentRankFile, "-1");
	fclose(m_CurrentRankFile);

	Sleep(500);
	//弹出排名对话框
	RankDlgReturn = RankDlg.DoModal();
	RankDlg.DestroyWindow();

	// To avoid the problem of the player still press a key when 
	// the RankDlg PopUp and the player let the key go before close
	// the RankDlg
	m_PianoKeyboard.m_Reset();

	// Show a Dialog to present the Score and Rank of this player

		// Prepare the Data Package
		/*int m_MessageToRankDlg[3] = { m_HighestScore,m_Scores[0],m_Rank};

		if (m_RankDlg != NULL)
		{
			m_RankDlg->DestroyWindow();
			m_RankDlg = NULL;
		}
		m_RankDlg = new CRankDlg;
		m_RankDlg->Create(IDD_RANK_DIALOG, this);

		if (&m_RankDlg == NULL) return (void)MessageBox(_T("子窗口的句柄为空！"));
		m_RankDlg->ShowWindow(SW_SHOW);
		m_RankDlg->MoveWindow(100, 100,500,500);
		m_RankDlg->SendMessage(WM_RANK_MSG, (WPARAM)&m_MessageToRankDlg);*/


}

//----------------------MIDI CODE------------------------//
void CProjectDlg::m_LinkDevice()
{
	if (midi::CMIDIOutDevice::GetNumDevs() > 0)
	{
		m_OutDevice.Open(0);
		MessageBox("Midi output device ready");
	}
	else MessageBox("No midi output device avaliable", "Warning", MB_ICONWARNING | MB_OK);
	if (midi::CMIDIInDevice::GetNumDevs() > 0)
	{

		m_InDevice.SetReceiver(*this);
		m_InDevice.Open(0);

		m_InDevice.StartRecording();
		MessageBox("Midi input device ready.Start recording");
	}
	else MessageBox("No midi input device avaliable", "Warning", MB_ICONWARNING | MB_OK);


}

void CProjectDlg::OnNoteOn(unsigned char note)
{
	midi::CShortMsg m_ShortMsg(midi::NOTE_ON, 0, note, 127, 0);
	m_ShortMsg.SendMsg(m_OutDevice);
}

void CProjectDlg::OnNoteOff(unsigned char note)
{
	midi::CShortMsg m_ShortMsg(midi::NOTE_OFF, 0, note, 0, 0);
	m_ShortMsg.SendMsg(m_OutDevice);
}

void CProjectDlg::ReceiveMsg(DWORD Msg, DWORD TimeStamp)
{
	midi::CShortMsg m_ShortMsg(Msg, TimeStamp);
	unsigned char m_Command = m_ShortMsg.GetCommand();
	unsigned char m_Note = m_ShortMsg.GetData1();
	int m_key = m_ShortMsg.GetData1();
	if (m_Command == midi::NOTE_OFF || (m_ShortMsg.GetData2() == 0 && m_Command == midi::NOTE_ON))
	{
		if (m_Note >= LOW_PITCH && m_Note <= HIGH_PITCH)
		{
			m_MIDIErrorLock = 0;
			m_BlockControl.m_KeyUp(m_Note);
			m_PianoKeyboard.m_KeyUp(m_Note);
			m_PowerLow.m_KeyUp(m_Note);
			//m_Keyup(m_key);
			//MessageBox("Off");
			//OnNoteOff(m_Note);
			if (m_Model == CHALLENGE_MODEL)
			{
				UpdateScore(m_Scores[0]);
			}
			// Don't Put this reset in WM_KEYDOWN !!
			// Reset Return CountDown
			m_ReturnCountDown = RETURN_COUNTDOWN_TIME;

		}
		else if (0 == m_MIDIErrorLock)
		{
			m_MIDIErrorLock = 1;
			MessageBox("Picth out of range", "Error", MB_ICONWARNING | MB_OK);
		}
	}
	else if (m_Command == midi::NOTE_ON&& m_ShortMsg.GetData2() > 0)
	{
		if (m_Note >= LOW_PITCH && m_Note <= HIGH_PITCH)
		{
			bool Buffer; // Used to decide what color
			// the piano key should be 
			m_MIDIErrorLock = 0;
			Buffer = m_BlockControl.m_KeyDown(m_Note);
			m_PianoKeyboard.m_KeyDown(m_Note, Buffer);
			m_PowerLow.m_KeyDown(m_Note);
			//m_Keydown(m_key);
			//MessageBox("On");
			//OnNoteOn(m_Note);
		}
		else if (0 == m_MIDIErrorLock)
		{
			m_MIDIErrorLock = 1;
			MessageBox("Picth out of range", "Error", MB_ICONWARNING | MB_OK);
		}
	}
	// IF you need error command warning,Open this
	//else if (0 == m_ErrorLock)
	//{
	//	m_ErrorLock = 1;
	//	MessageBox("Unknown Command", "Error", MB_ICONWARNING | MB_OK);
	//}
}

void CProjectDlg::OnError(DWORD Msg, DWORD TimeStamp)
{
	MessageBox("An illegal MIDI short message was received.", "Error", MB_ICONSTOP | MB_OK);
}
//----------------------MIDI CODE------------------------//


//设置背景字体、颜色
HBRUSH CProjectDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH   hBrush = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);


	//设置导航栏功能字体
	if ((pWnd->GetDlgCtrlID() == IDC_FREE_PLAY) || (pWnd->GetDlgCtrlID() == IDC_OPEN) || (pWnd->GetDlgCtrlID() == IDC_RETURN) || (pWnd->GetDlgCtrlID() == IDB_SCORE)
		|| (pWnd->GetDlgCtrlID() == IDC_SPEED_CONTROL_1) || (pWnd->GetDlgCtrlID() == IDC_SPEED_CONTROL_2) || (pWnd->GetDlgCtrlID() == IDC_SPEED_CONTROL_3)) {

		//得字体设置
		font = new CFont();
		pDC->SetBkMode(TRANSPARENT);
		//pDC->SetBkColor(RGB(0, 238, 0));
		//设置默认颜色

		//设置自由弹奏默认高亮显示
		if (pWnd->GetDlgCtrlID() == free_play)
		{
			pDC->SetTextColor(RGB(218, 165, 32));
		}
		else
		{
			pDC->SetTextColor(RGB(93, 137, 13));
			//pDC->SetTextColor(RGB(189, 183, 107));
		}
		//pDC->SetTextColor(RGB(189, 183, 107));

		//设置导航栏按钮点击变色
		if (pWnd->GetDlgCtrlID() == navigation_flag_current)
		{
			pDC->SetTextColor(RGB(218, 165, 32));
		}


		font->CreatePointFont(360, "华文行楷", NULL);
		pDC->SelectObject(font);

		delete font;

		return   (HBRUSH)::GetStockObject(NULL_BRUSH);
	}

	//设置创作模式下圆周率、自然数、无理数的字体
	if ((pWnd->GetDlgCtrlID() == IDC_CYCLERATIO) || (pWnd->GetDlgCtrlID() == IDC_NATURALNUM)
		|| (pWnd->GetDlgCtrlID() == IDC_IRRATIONALNUM)) {
		//得字体设置
		font = new CFont();
		pDC->SetBkMode(TRANSPARENT);
		//pDC->SetBkColor(RGB(0, 238, 0));
		//设置默认颜色
		pDC->SetTextColor(RGB(93, 137, 13));

		if (pWnd->GetDlgCtrlID() == navigation_flag_current)
		{
			pDC->SetTextColor(RGB(218, 165, 32));
		}

		font->CreatePointFont(285, "华文行楷", NULL);
		pDC->SelectObject(font);

		delete font;

		return   (HBRUSH)::GetStockObject(NULL_BRUSH);
	}

	//设置了解更多一栏的字体大小
	if (pWnd->GetDlgCtrlID() == IDC_LEARNMORE) {
		font = new CFont();
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetBkColor(RGB(0, 238, 0));
		pDC->SetTextColor(RGB(189, 183, 107));

		font->CreatePointFont(240, "华文行楷", NULL);
		pDC->SelectObject(font);

		delete font;

		return   (HBRUSH)::GetStockObject(NULL_BRUSH);
	}

	//设置钢琴键盘do位置的字体和颜色
	if ((pWnd->GetDlgCtrlID() == IDC_DO_1) || (pWnd->GetDlgCtrlID() == IDC_DO_2)
		|| (pWnd->GetDlgCtrlID() == IDC_DO_3) || (pWnd->GetDlgCtrlID() == IDC_DO_4)
		|| (pWnd->GetDlgCtrlID() == IDC_DO_5) || (pWnd->GetDlgCtrlID() == IDC_DO_6)
		|| (pWnd->GetDlgCtrlID() == IDC_DO_7) || (pWnd->GetDlgCtrlID() == IDC_DO_8)) {
		font = new CFont();
		pDC->SetBkMode(TRANSPARENT);
		//pDC->SetBkColor(RGB(0, 238, 0));
		if (pWnd->GetDlgCtrlID() == IDC_DO_1) pDC->SetTextColor(RGB(141, 0, 127));
		if (pWnd->GetDlgCtrlID() == IDC_DO_2) pDC->SetTextColor(RGB(205, 96, 2));
		if (pWnd->GetDlgCtrlID() == IDC_DO_3) pDC->SetTextColor(RGB(255, 215, 0));
		if (pWnd->GetDlgCtrlID() == IDC_DO_4) pDC->SetTextColor(RGB(24, 174, 4));
		if (pWnd->GetDlgCtrlID() == IDC_DO_5) pDC->SetTextColor(RGB(3, 226, 196));
		if (pWnd->GetDlgCtrlID() == IDC_DO_6) pDC->SetTextColor(RGB(44, 87, 210));
		if (pWnd->GetDlgCtrlID() == IDC_DO_7) pDC->SetTextColor(RGB(90, 3, 101));
		if (pWnd->GetDlgCtrlID() == IDC_DO_8) pDC->SetTextColor(RGB(90, 3, 101));
		//pDC->SetTextColor(RGB(2, 105, 223));

		font->CreatePointFont(240, "楷体", NULL);
		pDC->SelectObject(font);

		delete font;

		return   (HBRUSH)::GetStockObject(NULL_BRUSH);
	}

	//设置得分字体
	if (pWnd->GetDlgCtrlID() == IDC_SCORE_TEXT) {
		//得字体设置
		font = new CFont();
		//pDC->SetBkMode(TRANSPARENT);
		//pDC->SetBkColor(RGB(34, 139, 34));
		pDC->SetBkColor(RGB(247, 252, 222));
		pDC->SetTextColor(RGB(218, 165, 32));
		//pDC->SetTextColor(RGB(189, 183, 107));

		font->CreatePointFont(360, "楷体", NULL);
		pDC->SelectObject(font);

		delete font;

		return   (HBRUSH)::GetStockObject(NULL_BRUSH);
	}

	return   (HBRUSH)::GetStockObject(NULL_BRUSH);
	//return hBrush;
}

//当单击“打开”时的功能
void CProjectDlg::OnStnClickedOpen()
{
	//必须放在restart函数之前，否则restart会重置
	// navigation_flag_current的值
	// 将点亮切换到open
	int buf = navigation_flag_current;
	navigation_flag_before = buf;
	navigation_flag_current = IDC_OPEN;
	Show_NavigationClicked(navigation_flag_before);
	Show_NavigationClicked(navigation_flag_current);

	m_State = FALSE;
	CSelectDlg m_SelectDlg;
	m_SelectDlg.m_Initialize("", 0);
	m_SelectDlg.DoModal();
	UpdateData(TRUE);
	m_SelectDlg.DestroyWindow();

	//将open按键不点亮
	Show_NavigationClicked(IDC_OPEN);
	// Need to change
	/*
	bool m_SelectDlgReturn;
	if (m_SelectDlgReturn == IDOK)
	{
		// Restart
		m_Restart();
	}
	else if (m_SelectDlgReturn == IDCANCEL)
	{
		//重置导航栏按钮点击状态
		navigation_flag_current = navigation_flag_before;
	}
	//将open按键不点亮
	Show_NavigationClicked(IDC_OPEN);
	//显示默认的普通速度
	Show_NavigationClicked(navigation_flag_current);

	if (m_SelectDlgReturn == IDOK)
	{
		// Reset CountDown
		m_CountDownCounter = COUNTERDOWN_TIME;
		m_CountDownLock = FALSE;
	}
	*/
	//m_State = TRUE;
	//MessageBox("HI");


}

//当单击“返回”时的功能
void CProjectDlg::OnStnClickedReturn()
{
	m_State = FALSE;
	CreateTxetFile();

	// Restart
	m_Restart();

	CWelcomeDlg m_WelcomeDlg;
	INT_PTR m_WelcomeDlgReturn;
	m_WelcomeDlgReturn = m_WelcomeDlg.DoModal();
	UpdateData(TRUE);
	m_WelcomeDlg.DestroyWindow();
	Show_Model(m_Model);
	// To avoid the music array isn't emptied when you 
	// enter the Create Mode in the second time
	m_BlockControl.m_ChangeCreateModelOption(CREATE_MODEL_FREEPLAY);

	// PopUp a SelectDlg after the user choose the Model
	if (m_Model == CHALLENGE_MODEL || m_Model == LEARNING_MODEL)
	{
		OnStnClickedOpen();
	}

	m_State = TRUE;
}

void CProjectDlg::Show_Model(int model) {

	if (model == CHALLENGE_MODEL)
	{
		GetDlgItem(IDC_OPEN)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SPEED_CONTROL_1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SPEED_CONTROL_2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SPEED_CONTROL_3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDB_SCORE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SCORE_TEXT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_RETURN)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_LEARNMORE)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_CYCLERATIO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_NATURALNUM)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_IRRATIONALNUM)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_FREE_PLAY)->ShowWindow(SW_HIDE);

	}
	if (m_Model == LEARNING_MODEL) {
		GetDlgItem(IDC_OPEN)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SPEED_CONTROL_1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SPEED_CONTROL_2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SPEED_CONTROL_3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDB_SCORE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SCORE_TEXT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RETURN)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_LEARNMORE)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_CYCLERATIO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_NATURALNUM)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_IRRATIONALNUM)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_FREE_PLAY)->ShowWindow(SW_HIDE);
	}

	if (m_Model == CREATE_MODEL)
	{
		GetDlgItem(IDC_OPEN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPEED_CONTROL_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPEED_CONTROL_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPEED_CONTROL_3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDB_SCORE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SCORE_TEXT)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_RETURN)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_CYCLERATIO)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_NATURALNUM)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_IRRATIONALNUM)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_FREE_PLAY)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_LEARNMORE)->ShowWindow(SW_HIDE);
	}
}

void CProjectDlg::OnStnClickedLearnmore()
{
	m_State = FALSE;
	CLearnMoreDlg m_LearnMoreDlg;
	INT_PTR m_LearnMoreDlgReturn;
	m_LearnMoreDlgReturn = m_LearnMoreDlg.DoModal();
	UpdateData(TRUE);
	m_State = TRUE;
	m_LearnMoreDlg.DestroyWindow();
}


void CProjectDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}

//点击自由弹奏按钮
void CProjectDlg::OnStnClickedFreePlay()
{
	m_State = FALSE;

	//必须放在restart函数之前，否则restart会重置
	// navigation_flag_current的值
	int buf = navigation_flag_current;

	m_BlockControl.m_ChangeCreateModelOption(CREATE_MODEL_FREEPLAY);
	m_Restart();

	free_play = IDC_FREE_PLAY;//关闭自由弹奏高亮显示
	navigation_flag_before = buf;
	navigation_flag_current = IDC_FREE_PLAY;
	Show_NavigationClicked(navigation_flag_before);
	Show_NavigationClicked(navigation_flag_current);

	// Function m_Restart() had clear the Number in ScoreText
	m_State = TRUE;
}

void CProjectDlg::OnStnClickedCycleratio()
{
	m_State = FALSE;
	//必须放在restart函数之前，否则restart会重置
	// navigation_flag_current的值
	int buf = navigation_flag_current;

	m_BlockControl.m_ChangeCreateModelOption(CREATE_MODEL_PIE);
	m_Restart();

	free_play = 0;//关闭自由弹奏高亮显示
	navigation_flag_before = buf;
	navigation_flag_current = IDC_CYCLERATIO;
	GetDlgItem(IDC_FREE_PLAY)->SetWindowTextA("自由弹奏");//更新自由弹奏颜色
	Show_NavigationClicked(navigation_flag_before);
	Show_NavigationClicked(navigation_flag_current);


	// Set the Number of pie
	m_CreateModelNumber = "3141592 π    ";
	m_ScoreText.ResetContent();
	m_ScoreText.AddString(m_CreateModelNumber);

	m_State = TRUE;
}


void CProjectDlg::OnStnClickedNaturalnum()
{
	m_State = FALSE;

	//必须放在restart函数之前，否则restart会重置
	// navigation_flag_current的值
	int buf = navigation_flag_current;

	m_BlockControl.m_ChangeCreateModelOption(CREATE_MODEL_E);
	m_Restart();

	free_play = 0;//关闭自由弹奏高亮显示
	navigation_flag_before = buf;
	navigation_flag_current = IDC_NATURALNUM;
	GetDlgItem(IDC_FREE_PLAY)->SetWindowTextA("自由弹奏");//更新自由弹奏颜色
	Show_NavigationClicked(navigation_flag_before);
	Show_NavigationClicked(navigation_flag_current);

	// Set the Number of e
	m_CreateModelNumber = "2718281 e    ";
	m_ScoreText.ResetContent();
	m_ScoreText.AddString(m_CreateModelNumber);

	m_State = TRUE;
}


void CProjectDlg::OnStnClickedIrrationalnum()
{
	m_State = FALSE;

	//必须放在restart函数之前，否则restart会重置
	// navigation_flag_current的值
	int buf = navigation_flag_current;

	m_BlockControl.m_ChangeCreateModelOption(CREATE_MODEL_SQUARE2);
	m_Restart();

	free_play = 0;//关闭自由弹奏高亮显示
	navigation_flag_before = buf;
	navigation_flag_current = IDC_IRRATIONALNUM;
	GetDlgItem(IDC_FREE_PLAY)->SetWindowTextA("自由弹奏");//更新自由弹奏颜色
	Show_NavigationClicked(navigation_flag_before);
	Show_NavigationClicked(navigation_flag_current);

	// Set the Number of square 2
	m_CreateModelNumber = "1414213√2     ";
	m_ScoreText.ResetContent();
	m_ScoreText.AddString(m_CreateModelNumber);

	m_State = TRUE;
}

void  CProjectDlg::Show_NavigationClicked(int kind) {
	CString flag;

	switch (kind)
	{
	case IDC_OPEN:flag.Format("%s", "选曲"); break;
	case IDC_SPEED_CONTROL_1:flag.Format("%s", "初级"); break;
	case IDC_SPEED_CONTROL_2:flag.Format("%s", "中级"); break;
	case IDC_SPEED_CONTROL_3:flag.Format("%s", "高级"); break;
	case IDC_CYCLERATIO:flag.Format("%s", "弹π"); break;
	case IDC_NATURALNUM:flag.Format("%s", "弹e"); break;
	case IDC_IRRATIONALNUM:flag.Format("%s", "弹√2"); break;
	case IDC_FREE_PLAY:flag.Format("%s", "自由弹奏"); break;
	}
	GetDlgItem(kind)->SetWindowTextA(flag);

}


void CProjectDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_ReturnCountDown = RETURN_COUNTDOWN_TIME;
	__super::OnLButtonUp(nFlags, point);
}
