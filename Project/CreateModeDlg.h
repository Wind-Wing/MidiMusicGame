
// ProjectDlg.h : ͷ�ļ�
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
#include "afxcmn.h"
#include "afxwin.h"
// MIDI Device
#include "midi.h"
#include "LongMsg.h"
#include "ShortMsg.h"
#include "MIDIInDevice.h"
#include "MIDIOutDevice.h"

// CProjectDlg �Ի���
class CCreateModeDlg : public CDialogEx, public midi::CMIDIReceiver
{
	// ����
public:
	CCreateModeDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CCreateModeDlg();

	// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CREATEMODE_DIALOG };
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
														// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	// My Code
#define TIMER_PRINT 1//1
#define REFRESH_TIME 15//15
#define TIMING_ACCURACY 1//1
	// They are also used in BlockControl.h and PianoKeyboard.h
#define LOW_PITCH 21//24
#define HIGH_PITCH 108//84
#define BOTTOM_TOP 0.02
#define BOTTOM_BOTTON 0.08
#define COUNTERDOWN_STEP 50//50
#define COUNTERDOWN_TIME 150//150
#define CHALLENGE_MODEL 1
#define LEARNING_MODEL 2
#define CREATE_MODEL 3

public:

	int counter;
	//��������
	CFont *font;

	int m_BlockDroppingTimeHandle;
	int m_BlockCreateTimeHandle;

	int perfect_flag;
	int perfect_flag_2;
	int perfect_counter;

	//���ڹ��򻯵÷�
	CString score_buf;

	// Function m_CallBlock() is used to avoid the problem of operating private member BlockControl
	void m_CallBlockDropping();
	void m_CallBlockCreate();
	//void m_CallBlockDestroy();
	// Function m_CanStart() is used to avoid the problem of call private member m_State()
	bool m_CanStart();
	void m_SliderEvent(int speedRate);

	void m_MoveBottoms();

	void m_CountDown(CDC&);
	void m_AdjustSilder();
	void m_CalculateScore(int);

	//ProjectDlg��������ز���(������Ա�����������)����
	void m_Restart();

	//��Perfect
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

	// MIDI Device
	midi::CMIDIInDevice m_InDevice;
	midi::CMIDIOutDevice m_OutDevice;

	//���µ÷�
	void UpdataScore(int score);

private:
	bool m_State = FALSE;
	// m_CountDownLock is False means is Counting Down,not ready to Start
	bool m_CountDownLock = FALSE;
	// m_SliderLock is used to link the Function SliderEvent and SliderControl
	// I use m_SliderLock to create a time interval to decrease the delay cause by
	// reset TimeEvent
	// The Lock's will open when the user chang slider
	// then the OnPaint function will reset the TimeEvent by checking SliderLock
	bool m_SliderLock = FALSE;
	CRect m_client;
	CRect m_BottomPosition;
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

	CBackground m_Background;
	CPianoKeyboard m_PianoKeyboard;
	CStave m_Stave;
	CPowerLow m_PowerLow;
	CTitle m_Title;
	CBlockControl m_BlockControl;
	CRuler m_Ruler;

	CBrush m_brush;

	int m_Model;//�������Ͳ�������ͬ������Ӧ��ͬģʽ�µ���Ϸ����
	bool m_MIDIErrorLock;
	//CSelectDlg * m_SelectDlg;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnNMCustomdrawSpeedSlider(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl m_SpeedSlider;
protected:
	afx_msg LRESULT OnSelect(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnModel(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	CStatic m_Buffer;

	afx_msg void OnStnClickedOpen();

	//�˲����������������ٶȣ������򵥡���ͨ������
	//�޸�ʱ��:2016��6��16��
	afx_msg void OnClickedSpeedControl_1();
	afx_msg void OnClickedSpeedControl_2();
	afx_msg void OnClickedSpeedControl_3();

	CListBox m_ScoreList;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnStnClickedReturn();
	afx_msg void OnStnClickedLearnmore();

	CListBox m_ScoreText;
	afx_msg void OnBnClickedOk();
};