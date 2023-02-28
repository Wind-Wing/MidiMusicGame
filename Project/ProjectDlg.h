
// ProjectDlg.h : 头文件
//

#pragma once
#include"PianoKeyboard.h"
#include"Background.h"
#include"Stave.h"
#include"Title.h"
#include"BlockControl.h"
#include"PowerLow.h"
#include"Ruler.h"
#include "WelcomeDlg.h"
#include "SelectDlg.h"
#include "LearnMoreDlg.h"
#include "RankDlg.h"
#include "afxcmn.h"
#include "afxwin.h"
// MIDI Device
#include "midi.h"
#include "LongMsg.h"
#include "ShortMsg.h"
#include "MIDIInDevice.h"
#include "MIDIOutDevice.h"

// CProjectDlg 对话框
class CProjectDlg : public CDialogEx, public midi::CMIDIReceiver
{
	// 构造
public:
	CProjectDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CProjectDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROJECT_DIALOG };
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	// My Code
#define KEY_BOARD_BOTTOM 0.67//0.7
	// KEY_BOARD_POSITION is calculate from the keyboard top
#define KEY_BOARD_TOP 0.47//0.5

#define TIMER_PRINT 1//1
#define REFRESH_TIME 15//15
#define TIMING_ACCURACY 1//1
// They are also used in BlockControl.h and PianoKeyboard.h
#define LOW_PITCH 21//24
#define HIGH_PITCH 108//84
#define BOTTOM_TOP 0.008
#define BOTTOM_BOTTON 0.068
#define COUNTERDOWN_STEP 50//50
#define COUNTERDOWN_TIME 150//150
	// Three Models
#define CHALLENGE_MODEL 1
#define LEARNING_MODEL 2
#define CREATE_MODEL 3
	// Four Options of Create Model
#define CREATE_MODEL_FREEPLAY 0
#define CREATE_MODEL_PIE 1
#define	CREATE_MODEL_E 2
#define	CREATE_MODEL_SQUARE2 3
	// This value set the return time interval
#define RETURN_COUNTDOWN_TIME 1500
	// This value describe the max score and rank records
	// Also Define in RankDlg
#define RECORD_NUMBER 100000

public:
	int counter = 0;
	//设置字体
	CFont *font;
	CRankDlg RankDlg;
	INT_PTR RankDlgReturn;
	//设置导航栏标记，标记当前点击的导航栏按钮，用于控制颜色显示
	int navigation_flag_before;//上一次点击的按钮
	int navigation_flag_current;//当前点击按钮
	int free_play;//设置进入创作模式时默认自由弹奏高亮显示

	int m_BlockDroppingTimeHandle;
	int m_BlockCreateTimeHandle;

	int perfect_flag;
	int perfect_flag_2;
	int perfect_counter;

	//用于规则化得分 
	CString score_buf;
	//CRankDlg* m_RankDlg;
	// Function m_CallBlock() is used to avoid the problem of operating private member BlockControl
	void m_CallBlockDropping();
	void m_CallBlockCreate();
	//void m_CallBlockDestroy();
	// Function m_CanStart() is used to avoid the problem of call private member m_State()
	bool m_CanStart();
	void m_ChangeSpeed(int speedRate);

	void m_MoveButtons();

	void m_CountDown(CDC&);
	//void m_AdjustSilder();
	void m_CalculateScore(int);

	void m_CreateModelUpdateNumber(int);

	//ProjectDlg内有必要多次重复初始化的相关参数(包含成员变量、对象等)重置
	void m_Restart();

	//弹Perfect
	void m_Perfect(CDC& m_cacheDC);

	// MIDI Device
	void ReceiveMsg(DWORD, DWORD);
	void ReceiveMsg(LPSTR Msg, DWORD BytesRecorded, DWORD TimeStamp) {}
	void OnError(DWORD, DWORD);
	void OnError(LPSTR Msg, DWORD BytesRecorded, DWORD TimeStamp) {}
	void OnNoteOn(unsigned char);
	void OnNoteOff(unsigned char);
	void m_LinkDevice();
	void CreateTxetFile();
	void m_ShowCreateModelNumber();
	void m_ShowRank();
	CString m_GetNewMidiPath();
	void m_ClassifyNewMidi(CString);
	int m_CallExecute(CString);

	// MIDI Device
	midi::CMIDIInDevice m_InDevice;
	midi::CMIDIOutDevice m_OutDevice;

	//更新得分
	void UpdateScore(int score);

private:
	bool m_State = FALSE;
	// m_CountDownLock is False means is Counting Down,not ready to Start
	bool m_CountDownLock;
	// m_SliderLock is used to link the Function SliderEvent and SliderControl
	// I use m_SliderLock to create a time interval to decrease the delay cause by
	// reset TimeEvent
	// The Lock's will open when the user chang slider
	// then the OnPaint function will reset the TimeEvent by checking SliderLock
	//bool m_SliderLock = FALSE;
	CRect m_client;
	CRect m_CountDownPosition;
	CRect m_ButtonPosition;
	CImage m_ButtonPicture;
	CImage m_CountDownPicture;
	CImage m_PerfectPicture;
	CDC m_cacheDC;
	CBitmap m_cacheCBitmap;

	// Used to calculate Time Interval
	int m_DropRefreshTime;
	int m_CreateRefreshTime;
	int m_BlockLength;
	// m_CountDownCounter is used to control the speed of counting down
	int m_CountDownCounter;
	// Values to store Scores
		// 0 means the total score
		// 1 means miss
		// 2 means good
		// 3 means great
		// 4 means perfect
	int m_Scores[5];
	int m_Model;//界面类型参数，不同参数对应不同模式下的游戏界面
	bool m_MIDIErrorLock;
	// Used to auto return when nobody press any key in some time
	int  m_ReturnCountDown;
	// Used to record Current Speed
	int m_CurrentSpeed;

	CBackground m_Background;
	CPianoKeyboard m_PianoKeyboard;
	CStave m_Stave;
	CPowerLow m_PowerLow;
	CTitle m_Title;
	CBlockControl m_BlockControl;
	CRuler m_Ruler;

	CBrush m_brush;
	// Used to display the Numbers in create model 
	CString m_CreateModelNumber;

	//CSelectDlg * m_SelectDlg;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonSelect();
	//afx_msg void OnBnClickedButtonLearnMore();
	//afx_msg void OnBnClickedButtonReturn();
	//afx_msg void OnNMCustomdrawSpeedSlider(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl m_SpeedSlider;
protected:
	afx_msg LRESULT OnSelect(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnModel(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	CStatic m_Buffer;

	afx_msg void OnStnClickedOpen();

	//此部分用来调整乐曲速度，即：简单、普通、困难
	//修改时间:2016年6月16日
	afx_msg void OnClickedSpeedControl_1();
	afx_msg void OnClickedSpeedControl_2();
	afx_msg void OnClickedSpeedControl_3();

	CListBox m_ScoreList;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnStnClickedReturn();
	afx_msg void OnStnClickedLearnmore();

	CListBox m_ScoreText;
	afx_msg void OnBnClickedOk();

	//模式选择
	void Show_Model(int model);
	afx_msg void OnStnClickedFreePlay();
	afx_msg void OnStnClickedCycleratio();
	afx_msg void OnStnClickedNaturalnum();
	afx_msg void OnStnClickedIrrationalnum();

	//设置导航栏按钮点击变色
	void  CProjectDlg::Show_NavigationClicked(int kind);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
