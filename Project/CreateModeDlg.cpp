
// ProjectDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Project.h"
#include "CreateModeDlg.h"
#include "afxdialogex.h"

// The head file of function timeSetEvent()
#include <windows.h>
#pragma comment(lib,"Winmm.lib")
#include <MMSystem.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CProjectDlg �Ի���
// Global Function that used to be ReCall
void PASCAL m_BlockDroppingTimer_CreateMode(UINT wTimerID, UINT msg, DWORD dwUser, DWORD dwl, DWORD dw2)
{
	CCreateModeDlg *ptr = (CCreateModeDlg*)dwUser;
	if (ptr->m_CanStart())
	{
		// Drop blocks
		ptr->m_CallBlockDropping();
		////Destroy blocks
		//ptr->m_CallBlockDestroy();
		ptr = NULL;
	}
}

void PASCAL m_BlockCreateTimer_CreateMode(UINT wTimerID, UINT msg, DWORD dwUser, DWORD dwl, DWORD dw2)
{
	CCreateModeDlg *ptr = (CCreateModeDlg*)dwUser;
	if (ptr->m_CanStart())
	{
		// Drop blocks
		ptr->m_CallBlockCreate();
		ptr = NULL;
	}
}

CCreateModeDlg::CCreateModeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CREATEMODE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CCreateModeDlg::~CCreateModeDlg()
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
}

void CCreateModeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_SPEED_SLIDER, m_SpeedSlider);

	//��������ٶȿ��ƣ��򵥡���ͨ������
	//�˴�Ϊ�������ݸ���
	/*DDX_Control(pDX, IDC_SPEED_CONTROL_1, m_SpeedSlider);
	DDX_Control(pDX, IDC_SPEED_CONTROL_2, m_SpeedSlider);
	DDX_Control(pDX, IDC_SPEED_CONTROL_3, m_SpeedSlider);*/

	DDX_Control(pDX, IDC_RETURN, m_Buffer);
	DDX_Control(pDX, IDC_SCORE_TEXT, m_ScoreText);
}

BEGIN_MESSAGE_MAP(CCreateModeDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_CREATE()

	//	ON_WM_KEYDOWN()
	//	ON_WM_KEYUP()
	//ON_BN_CLICKED(IDC_BUTTON_SELECT, &CProjectDlg::OnBnClickedButtonSelect)
	//ON_BN_CLICKED(IDC_BUTTON_LEARNMORE, &CProjectDlg::OnBnClickedButtonLearnMore)
	//ON_BN_CLICKED(IDC_BUTTON_RETURN, &CProjectDlg::OnBnClickedButtonReturn)
	


	ON_MESSAGE(WM_SELECT, &CCreateModeDlg::OnSelect)
	ON_MESSAGE(WM_MODE, &CCreateModeDlg::OnModel)
	ON_WM_DRAWITEM()

	//ע������¼�

	/*
	��ͼƬ��ʽ�Ĵ򿪡����أ��Լ��ٶȵ���ȫ������Static Text
	*���ñ���͸��
	*/
	// ON_STN_CLICKED(IDC_OPEN, &CProjectDlg::OnClickedPictureOpen)
	//ON_STN_CLICKED(IDC_PICTURE_LEARNMORE, &CProjectDlg::OnClickedPictureLearnmore)
	//ON_STN_CLICKED(IDC_PICTURE_RETURN, &CProjectDlg::OnClickedPictureReturn)

	ON_STN_CLICKED(IDC_OPEN, &CCreateModeDlg::OnStnClickedOpen)
	ON_STN_CLICKED(IDC_RETURN, &CCreateModeDlg::OnStnClickedReturn)
	ON_STN_CLICKED(IDC_LEARNMORE, &CCreateModeDlg::OnStnClickedLearnmore)

	ON_STN_CLICKED(IDC_SPEED_CONTROL_1, &CCreateModeDlg::OnClickedSpeedControl_1)
	ON_STN_CLICKED(IDC_SPEED_CONTROL_2, &CCreateModeDlg::OnClickedSpeedControl_2)
	ON_STN_CLICKED(IDC_SPEED_CONTROL_3, &CCreateModeDlg::OnClickedSpeedControl_3)

	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, &CCreateModeDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CProjectDlg ��Ϣ�������

BOOL CCreateModeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	////  ִ�д˲���
	//SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	//SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	//counter = 0;

	////���õ÷���
	//m_ScoreText.ResetContent();
	//m_ScoreText.AddString("0                   ");

	//ShowWindow(SW_MAXIMIZE);
	//// Set the size of Windows
	////int x = ::GetSystemMetrics(SM_CXSCREEN);
	////int y = ::GetSystemMetrics(SM_CYSCREEN);
	////::MoveWindow(this->m_hWnd, 0, 0, x, y, true);
	////TODO: �ڴ���Ӷ���ĳ�ʼ������

	//// My Code
	//// Create MaxT.out
	//CreateTxetFile();
	//// Initialize State / Stop
	//m_State = FALSE;
	//// Get Client Size
	//GetClientRect(&m_client);

	//// Initialize Objects
	//m_Background.m_SetPosition(m_client);
	//m_Stave.m_Initialize(m_client);
	//m_PowerLow.m_Initialize(m_client);
	//m_Title.m_SetPosition(m_client);
	//m_PianoKeyboard.m_Initialize(m_client);
	//m_Ruler.m_Initialize(m_client);
	//m_BlockLength = m_BlockControl.m_Initialize(m_client, m_PianoKeyboard);

	//m_BlockCreateTimeHandle = NULL;
	//m_BlockDroppingTimeHandle = NULL;

	//// Show Welcome Dialog
	////Show Welcome Dialog must be at this place or may cause problems
	//CWelcomeDlg m_WelcomeDlg;
	//INT_PTR m_WelcomeDlgReturn;
	//m_WelcomeDlgReturn = m_WelcomeDlg.DoModal();
	//if (m_WelcomeDlgReturn == IDCANCEL) { exit(0); }
	//else m_WelcomeDlg.DestroyWindow();

	//// Set Bottoms Position

	//// m_BottomPosition �������ĸ���ť��Position
	//m_BottomPosition.top = (int)m_client.bottom * BOTTOM_TOP;
	//m_BottomPosition.bottom = (int)m_client.bottom * BOTTOM_BOTTON;

	//// Move My Bottoms
	//m_MoveBottoms();

	//// Set Slider
	//m_SpeedSlider.SetRange(100, 300);
	//m_SpeedSlider.SetTicFreq(100);
	//// Set Slider Lock
	//m_SliderLock = TRUE;

	//// Set CountDown Position
	//m_CountDownPosition.top = (int)m_client.bottom*0.3;
	//m_CountDownPosition.bottom = (int)m_client.bottom*0.7;
	//m_CountDownPosition.left = (int)m_client.right *0.4;
	//m_CountDownPosition.right = (int)m_client.right *0.6;
	//// Set CountDownCounter
	//m_CountDownCounter = COUNTERDOWN_TIME;

	////Set perfect
	//int perfect_flag = 0;
	//int perfect_flag_2 = 0;
	//int perfect_counter = COUNTERDOWN_STEP / 5;

	//// Set Score
	//for (int counter = 0; counter < 5; counter++)
	//{
	//	m_Scores[counter] = 0;
	//}

	//// Attetion: LinkMIDIDevice Must be put behind Set Timer 
	////			 or will cause timer is not accurate
	//// Initialize MIDIErrorLock
	//m_MIDIErrorLock = FALSE;
	//// Link MIDI Device
	//m_LinkDevice();
	//// To Pass Model into Objects
	//// and Reset the Moving Items
	//m_Restart();
	//// Change State to Start
	////m_State = TRUE;

	////����PerfectͼƬ
	//m_PerfectPicture.Load("res\\perfect.png");

	//// Must be Put behind m_LinkDevice
	//// Because Messagebox can left time for Initialize
	//// m_Slider will auto call m_SliderEvent to change m_State = TRUE
	//// when Initialize,so we need to change m_State = FALSE again
	//m_State = FALSE;
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ� 

//������Ϸ������
void CCreateModeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);


	}
	/*
	else{}���  ��ͼ���
	*/
	else
	{
		m_BlockControl.m_ChangeOtherObjectPosition(m_Stave, m_Ruler);
		// Create CacheDC
		CDC *cDC = this->GetDC();
		m_cacheDC.CreateCompatibleDC(NULL);
		m_cacheCBitmap.CreateCompatibleBitmap(cDC, m_client.Width(), m_client.Height());
		m_cacheDC.SelectObject(&m_cacheCBitmap);

		//����սģʽ���������
		if (m_Model == CHALLENGE_MODEL)
		{
			// �޸Ŀ���ʧʱ��λ��
			perfect_flag = m_BlockControl.m_BlockDestroy(m_PowerLow);
			m_CalculateScore(perfect_flag);
			//UpdataScore(m_Scores[0]);
			UpdataScore(counter);

		}
		m_Background.m_Print(m_cacheDC);
		m_BlockControl.m_Print(m_cacheDC);
		m_PowerLow.m_Print(m_cacheDC);
		m_Stave.m_Print(m_cacheDC);
		m_Ruler.m_Print(m_cacheDC);
		//m_NumberedMusicNotation.m_Print(m_cacheDC);
		m_PianoKeyboard.m_Print(m_cacheDC);

		//����Title
		m_Title.m_Print(m_cacheDC);

		//��Perfect
		//OnPaint()����Ϊһ��ʱ�����ڣ�ÿ�ι̶�ʱ��ˢ�¡�������ʱ��ֵҪ�ӳ�һ��ʱ����ˢ�´���
		//�޸�ʱ�䣺2016��6��15��
		m_Perfect(m_cacheDC);

		// CountDown when nessary
		//m_CountDown : 3��2��1����ʱ
		if ((!m_CountDownLock) && m_State) { m_CountDown(m_cacheDC); }

		// Print on Screen
		cDC->BitBlt(0, 0, m_client.Width(), m_client.Height(), &m_cacheDC, 0, 0, SRCCOPY); \

			// Adjust m_Slider to achieve classification
			if (m_SliderLock) { m_AdjustSilder(); }

		// Release 
		ValidateRect(&m_client);
		//Invalidate(FALSE);
		m_cacheDC.DeleteDC();
		m_cacheCBitmap.DeleteObject();
		ReleaseDC(cDC);
		//CDialogEx::OnPaint();
	}
}

void CCreateModeDlg::m_CalculateScore(int m_Input)
{
	// m_Input == 0 means invalid input
	if (m_Input == 0)return;
	// Vital statistics of all levels of grades	
	m_Scores[m_Input] ++;
	// Calc the total score

	//����Ϊԭ�汾
	//if((m_Input * 20 - 30) > 0)                
	//m_Scores[0] += (m_Input * 20 - 30);

	//��Ϊ�¸Ķ�����,��Ϊ���������,�һ�������10����
	//���ȣ��ж�m_Input���������ͣ�������Ӧ���ͼ���÷�

	//Miss���÷�
	if (m_Input == 1) {
		m_Scores[0] += 0;
	}
	//һ��Good��100��
	if (m_Input == 2) {
		m_Scores[0] += 100;
	}
	//һ��Great��200��
	if (m_Input == 3) {
		m_Scores[0] += 200;
	}
	//һ��Perfect��500��
	if (m_Input == 4) {
		m_Scores[0] += 500;
	}
}

void CCreateModeDlg::UpdataScore(int score) {
	score_buf.Format("%d", score);
	m_ScoreText.ResetContent();
	m_ScoreText.AddString(score_buf);
}

void CCreateModeDlg::m_CallBlockDropping()
{
	m_BlockControl.m_BlockDropping();
}

void CCreateModeDlg::m_CallBlockCreate()
{
	m_BlockControl.m_BlockCreate();
}


//�������ĸ���ť�Ľ��沼��
void CCreateModeDlg::m_MoveBottoms()
{
	m_BottomPosition.left = m_client.right * 0.15;
	m_BottomPosition.right = m_client.right * 0.23;
	GetDlgItem(IDC_RETURN)->MoveWindow(m_BottomPosition);

	m_BottomPosition.left = m_client.right * 0.24;
	m_BottomPosition.right = m_client.right * 0.32;
	GetDlgItem(IDC_CYCLERATIO)->MoveWindow(m_BottomPosition);

	m_BottomPosition.left = m_client.right * 0.35;
	m_BottomPosition.right = m_client.right * 0.43;
	GetDlgItem(IDC_NATURALNUM)->MoveWindow(m_BottomPosition);

	m_BottomPosition.left = m_client.right * 0.42;
	m_BottomPosition.right = m_client.right * 0.50;
	GetDlgItem(IDC_IRRATIONALNUM)->MoveWindow(m_BottomPosition);

	m_BottomPosition.left = m_client.right * 0.49;
	m_BottomPosition.right = m_client.right * 0.57;
	GetDlgItem(IDC_GOLDNUM)->MoveWindow(m_BottomPosition);

	/*m_BottomPosition.left = m_client.right * 0.60;
	m_BottomPosition.right = m_client.right * 0.70;
	GetDlgItem(IDB_SCORE)->MoveWindow(m_BottomPosition);

	m_BottomPosition.left = m_client.right * 0.68;
	m_BottomPosition.right = m_client.right * 0.85;
	m_BottomPosition.bottom = m_BottomPosition.bottom*0.90;
	GetDlgItem(IDC_SCORE_TEXT)->MoveWindow(m_BottomPosition);

	m_BottomPosition.left = m_client.right * 0.80;
	m_BottomPosition.right = m_client.right * 0.98;
	GetDlgItem(IDC_LEARNMORE)->MoveWindow(m_BottomPosition);*/

	//���������սģʽ������ʾ�÷���
	//if (m_Model != 1)
	//{
	//	GetDlgItem(IDB_SCORE)->ShowWindow(FALSE);
	//	GetDlgItem(IDC_SCORE_TEXT)->ShowWindow(FALSE);

	//}
	//else {
	//	GetDlgItem(IDB_SCORE)->ShowWindow(TRUE);
	//	GetDlgItem(IDC_SCORE_TEXT)->ShowWindow(TRUE);
	//}
}

/*
��Perfect
*/
void CCreateModeDlg::m_Perfect(CDC& m_cacheDC)
{
	if (perfect_flag == 4) {
		perfect_flag_2 = perfect_flag;
	}
	if ((perfect_flag_2 == 4) && (perfect_counter != 0)) {
		m_PerfectPicture.Draw(m_cacheDC, m_CountDownPosition);
		perfect_counter--;
	}
	else {
		perfect_counter = COUNTERDOWN_STEP / 5;
		perfect_flag_2 = perfect_flag;
	}
}

void CCreateModeDlg::m_CountDown(CDC& m_cacheDC)
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

void CCreateModeDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

//���̰�����Ӧ�¼�
BOOL CCreateModeDlg::PreTranslateMessage(MSG* pMsg)
{

	//TODO: �ڴ����ר�ô����/����û���
	if (pMsg->wParam >= LOW_PITCH && pMsg->wParam < HIGH_PITCH)
	{
		//���¼�ʱ�����
		if (pMsg->message == WM_KEYDOWN)
		{
			// Check whether the data is Zero
			m_BlockControl.m_KeyDown(pMsg->wParam);//���ݼ��̶�Ӧ��ASCII��
			m_PianoKeyboard.m_KeyDown(pMsg->wParam);
			//ͳ������
			m_PowerLow.m_KeyDown(pMsg->wParam);

			return TRUE;


		}
		else if (pMsg->message == WM_KEYUP)
		{
			m_BlockControl.m_KeyUp(pMsg->wParam);
			m_PianoKeyboard.m_KeyUp(pMsg->wParam);
			m_PowerLow.m_KeyUp(pMsg->wParam);

			//��ʾ�÷�
			//���ɷ���WM_KEYDOWN�У�
			//UpdataScore(m_Scores[0]);
			
			UpdataScore(counter);

			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
//
//void CCreateModeDlg::OnNMCustomdrawSpeedSlider(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	m_SliderLock = TRUE;
//	*pResult = 0;
//}

//m_SliderEvent(int speedRate)���������м򵥡���ͨ�����������ٶȵ���
//�޸�ʱ�䣺2016��6��16��
void CCreateModeDlg::m_SliderEvent(int kind)
{
	// Get Current Slider Position
	//int m_SpeedRate = (int)(400.0 - m_SpeedSlider.GetPos()) / 50.0;//֮ǰ����Ϊ����100.0
	int delay_create = m_CreateRefreshTime;
	int delay_drop = m_DropRefreshTime;
	if (kind == 1) {
		//delay_create = m_CreateRefreshTime >> 1;//1
		//delay_drop = m_DropRefreshTime >> 1;//1
		delay_create = m_CreateRefreshTime / 8;//1
		delay_drop = m_DropRefreshTime / 8;//1
	}
	else if (kind == 2) {
		delay_create = m_CreateRefreshTime;
		delay_drop = m_DropRefreshTime;
	}
	else if (kind == 3) {
		delay_create = m_CreateRefreshTime << 1;
		delay_drop = m_DropRefreshTime << 1;
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
	if (m_Model != CREATE_MODEL)
		// Set New TimerEvent
		m_BlockCreateTimeHandle = timeSetEvent(delay_create, TIMING_ACCURACY, (LPTIMECALLBACK)m_BlockCreateTimer_CreateMode, (DWORD_PTR)this, TIME_PERIODIC);
	m_BlockDroppingTimeHandle = timeSetEvent(delay_drop, TIMING_ACCURACY, (LPTIMECALLBACK)m_BlockDroppingTimer_CreateMode, (DWORD_PTR)this, TIME_PERIODIC);
	// Set Lock
	m_SliderLock = FALSE;
	// Start Again
	m_State = TRUE;
	// Test Code---------------------------------------
	//CString str;
	//str.Format("%d", m_SpeedRate);
	//GetDlgItem(IDC_BUTTON_LEARNMORE)->SetWindowText(str);
	//-------------------------------------------------

}

//�˲��ִ���������
//void CCreateModeDlg::m_AdjustSilder()
//{
//	 Set Slider Position to achieve the Quantisation of scale of Slider
//	int m_CurrentSliderScale = m_SpeedSlider.GetPos();
//	if (m_CurrentSliderScale >= 100 && m_CurrentSliderScale <= 150)
//		m_SpeedSlider.SetPos(100);
//	else if (m_CurrentSliderScale <= 250)
//		m_SpeedSlider.SetPos(200);
//	else if (m_CurrentSliderScale <= 300)
//		m_SpeedSlider.SetPos(300);
//	m_SliderEvent(1);
//}

afx_msg LRESULT CCreateModeDlg::OnSelect(WPARAM wParam, LPARAM lParam)
{
	MessageBox(*(CString*)wParam);
	return 0;
}

afx_msg LRESULT CCreateModeDlg::OnModel(WPARAM wParam, LPARAM lParam)
{
	//MessageBox(*(CString*)wParam);
	m_Model = *(int*)wParam;
	return 0;
}



//-----------------Hardly Changed Code------------------//

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CCreateModeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

bool CCreateModeDlg::m_CanStart()
{
	return (m_State&&m_CountDownLock);
}

// This function is Abandoned
void CCreateModeDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (m_State)
		switch (nIDEvent)
		{
		case TIMER_PRINT:OnPaint(); break;
		}

	CDialogEx::OnTimer(nIDEvent);
}

int CCreateModeDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	SetTimer(TIMER_PRINT, REFRESH_TIME, NULL);
	return 0;
}

void CCreateModeDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CCreateModeDlg::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnKeyUp(nChar, nRepCnt, nFlags);
}


void CCreateModeDlg::OnClickedSpeedControl_1()
{
	m_SliderEvent(3);
}

void CCreateModeDlg::OnClickedSpeedControl_2()
{
	m_SliderEvent(2);
}

void CCreateModeDlg::OnClickedSpeedControl_3()
{
	m_SliderEvent(1);
}


void CCreateModeDlg::m_Restart()
{

	//���õ÷���
	m_ScoreText.ResetContent();
	m_ScoreText.AddString("0                   ");

	m_CountDownCounter = COUNTERDOWN_TIME;
	m_CountDownLock = FALSE;
	// Reset CountDown
	if (m_Model == CHALLENGE_MODEL)
		m_CountDownCounter = COUNTERDOWN_TIME;
	m_CountDownLock = FALSE;
	// Reset Score
	for (int counter = 0; counter < 5; counter++)
	{
		m_Scores[counter] = 0;
	}
	// Get my RefreshTime
	//��������MIDIʱ�ĳ�ʼ�ٶ�
	//�޸ĺ�汾,�޸�ʱ�䣺2016��6��15��
	m_CreateRefreshTime = m_BlockControl.m_GetText() * 4;
	// Reset m_BlockControl
	m_BlockControl.m_Reset(m_Model);
	// Reset Stave
	m_Stave.m_Reset();
	// Reset Powerlow
	m_PowerLow.m_Reset(m_Model);
	// Reset Ruler
	m_Ruler.m_Initialize(m_client);
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

	if (m_Model == CREATE_MODEL)return;

	// Set Timer 
	// Warning :When the TimePeriod is smaller than the running time that is needed by Recallfunction
	//			timeSetEvent will go wrong
	m_BlockCreateTimeHandle = timeSetEvent(m_CreateRefreshTime, TIMING_ACCURACY, (LPTIMECALLBACK)m_BlockCreateTimer_CreateMode, (DWORD_PTR)this, TIME_PERIODIC);
	// Block_DROPPING_STEP is defiened in BlockControl.h
	m_DropRefreshTime = (int)(m_CreateRefreshTime*BLOCK_DROPPING_STEP / m_BlockLength);
	m_BlockDroppingTimeHandle = timeSetEvent(m_DropRefreshTime, TIMING_ACCURACY, (LPTIMECALLBACK)m_BlockDroppingTimer_CreateMode, (DWORD_PTR)this, TIME_PERIODIC);
}

void CCreateModeDlg::CreateTxetFile()
{
	CFile  m_TextFile("exe\\MaxT.out ", CFile::modeCreate);
	m_TextFile.Close();
}

//----------------------MIDI CODE------------------------//
void CCreateModeDlg::m_LinkDevice()
{
	//MessageBox("1");
	if (midi::CMIDIOutDevice::GetNumDevs() > 0)
	{
		//	//MessageBox("2");
		m_OutDevice.Open(0);
		MessageBox("Midi output device ready");
	}
	else MessageBox("No midi output device avaliable", "Warning", MB_ICONWARNING | MB_OK);
	//
	//MessageBox("3");
	if (midi::CMIDIInDevice::GetNumDevs() > 0)
	{
		//MessageBox("4");
		m_InDevice.SetReceiver(*this);
		//MessageBox("5");
		m_InDevice.Open(0);
		//MessageBox("6");
		m_InDevice.StartRecording();
		MessageBox("Midi input device ready.Start recording");
	}
	else MessageBox("No midi input device avaliable", "Warning", MB_ICONWARNING | MB_OK);
	//MessageBox("7");

}

void CCreateModeDlg::OnNoteOn(unsigned char note)
{
	midi::CShortMsg m_ShortMsg(midi::NOTE_ON, 0, note, 127, 0);
	m_ShortMsg.SendMsg(m_OutDevice);
}

void CCreateModeDlg::OnNoteOff(unsigned char note)
{
	midi::CShortMsg m_ShortMsg(midi::NOTE_OFF, 0, note, 0, 0);
	m_ShortMsg.SendMsg(m_OutDevice);
}

void CCreateModeDlg::ReceiveMsg(DWORD Msg, DWORD TimeStamp)
{
	midi::CShortMsg m_ShortMsg(Msg, TimeStamp);
	unsigned char m_Command = m_ShortMsg.GetCommand();
	unsigned char m_Note = m_ShortMsg.GetData1();
	int m_key = m_ShortMsg.GetData1();
	if (m_Command == midi::NOTE_OFF || (m_ShortMsg.GetData2() == 0 && m_Command == midi::NOTE_ON))
	{
		if (m_Note >= LOW_PITCH && m_Note < HIGH_PITCH)
		{
			m_MIDIErrorLock = 0;
			m_BlockControl.m_KeyUp(m_Note);
			m_PianoKeyboard.m_KeyUp(m_Note);
			m_PowerLow.m_KeyUp(m_Note);
			//m_Keyup(m_key);
			//MessageBox("Off");
			//OnNoteOff(m_Note);

		}
		else if (0 == m_MIDIErrorLock)
		{
			m_MIDIErrorLock = 1;
			MessageBox("Picth out of range", "Error", MB_ICONWARNING | MB_OK);
		}
	}
	else if (m_Command == midi::NOTE_ON&& m_ShortMsg.GetData2() > 0)
	{
		if (m_Note >= LOW_PITCH && m_Note < HIGH_PITCH)
		{
			m_MIDIErrorLock = 0;
			m_BlockControl.m_KeyDown(m_Note);
			m_PianoKeyboard.m_KeyDown(m_Note);
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

void CCreateModeDlg::OnError(DWORD Msg, DWORD TimeStamp)
{
	MessageBox("An illegal MIDI short message was received.", "Error", MB_ICONSTOP | MB_OK);
}

//���õ÷����塢������ɫ
HBRUSH CCreateModeDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	counter++;
	HBRUSH   hBrush = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	//counter++;

	//���õ�������������
	if ( (pWnd->GetDlgCtrlID() == IDC_RETURN)) {
		//����������
		font = new CFont();
		pDC->SetBkMode(TRANSPARENT);
		//pDC->SetBkColor(RGB(0, 238, 0));
		//����Ĭ����ɫ
		pDC->SetTextColor(RGB(0, 238, 0));

		font->CreatePointFont(120, "�����п�", NULL);
		pDC->SelectObject(font);

		return   (HBRUSH)::GetStockObject(NULL_BRUSH);
	}

	//�����˽����һ���������С
	if (pWnd->GetDlgCtrlID() == IDC_LEARNMORE) {
		//����������
		font = new CFont();
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetBkColor(RGB(0, 238, 0));
		pDC->SetTextColor(RGB(0, 238, 0));
		CFont *font = new CFont();

		font->CreatePointFont(240, "�����п�", NULL);
		pDC->SelectObject(font);

		return   (HBRUSH)::GetStockObject(NULL_BRUSH);
	}

	//���õ÷�����
	if (pWnd->GetDlgCtrlID() == IDC_SCORE_TEXT) {
		//����������
		font = new CFont();
		//pDC->SetBkMode(TRANSPARENT);
		pDC->SetBkColor(RGB(135, 206, 235));
		pDC->SetTextColor(RGB(255, 99, 71));

		font->CreatePointFont(360, "����", NULL);
		pDC->SelectObject(font);

		return   (HBRUSH)::GetStockObject(NULL_BRUSH);
	}

	return   (HBRUSH)::GetStockObject(NULL_BRUSH);
	//return hBrush;
}

//���������򿪡�ʱ�Ĺ���
void CCreateModeDlg::OnStnClickedOpen()
{
	m_State = FALSE;
	CSelectDlg m_SelectDlg;
	INT_PTR m_SelectDlgReturn;
	m_SelectDlgReturn = m_SelectDlg.DoModal();
	UpdateData(TRUE);
	m_SelectDlg.DestroyWindow();
	// Restart
	m_Restart();
	m_State = TRUE;
}

//�����������ء�ʱ�Ĺ���
void CCreateModeDlg::OnStnClickedReturn()
{
	m_State = FALSE;
	m_CountDownLock = FALSE;
	// Restart
	CreateTxetFile();
	m_Restart();

	CWelcomeDlg m_WelcomeDlg;
	INT_PTR m_WelcomeDlgReturn;
	m_WelcomeDlgReturn = m_WelcomeDlg.DoModal();
	if (m_WelcomeDlgReturn == IDCANCEL) { exit(0); }

	UpdateData(TRUE);
	m_WelcomeDlg.DestroyWindow();
	m_State = TRUE;
}


void CCreateModeDlg::OnStnClickedLearnmore()
{
	m_State = FALSE;
	CLearnMoreDlg m_LearnMoreDlg;
	INT_PTR m_LearnMoreDlgReturn;
	m_LearnMoreDlgReturn = m_LearnMoreDlg.DoModal();
	UpdateData(TRUE);
	m_State = TRUE;
	m_LearnMoreDlg.DestroyWindow();
}


void CCreateModeDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();
}